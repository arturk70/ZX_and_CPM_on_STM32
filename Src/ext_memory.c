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
		ILI9341_sendBuf(lru_cache->x, lru_cache->y, lru_cache->x + 7, lru_cache->y + 7, (uint16_t*)(lru_cache->data), CACHE_BLOCK_SIZE/2);
	}

	register uint16_t straddr,xi,yi;

	straddr = addr & ~((uint16_t)CACHE_BLOCK_SIZE-1);
	if(straddr < ILI9341_PWIDTH*(ILI9341_PHEIGHT-192)*2) {
		xi = straddr / CACHE_BLOCK_SIZE;
		yi = xi / (ILI9341_PWIDTH/8) * 8;
		xi = xi % (ILI9341_PWIDTH/8) * 8;
		if(yi >= 24)
			yi += 192;
	}
	else {
		xi = 0;
		yi = 24 + (straddr - ILI9341_PWIDTH*(ILI9341_PHEIGHT-192)*2) / CACHE_BLOCK_SIZE * 8;
		if(yi >= 216) {
			xi = 312;
			yi -= 192;
		}
	}

	lru_cache->straddr = straddr;
	lru_cache->x=xi; lru_cache->y=yi;
	lru_cache->writed = 0;

	ILI9341_readBuf(xi, yi, xi + 7, yi + 7, (uint16_t*)(lru_cache->data), CACHE_BLOCK_SIZE/2);

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
