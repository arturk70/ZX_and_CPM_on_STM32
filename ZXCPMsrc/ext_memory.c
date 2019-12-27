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

uint8_t extmem_rw(register extmem_op_t op, register uint32_t addr, register uint32_t data) {
	register cache_t *cur_cache, *ilru_cache, *imru_cache;
	register uint8_t *cmapptr, *cmapblkptr;
	register uint32_t blknum;
	ilru_cache = lru_cache;
	imru_cache = mru_cache;
	cmapptr = cache_map;
	blknum = addr >> 7;
	cmapblkptr = &cmapptr[blknum];

	if(*cmapblkptr == 0xff) { //no cache found for address
		if(next_unused < cache + CACHE_BLOCKS_NUM) {
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

<<<<<<< HEAD
		if(lru_cache->writed) {
			ILI9341_sendBuf(lru_cache->x, lru_cache->y, lru_cache->x + 7, lru_cache->y + 7, (uint16_t*)(lru_cache->data), 64);
=======
		if(cur_cache->writed) {
			ILI9341_sendBuf(cur_cache->x, cur_cache->y, cur_cache->x + 7, cur_cache->y + 7, (uint16_t*)(cur_cache->data), 64);
>>>>>>> a85d6ec031f3b0037ff72383c0c772f9410b92b3
		}

		register uint16_t xi,yi;

		if(blknum < 240) {
			yi = (blknum / 40) * 8;
			xi = (blknum % 40) * 8;
			if(yi >= 24)
				yi += 192;
		}
		else {
			yi = (blknum - 240) * 8;
			xi = yi / 192 * 8;
			yi = yi % 192 + 24;
			if(xi >= 16) {
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
		mru_cache = cur_cache;
		cur_cache->next = NULL;
	}

	lru_cache = ilru_cache;

	if(op == EXTM_READ)
		return cur_cache->data[addr & 0x007f];
	else {
		cur_cache->writed = 1;
		return cur_cache->data[addr & 0x007f] = data;
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

