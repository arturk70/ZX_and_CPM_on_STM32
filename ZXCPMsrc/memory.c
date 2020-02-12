/*
 * memory.c
 *
 *  Created on: Oct 8, 2019
 *      Author: artur
 */

#include <stdlib.h>
#include "memory.h"
#include "ZXROM.h"

uint8_t mem[INTRAMSIZE];
uint16_t fakeROM;

void* (*mem_ptr)(register uint32_t addr, register uint32_t write_flag);

void* zxmem_ptr(register uint32_t addr, register uint32_t write_flag) {
	register void* armaddr;

	if(addr < ZXROMSIZE) {
		if(write_flag)
			armaddr = &fakeROM;
		else
			armaddr = (uint8_t*)ZXROM + addr;
	}
	else if(addr < (ZXROMSIZE + INTRAMSIZE))
		armaddr = mem + addr - ZXROMSIZE;
	else {
		if(write_flag)
			addr |= WRITE_OP;
		armaddr = extmem_armaddr(addr - (ZXROMSIZE + INTRAMSIZE));
	}

	return armaddr;
}

void* cpmmem_ptr(register uint32_t addr, register uint32_t write_flag) {
	register void* armaddr;

	if(addr > CPMMEMSIZE - 1)
		return &fakeROM;
	else if(addr > (CPMMEMSIZE - 1 - INTRAMSIZE))
		armaddr = mem + addr - (CPMMEMSIZE - INTRAMSIZE);
	else {
		if(write_flag)
			addr |= WRITE_OP;
		armaddr = extmem_armaddr(addr);
	}

	return armaddr;
}

void mem_Init(register memtype_t type) {
	extmem_Init();

	if(type == MEMTYPE_ZX) {
		mem_ptr = zxmem_ptr;
	}
	else {
		mem_ptr = cpmmem_ptr;
	}
}

