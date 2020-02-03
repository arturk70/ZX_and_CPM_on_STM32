/*
 * ext_memory.c
 *
 *  Created on: Oct 8, 2019
 *      Author: artur
 */

#include "ext_memory.h"

static uint8_t cache_map[MEM_BLOCS_NUM+1];// + 1 fake block
static cache_t cache[CACHE_BLOCKS_NUM];
static cache_t *lru_cache, *mru_cache, *next_unused;

uint32_t mem_time = 0;

//addr & 0xf0000000 --- 0-read, 1-write
uint8_t extmem_rw(register uint32_t addr, register uint32_t data) {
	register cache_t *cur_cache, *ilru_cache, *imru_cache;
	register uint8_t *cmapptr, *cmapblkptr;
	register uint32_t blknum;
	ilru_cache = lru_cache;//less recently used block
	imru_cache = mru_cache;//most recently used block
	cmapptr = cache_map;
	blknum = (addr & 0xffff) >> 7;
	cmapblkptr = &cmapptr[blknum];

	if(*cmapblkptr == 0xff) { //no cache found for address
		if(next_unused < cache + CACHE_BLOCKS_NUM) {//not all cache blocks used
			cur_cache = next_unused++;
			if(!ilru_cache)
				ilru_cache = cur_cache;
		}
		else {
			cur_cache = ilru_cache;
			if(cur_cache->next) {
				ilru_cache = cur_cache->next;
				ilru_cache->prev = NULL;
			}
		}

		*cmapblkptr = (cur_cache - cache);
		cmapptr[cur_cache->blknum] = 0xff;

		if(cur_cache->writed) {
			ILI9341_sendBuf(cur_cache->x, cur_cache->y, cur_cache->x + 7, cur_cache->y + 7, (uint16_t*)(cur_cache->data), 64);
		}

		//calculate coordinates of block on screen
		register uint16_t xi,yi;

		if(blknum < 240) {//block in horizontal bar
			yi = blknum / 40;
			xi = (blknum - yi*40) * 8;
			yi = yi * 8;
			if(yi >= 24)//block in bottom bar
				yi += 192;
		}
		else {//block in vertical bar
			yi = (blknum - 240) * 8;
			xi = yi / 192;
			yi = (yi - xi * 192) + 24;
			xi = xi * 8;
			if(xi >= 16) {//block in right bar
				xi += 288;
			}
		}

		cur_cache->blknum = blknum;
		cur_cache->x=xi; cur_cache->y=yi;
		cur_cache->writed = 0;

		ILI9341_readBuf(xi, yi, xi + 7, yi + 7, (uint16_t*)(cur_cache->data), 64);
	}
	else {
		cur_cache = &cache[*cmapblkptr];
		if(imru_cache != cur_cache) {
			if(cur_cache->prev)
				((cache_t*)(cur_cache->prev))->next = cur_cache->next;
			if(cur_cache->next) {
				if(cur_cache == ilru_cache)
					ilru_cache = cur_cache->next;
				((cache_t*)(cur_cache->next))->prev = cur_cache->prev;
			}
		}
	}

	if(imru_cache != cur_cache) {
		if(imru_cache)
			imru_cache->next = cur_cache;
		cur_cache->prev = imru_cache;
		cur_cache->next = NULL;
		mru_cache = cur_cache;
	}

	lru_cache = ilru_cache;

	if(addr & WRITE_OP) {//write
		cur_cache->writed = 1;
		return cur_cache->data[addr & 0x007f] = data;
	}
	else {//read
		return cur_cache->data[addr & 0x007f];
	}
}

void extmem_Init() {
	for(register int i=0; i<MEM_BLOCS_NUM+1; i++)
			cache_map[i] = 0xff;

	lru_cache = NULL;
	mru_cache = NULL;
	next_unused = cache;

	for(register int i=0; i<CACHE_BLOCKS_NUM;i++) {
			cache[i].blknum = MEM_BLOCS_NUM;
			cache[i].writed = 0;
			cache[i].prev = NULL;
			cache[i].next = NULL;
	}
}

