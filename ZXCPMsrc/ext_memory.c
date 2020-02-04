/*
 * ext_memory.c
 *
 *  Created on: Oct 8, 2019
 *      Author: artur
 */

#include "ext_memory.h"

static cache_t* cache_map[MEM_BLOCS_NUM];
static cache_t cache[CACHE_BLOCKS_NUM];
static cache_t *lru_cache, *mru_cache;

uint32_t mem_time = 0;

//addr & 0xf0000000 --- 0-read, 1-write
uint8_t* extmem_armaddr(register uint32_t addr) {
	register cache_t *cur_cache, *ilru_cache, *imru_cache;
	register cache_t **cmapptr;
	register uint32_t blknum;
	ilru_cache = lru_cache;//less recently used block
	imru_cache = mru_cache;//most recently used block
	cmapptr = cache_map;
	blknum = (addr & 0xffff) >> 7;//calculate number of block on the screen
	cur_cache = cmapptr[blknum];//get pointer to number of cache

	if(cur_cache) {//cache found for address
		if(imru_cache != cur_cache) {//current cache block is not most recently used cache block
			//crop them from middle of sorted by usage cache blocks array
			if(cur_cache == ilru_cache)
				ilru_cache = cur_cache->next;
			else
				((cache_t*)(cur_cache->prev))->next = cur_cache->next;
			((cache_t*)(cur_cache->next))->prev = cur_cache->prev;

			//set current cache as most recently used
			imru_cache->next = cur_cache;
			cur_cache->prev = imru_cache;
			cur_cache->next = NULL;
			mru_cache = cur_cache;
		}
	}
	else { //no cache found for address
		cur_cache = ilru_cache;//then use less recently used
		ilru_cache = ilru_cache->next;
		ilru_cache->prev = NULL;

		if(cur_cache->writed) {//if data in cache was changed
			//write cache block back to the screen
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

		//set cache in cache map for block
		cmapptr[blknum] = cur_cache;
		//clear previous cache block in cache map
		cmapptr[cur_cache->blknum] = NULL;

		//set parameters of new cache block
		cur_cache->blknum = blknum;
		cur_cache->x=xi; cur_cache->y=yi;
		cur_cache->writed = 0;
		//read data of new cache block from the screen
		ILI9341_readBuf(xi, yi, xi + 7, yi + 7, (uint16_t*)(cur_cache->data), 64);

		//set current cache as most recently used
		imru_cache->next = cur_cache;
		cur_cache->prev = imru_cache;
		cur_cache->next = NULL;
		mru_cache = cur_cache;
	}


	lru_cache = ilru_cache;

	if(addr & WRITE_OP) //write
		cur_cache->writed = 1;

	return cur_cache->data +(addr & 0x007f);

}

void extmem_Init() {
	register int i;
	register uint16_t xi,yi;
	register cache_t *cur_cache;

	for(i=0; i<CACHE_BLOCKS_NUM;i++) {
		cur_cache = cache + i;
		cur_cache->blknum = i;
		cur_cache->writed = 0;
		if(i == 0) {
			cur_cache->prev = NULL;
			lru_cache = cur_cache;
		}
		else
			cur_cache->prev = cur_cache - 1;
		if(i == CACHE_BLOCKS_NUM - 1) {
			cur_cache->next = NULL;
			mru_cache = cur_cache;
		}
		else
			cur_cache->next = cur_cache + 1;

		yi = i / 40;
		xi = (i - yi*40) * 8;
		yi = yi * 8;
		cur_cache->x = xi;
		cur_cache->y = yi;
		ILI9341_readBuf(xi, yi, xi + 7, yi + 7, (uint16_t*)(cur_cache->data), 64);

		cache_map[i] = cur_cache;
	}

	for(; i<MEM_BLOCS_NUM; i++)
			cache_map[i] = NULL;
}

