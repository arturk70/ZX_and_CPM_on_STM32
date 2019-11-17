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

uint32_t mem_time = 0;

static cache_t cache[CACHE_BLOCKS_NUM];

static uint8_t search_cache(uint16_t addr) {//return index of appropriate cache block, update if need
	register uint8_t lru_num = 0;

	for(register uint8_t i=0; i<CACHE_BLOCKS_NUM;i++) {
		if((addr >= cache[i].straddr) && (addr <= (cache[i].straddr+CACHE_BLOCK_SIZE-1)))
			return i;
		if(cache[i].usaget < cache[lru_num].usaget) lru_num = i;
	}

//	UINT size;
	if(cache[lru_num].writed) {
//		f_lseek(&USERFile, cache[lru_num].straddr);
//		f_write(&USERFile, cache[lru_num].data, CACHE_BLOCK_SIZE, &size);
		SD_writeblock((uint32_t)(cache[lru_num].straddr >> 5) + SWAP_PARTITION_START_BLOCK, cache[lru_num].data);
	}

	cache[lru_num].straddr = addr & 0xfe00;
	cache[lru_num].usaget = mem_time;
	cache[lru_num].writed = 0;
//	f_lseek(&USERFile, cache[lru_num].straddr);
//	f_read(&USERFile, cache[lru_num].data, CACHE_BLOCK_SIZE, &size);
	SD_readblock((uint32_t)(cache[lru_num].straddr >> 5) + SWAP_PARTITION_START_BLOCK, cache[lru_num].data);

	return lru_num;
}

uint8_t extmem_read(uint16_t addr) {
	register uint8_t blocknum = search_cache(addr);

	cache[blocknum].usaget = mem_time;

	return cache[blocknum].data[addr - cache[blocknum].straddr];

}

void extmem_write(uint16_t addr, uint8_t data) {
	register uint8_t blocknum = search_cache(addr);

	cache[blocknum].usaget = mem_time;
	cache[blocknum].writed = 1;

	cache[blocknum].data[addr - cache[blocknum].straddr] = data;
}

void extmem_Init() {
	for(register uint8_t i=0; i<CACHE_BLOCKS_NUM;i++) {
			cache[i].straddr = 0xffff;
			cache[i].usaget = 0;
			cache[i].writed = 0;
	}

	//f_open(&USERFile, "0:/PAGEFILE", FA_WRITE | FA_READ);
}

void extmem_deInit() {
//	f_close(&USERFile);
}
