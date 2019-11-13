/*
 * ext_memory.c
 *
 *  Created on: Oct 8, 2019
 *      Author: artur
 */

//#include "fatfs.h"
#include "SD_driver.h"
#include "ext_memory.h"

#define SWAP_PARTITION_START_BLOCK	2048

static cache_t cache[CACHE_BLOCKS_NUM];

static uint8_t search_cache(uint16_t addr) {//return index of appropriate cache block, update if need
	register uint8_t lfu_num = 0;

	for(register uint8_t i=0; i<CACHE_BLOCKS_NUM;i++) {
		if((addr >= cache[i].straddr) && (addr <= (cache[i].straddr+CACHE_BLOCK_SIZE-1)))
			return i;
		if(cache[i].usage < cache[lfu_num].usage) lfu_num = i;
	}

//	UINT size;
	if(cache[lfu_num].writed) {
//		f_lseek(&USERFile, cache[lfu_num].straddr);
//		f_write(&USERFile, cache[lfu_num].data, CACHE_BLOCK_SIZE, &size);
		SD_writeblock((uint32_t)(cache[lfu_num].straddr >> 5) + SWAP_PARTITION_START_BLOCK, cache[lfu_num].data);
	}

	cache[lfu_num].straddr = addr & 0xfe00;
	cache[lfu_num].usage = cache[lfu_num].writed = 0;
//	f_lseek(&USERFile, cache[lfu_num].straddr);
//	f_read(&USERFile, cache[lfu_num].data, CACHE_BLOCK_SIZE, &size);
	SD_readblock((uint32_t)(cache[lfu_num].straddr >> 5) + SWAP_PARTITION_START_BLOCK, cache[lfu_num].data);

	return lfu_num;
}

uint8_t extmem_read(uint16_t addr) {
	register uint8_t blocknum = search_cache(addr);

	cache[blocknum].usage++;

	return cache[blocknum].data[addr - cache[blocknum].straddr];

}

void extmem_write(uint16_t addr, uint8_t data) {
	register uint8_t blocknum = search_cache(addr);

	cache[blocknum].usage++;
	cache[blocknum].writed = 1;

	cache[blocknum].data[addr - cache[blocknum].straddr] = data;
}

void extmem_Init() {
	for(register uint8_t i=0; i<CACHE_BLOCKS_NUM;i++) {
			cache[i].straddr = 0xffff;
			cache[i].usage = 0;
			cache[i].writed = 0;
	}

	//f_open(&USERFile, "0:/PAGEFILE", FA_WRITE | FA_READ);
}

void extmem_deInit() {
//	f_close(&USERFile);
}
