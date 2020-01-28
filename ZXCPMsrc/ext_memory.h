/*
 * ext_memory.h
 *
 *  Created on: Oct 8, 2019
 *      Author: artur
 */

#ifndef EXT_MEMORY_H_
#define EXT_MEMORY_H_

#include "main.h"
#include "ili9341_driver.h"

#define MEM_BLOCS_NUM		336 // 336 = (320*24*2+8*192*2*2)/64 - number of 128 byte blocks in external memory
#define CACHE_BLOCKS_NUM	40
#define WRITE_OP	0xf0000000

typedef struct {
	uint8_t data[128];
	uint16_t blknum;
	uint8_t writed; //write flag
	uint16_t x,y;
	void *prev;
	void *next;
} cache_t;

//addr & 0xf0000000 --- 0-read, 1-write
uint8_t extmem_rw(register uint32_t addr, register uint32_t data);
void extmem_Init();
//void extmem_deInit();

#endif /* EXT_MEMORY_H_ */
