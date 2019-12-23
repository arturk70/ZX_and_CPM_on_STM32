/*
 * memory.h
 *
 *  Created on: Oct 8, 2019
 *      Author: artur
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#include "ext_memory.h"

typedef enum {
	MEMTYPE_ZX,
	MEMTYPE_CPM
} memtype_t;

extern uint8_t mem[];

extern uint8_t (*mem_read)(register uint32_t addr);
extern void (*mem_write)(register uint32_t addr, register uint32_t data);

uint8_t zxmem_read(register uint32_t addr);
void zxmem_write(register uint32_t addr, register uint32_t data);
uint8_t cpmmem_read(register uint32_t addr);
void cpmmem_write(register uint32_t addr, register uint32_t data);
void mem_Init(register memtype_t type);

#endif /* MEMORY_H_ */
