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
#define CACHE_BLOCKS_NUM	20

typedef struct {
	uint16_t straddr;
	uint32_t usaget; //last read or write time
	uint8_t writed; //write flag
	uint8_t data[CACHE_BLOCK_SIZE];
	uint16_t x,y;
} cache_t;

typedef enum {
	EXTM_READ,
	EXTM_WRITE
} extmem_op_t;

uint8_t extmem_rw(register extmem_op_t op, register uint16_t addr, register uint8_t data);
void extmem_Init();

#endif /* EXT_MEMORY_H_ */
