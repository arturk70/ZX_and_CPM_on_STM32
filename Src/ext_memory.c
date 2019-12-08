/*
 * ext_memory.c
 *
 *  Created on: Oct 8, 2019
 *      Author: artur
 */

#include "ext_memory.h"

static uint16_t bl[4], br[4], bt[4], bb[4]; // 0-top bar, 1 - left bar, 2 - right bar, 3 - bottom bar
static uint16_t bl_straddr[4], bl_endaddr[4];
static cache_t cache[CACHE_BLOCKS_NUM];

uint32_t mem_time = 0;

static uint16_t calc_cache_block(register uint16_t addr, uint16_t *x1, uint16_t *y1, uint16_t *x2, uint16_t *y2) {
	register uint16_t straddr = addr & ~((uint16_t)CACHE_BLOCK_SIZE-1);

	register uint8_t blnum = 0;
	while((straddr < bl_straddr[blnum]) || (straddr > bl_endaddr[blnum])) blnum++;

	register uint16_t pixnum = (straddr - bl_straddr[blnum]) / 2;
	*x1 = bl[blnum] + pixnum % (br[blnum] - bl[blnum] + 1);
	*y1 = bt[blnum] + pixnum / (br[blnum] - bl[blnum] + 1);

	if(straddr/2 < ILI9341_PWIDTH*(ILI9341_PHEIGHT-192)) {
		*x2 = *x1 + CACHE_BLOCK_SIZE/2 - 1; *y2 = *y1;
	}
	else {
		*x2 = *x1 + 7; *y2 = *y1 + CACHE_BLOCK_SIZE/16 - 1;
	}

	return straddr;
}

static cache_t* search_cache(register uint16_t addr) {//return index of appropriate cache block, update if need
	register cache_t* lru_cache = cache;

	for(register uint8_t i=0; i<CACHE_BLOCKS_NUM;i++) {
		if((addr >= cache[i].straddr) && (addr <= (cache[i].straddr+CACHE_BLOCK_SIZE-1)))
			return &cache[i];
		if(cache[i].usaget < lru_cache->usaget) lru_cache = &cache[i];
	}

	uint16_t x1, y1, x2, y2;

	if(lru_cache->writed) {
		calc_cache_block(lru_cache->straddr, &x1, &y1, &x2, &y2);
		ILI9341_sendBuf(x1, y1, x2, y2, (uint16_t*)(lru_cache->data), CACHE_BLOCK_SIZE/2);
	}

	lru_cache->straddr = calc_cache_block(addr, &x1, &y1, &x2, &y2);
	lru_cache->writed = 0;
	ILI9341_readBuf(x1, y1, x2, y2, (uint16_t*)(lru_cache->data), CACHE_BLOCK_SIZE/2);

	return lru_cache;
}

uint8_t extmem_read(register uint16_t addr) {
	register cache_t* block = search_cache(addr);

	block->usaget = mem_time++;

	return block->data[addr - block->straddr];

}

void extmem_write(register uint16_t addr, register uint8_t data) {
	register cache_t* block = search_cache(addr);

	block->usaget = mem_time++;
	block->writed = 1;

	block->data[addr - block->straddr] = data;
}

void extmem_Init() {

	bl[0] = 0;   br[0] = ILI9341_PWIDTH-1; bt[0] = 0;   bb[0] = 23;
	bl[1] = 0;   br[1] = ILI9341_PWIDTH-1; bt[1] = 216; bb[1] = ILI9341_PHEIGHT-1;
	bl[2] = 0;   br[2] = 7;                bt[2] = 24;  bb[2] = 215;
	bl[3] = 312; br[3] = ILI9341_PWIDTH-1; bt[3] = 24;  bb[3] = 215;
	bl_straddr[0] = 0;
	bl_endaddr[0]=(br[0]-bl[0]+1)*(bb[0]-bt[0]+1)*2-1;
	bl_straddr[1] = bl_endaddr[0]+1;
	bl_endaddr[1]=bl_straddr[1]+(br[1]-bl[1]+1)*(bb[1]-bt[1]+1)*2-1;
	bl_straddr[2] = bl_endaddr[1]+1;
	bl_endaddr[2]=bl_straddr[2]+(br[2]-bl[2]+1)*(bb[2]-bt[2]+1)*2-1;
	bl_straddr[3] = bl_endaddr[2]+1;
	bl_endaddr[3]=bl_straddr[3]+(br[3]-bl[3]+1)*(bb[3]-bt[3]+1)*2-1;
	for(uint8_t i=0; i<CACHE_BLOCKS_NUM;i++) {
			cache[i].straddr = 0xffff;
			cache[i].usaget = 0;
			cache[i].writed = 0;
	}
}

//void extmem_deInit() {
//}
