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

static uint16_t calc_cache_block(uint16_t addr, uint16_t *x1, uint16_t *y1, uint16_t *x2, uint16_t *y2) {
	uint16_t straddr = addr & ~((uint16_t)CACHE_BLOCK_SIZE-1);

	uint8_t blnum = 0;
	while((straddr < bl_straddr[blnum]) || (straddr > bl_endaddr[blnum])) blnum++;

	uint16_t pixnum = (addr-bl_straddr[blnum]) / 2;
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

static uint8_t search_cache(uint16_t addr) {//return index of appropriate cache block, update if need
	uint8_t lfu_num = 0;
	for(uint8_t i=0; i<CACHE_BLOCKS_NUM;i++) {
		if((addr >= cache[i].straddr) && (addr < (cache[i].straddr+CACHE_BLOCK_SIZE)))
			return i;
		if(cache[i].usage < cache[lfu_num].usage) lfu_num = i;
	}

	uint16_t x1, y1, x2, y2;

	if(cache[lfu_num].writed) {
		calc_cache_block(cache[lfu_num].straddr, &x1, &y1, &x2, &y2);
		ILI9341_sendBuf(x1, y1, x2, y2, (uint16_t*)cache[lfu_num].data, CACHE_BLOCK_SIZE/2);
	}

	cache[lfu_num].straddr = calc_cache_block(addr, &x1, &y1, &x2, &y2);
	cache[lfu_num].usage = cache[lfu_num].writed = 0;
	ILI9341_readBuf(x1, y1, x2, y2, (uint16_t*)cache[lfu_num].data, CACHE_BLOCK_SIZE/2);

	return lfu_num;
}

uint8_t extmem_read(uint16_t addr) {
	uint8_t blocknum = search_cache(addr);

	cache[blocknum].usage++;

	return cache[blocknum].data[addr - cache[blocknum].straddr];

}

void extmem_write(uint16_t addr, uint8_t data) {
	uint8_t blocknum = search_cache(addr);

	cache[blocknum].usage++;
	cache[blocknum].writed = 1;

	cache[blocknum].data[addr - cache[blocknum].straddr] = data;
}

void extmem_Init(uint16_t b0l, uint16_t b0r, uint16_t b0t, uint16_t b0b,
		uint16_t b1l, uint16_t b1r, uint16_t b1t, uint16_t b1b,
		uint16_t b2l, uint16_t b2r, uint16_t b2t, uint16_t b2b,
		uint16_t b3l, uint16_t b3r, uint16_t b3t, uint16_t b3b) {
	bl[0] = b0l; br[0] = b0r; bt[0] = b0t; bb[0] = b0b;
	bl[1] = b1l; br[1] = b1r; bt[1] = b1t; bb[1] = b1b;
	bl[2] = b2l; br[2] = b2r; bt[2] = b2t; bb[2] = b2b;
	bl[3] = b3l; br[3] = b3r; bt[3] = b3t; bb[3] = b3b;
	bl_straddr[0] = 0;
	bl_endaddr[0]=(br[0]-bl[0]+1)*(bb[0]-bt[0]+1)*2-1;
	bl_straddr[1] = bl_endaddr[0]+1;
	bl_endaddr[1]=bl_straddr[1]+(br[1]-bl[1]+1)*(bb[1]-bt[1]+1)*2-1;
	bl_straddr[2] = bl_endaddr[1]+1;
	bl_endaddr[2]=bl_straddr[2]+(br[2]-bl[2]+1)*(bb[2]-bt[2]+1)*2-1;
	bl_straddr[3] = bl_endaddr[2]+1;
	bl_endaddr[3]=bl_straddr[3]+(br[3]-bl[3]+1)*(bb[3]-bt[3]+1)*2-1;
	for(uint8_t i=1; i<CACHE_BLOCKS_NUM;i++)
			cache[i].straddr = 0xffff;
}
