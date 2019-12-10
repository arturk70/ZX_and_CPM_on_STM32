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

#define CACHE_BLOCK_SIZE	64
#define CACHE_BLOCKS_NUM	32

typedef struct {
	uint16_t straddr;
	uint32_t usaget; //last read or write time
	uint8_t writed; //write flag
	uint8_t data[CACHE_BLOCK_SIZE];
	uint16_t x1,y1,x2,y2;
} cache_t;

uint8_t extmem_read(register uint16_t addr);
void extmem_write(register uint16_t addr, register uint8_t data);
void extmem_Init();
//void extmem_deInit();

#endif /* EXT_MEMORY_H_ */
