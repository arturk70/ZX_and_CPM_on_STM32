/*
 * ext_memory.c
 *
 *  Created on: Oct 8, 2019
 *      Author: artur
 */

#include "ext_memory.h"

static cache_t cache[CACHE_BLOCKS_NUM];

uint32_t mem_time = 0;

static cache_t* search_cache(register uint16_t addr) {//return index of appropriate cache block, update if need
	register cache_t* lru_cache = cache;

	for(register uint8_t i=0; i<CACHE_BLOCKS_NUM;i++) {
		if((addr >= cache[i].straddr) && (addr <= (cache[i].straddr+CACHE_BLOCK_SIZE-1)))
			return &cache[i];
		if(cache[i].usaget < lru_cache->usaget) lru_cache = &cache[i];
	}

	if(lru_cache->writed) {
		ILI9341_sendBuf(lru_cache->x1, lru_cache->y1, lru_cache->x2, lru_cache->y2, (uint16_t*)(lru_cache->data), CACHE_BLOCK_SIZE/2);
	}

	register uint16_t straddr,x1i,x2i,y1i,y2i;

	straddr = addr & ~((uint16_t)CACHE_BLOCK_SIZE-1);
	if(straddr < ILI9341_PWIDTH*(ILI9341_PHEIGHT-192)*2) {
		x1i = (straddr / 2) % ILI9341_PWIDTH;
		y1i = (straddr / 2) / ILI9341_PWIDTH;
		if(y1i >= 24)
			y1i += 192;
		x2i = x1i + CACHE_BLOCK_SIZE/2 - 1; y2i = y1i;
	}
	else {
		x1i = 0;
		y1i = 24+(straddr / 2 - ILI9341_PWIDTH*(ILI9341_PHEIGHT-192)) / 8;
		if(y1i >= 216) {
			x1i = 312;
			y1i -= 192;
		}
		x2i = x1i + 7; y2i = y1i + CACHE_BLOCK_SIZE/16 - 1;
	}

	lru_cache->straddr = straddr;
	lru_cache->x1=x1i; lru_cache->x2=x2i; lru_cache->y1=y1i; lru_cache->y2=y2i;
	lru_cache->writed = 0;

	ILI9341_readBuf(x1i, y1i, x2i, y2i, (uint16_t*)(lru_cache->data), CACHE_BLOCK_SIZE/2);

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

	for(uint8_t i=0; i<CACHE_BLOCKS_NUM;i++) {
			cache[i].straddr = 0xffff;
			cache[i].usaget = 0;
			cache[i].writed = 0;
	}
}

//void extmem_deInit() {
//}
