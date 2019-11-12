/*
 * ext_memory.c
 *
 *  Created on: Oct 8, 2019
 *      Author: artur
 */

#include "ext_memory.h"

static FIL pagefile;
static cache_t cache[CACHE_BLOCKS_NUM];

static uint8_t search_cache(uint16_t addr) {//return index of appropriate cache block, update if need
	register uint8_t lfu_num = 0;
	uint32_t size;
	for(uint8_t i=0; i<CACHE_BLOCKS_NUM;i++) {
		if((addr >= cache[i].straddr) && (addr < (cache[i].straddr+CACHE_BLOCK_SIZE)))
			return i;
		if(cache[i].usage < cache[lfu_num].usage) lfu_num = i;
	}

	if(cache[lfu_num].writed) {
		f_lseek(&pagefile, cache[lfu_num].straddr);
		f_write(&pagefile, cache[lfu_num].data, CACHE_BLOCK_SIZE, &size);
	}

	cache[lfu_num].straddr = addr & 0xfe00;
	cache[lfu_num].usage = cache[lfu_num].writed = 0;
	f_lseek(&pagefile, cache[lfu_num].straddr);
	f_read(&pagefile, cache[lfu_num].data, CACHE_BLOCK_SIZE, &size);

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

void extmem_Init() {
	for(uint8_t i=0; i<CACHE_BLOCKS_NUM;i++) {
			cache[i].straddr = 0xffff;
			cache[i].usage = 0;
			cache[i].writed = 0;
	}

	f_open(&pagefile, "0:/PAGEFILE", FA_WRITE | FA_READ);
}

void extmem_deInit() {
	f_close(&pagefile);
}
