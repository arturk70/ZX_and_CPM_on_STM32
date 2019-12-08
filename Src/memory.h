/*
 * memory.h
 *
 *  Created on: Oct 8, 2019
 *      Author: artur
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#include "ext_memory.h"

#define MEMTYPE_ZX	0
#define MEMTYPE_CPM	1

extern uint8_t mem[];

extern uint8_t (*mem_read)(register uint16_t addr);
extern void (*mem_write)(register uint16_t addr, register uint8_t data);

uint8_t zxmem_read(register uint16_t addr);
void zxmem_write(register uint16_t addr, register uint8_t data);
uint8_t cpmmem_read(register uint16_t addr);
void cpmmem_write(register uint16_t addr, register uint8_t data);
void mem_Init(uint8_t type);

#endif /* MEMORY_H_ */
