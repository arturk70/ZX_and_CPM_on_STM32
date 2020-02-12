/*
 * memory.h
 *
 *  Created on: Oct 8, 2019
 *      Author: artur
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#include "ext_memory.h"

#define ZXROMSIZE	0x4000
#define INTRAMSIZE	0x1c00
#define	CPMMEMSIZE	(48*1024)

#define MEM_READ(addr) (*((uint8_t*)mem_ptr(addr, 0)))
//#define MEM_READ16(addr) (*((uint16_t*)mem_ptr(addr, 0)))
#define MEM_WRITE(addr, data) {*((uint8_t*)mem_ptr(addr, 1)) = data;}
//#define MEM_WRITE16(addr, data) {*((uint16_t*)mem_ptr(addr, 1)) = data;}

typedef enum {
	MEMTYPE_ZX,
	MEMTYPE_CPM
} memtype_t;

extern uint8_t mem[];

extern void* (*mem_ptr)(register uint32_t addr, register uint32_t write_flag);

void* zxmem_ptr(register uint32_t addr, register uint32_t write_flag);
void* cpmmem_ptr(register uint32_t addr, register uint32_t write_flag);
void mem_Init(register memtype_t type);

#endif /* MEMORY_H_ */
