/*
 * ext_memory.h
 *
 *  Created on: Oct 8, 2019
 *      Author: artur
 */

#ifndef EXT_MEMORY_H_
#define EXT_MEMORY_H_

#include "main.h"
#include "fatfs.h"

#define CACHE_BLOCK_SIZE	512
#define CACHE_BLOCKS_NUM	8

typedef struct {
	uint16_t straddr;
	uint16_t usage; //read or write counter
	uint8_t writed; //write flag
	uint8_t data[CACHE_BLOCK_SIZE];
} cache_t;

uint8_t extmem_read(uint16_t addr);
void extmem_write(uint16_t addr, uint8_t data);
void extmem_Init();
void extmem_deInit();

#endif /* EXT_MEMORY_H_ */
