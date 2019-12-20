/*
 * ext_memory.c
 *
 *  Created on: Oct 8, 2019
 *      Author: artur
 */

#include "ext_memory.h"

static uint8_t cache_map[288];
static cache_t cache[CACHE_BLOCKS_NUM];

uint32_t mem_time = 0;

uint8_t extmem_rw(register extmem_op_t op, register uint16_t addr, register uint8_t data) {
	register cache_t *cur_cache, *last_cache, *lru_cache;
	register uint8_t* cmapptr = cache_map;
	cur_cache = lru_cache = cache;
	last_cache = cur_cache + CACHE_BLOCKS_NUM;

	register uint16_t straddr = addr >> 7;
	if(cmapptr[straddr] == 0xff) { //no cache found
		while(cur_cache < last_cache) {
			if(cur_cache->usaget < lru_cache->usaget) lru_cache = cur_cache;
			cur_cache++;
		}

		cmapptr[straddr] = (lru_cache - cache);
		cmapptr[lru_cache->straddr >> 7] = 0xff;

		if(lru_cache->writed) {
			ILI9341_sendBuf(lru_cache->x, lru_cache->y, lru_cache->x + 7, lru_cache->y + 7, (uint16_t*)(lru_cache->data), CACHE_BLOCK_SIZE/2);
		}

		register uint16_t xi,yi;

		straddr = straddr << 7;
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
	}
	else {
		lru_cache = &cache[cmapptr[straddr]];
		straddr = straddr << 7;
	}

	lru_cache->usaget = mem_time++;

	if(op == EXTM_READ)
		return lru_cache->data[addr - straddr];
	else {
		lru_cache->writed = 1;
		return lru_cache->data[addr - straddr] = data;
	}
}

void extmem_Init() {

	for(int i=0; i<CACHE_BLOCKS_NUM;i++) {
			cache[i].straddr = 0xffff;
			cache[i].usaget = 0;
			cache[i].writed = 0;
	}
	for(int i=0; i<288; i++)
		cache_map[i] = 0xff;
}

