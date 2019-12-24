/*
 * ext_memory.c
 *
 *  Created on: Oct 8, 2019
 *      Author: artur
 */

#include "ext_memory.h"

static uint8_t cache_map[289];// 288 = (320*24*2+8*192*2)/64 - number of 128 byte blocks in external memory + 1 fake block
static cache_t cache[CACHE_BLOCKS_NUM];

uint32_t mem_time = 0;

uint8_t extmem_rw(register extmem_op_t op, register uint32_t addr, register uint32_t data) {
	register cache_t *cur_cache, *last_cache, *lru_cache;
	register uint8_t *cmapptr, *cmapblkptr;
	register uint32_t blknum;
	cmapptr = cache_map;
	blknum = addr >> 7;
	cmapblkptr = &cmapptr[blknum];

	if(*cmapblkptr == 0xff) { //no cache found
		cur_cache = lru_cache = cache;
		last_cache = cur_cache + CACHE_BLOCKS_NUM;

		while(cur_cache < last_cache) {
			if(cur_cache->usaget < lru_cache->usaget) lru_cache = cur_cache;
			cur_cache++;
		}

		*cmapblkptr = (lru_cache - cache);
		cmapptr[lru_cache->blknum] = 0xff;

		if(lru_cache->writed) {
			ILI9341_sendBuf(lru_cache->x, lru_cache->y, lru_cache->x + 7, lru_cache->y + 7, (uint16_t*)(lru_cache->data), 64);
		}

		register uint16_t xi,yi;

		if(blknum < 240) {
			yi = (blknum / 40) * 8;
			xi = (blknum % 40) * 8;
			if(yi >= 24)
				yi += 192;
		}
		else {
			xi = 0;
			yi = (blknum - 240) * 8 + 24;
			if(yi >= 216) {
				xi = 312;
				yi -= 192;
			}
		}

		lru_cache->blknum = blknum;
		lru_cache->x=xi; lru_cache->y=yi;
		lru_cache->writed = 0;

		ILI9341_readBuf(xi, yi, xi + 7, yi + 7, (uint16_t*)(lru_cache->data), 64);
	}
	else {
		lru_cache = &cache[*cmapblkptr];
	}

	lru_cache->usaget = mem_time++;

	if(op == EXTM_READ)
		return lru_cache->data[addr & 0x007f];
	else {
		lru_cache->writed = 1;
		return lru_cache->data[addr & 0x007f] = data;
	}
}

void extmem_Init() {

	for(register int i=0; i<CACHE_BLOCKS_NUM;i++) {
			cache[i].blknum = 288;//initialize to fake block
			cache[i].usaget = 0;
			cache[i].writed = 0;
	}
	for(register int i=0; i<289; i++)
		cache_map[i] = 0xff;
}

