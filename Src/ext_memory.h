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

#define CACHE_BLOCK_SIZE	128
#define CACHE_BLOCKS_NUM	16

typedef struct {
	uint16_t straddr;
	uint32_t usaget; //last read or write time
	uint8_t writed; //write flag
	uint8_t data[CACHE_BLOCK_SIZE];
} cache_t;

extern uint32_t mem_time;

uint8_t extmem_read(uint16_t addr);
void extmem_write(uint16_t addr, uint8_t data);
void extmem_Init();
//void extmem_deInit();

#endif /* EXT_MEMORY_H_ */
