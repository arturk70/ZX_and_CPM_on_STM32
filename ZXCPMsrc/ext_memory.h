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
#define CACHE_BLOCKS_NUM	42
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
//return address in ARM memory of cached byte
uint8_t* extmem_armaddr(register uint32_t addr);
void extmem_Init();

#endif /* EXT_MEMORY_H_ */
