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

uint8_t (*mem_read)(register uint32_t addr);
void (*mem_write)(register uint32_t addr, register uint32_t data);

uint8_t zxmem_read(register uint32_t addr) {
	if(addr < ZXROMSIZE)
		return ZXROM[addr];
	else if(addr < (ZXROMSIZE + INTRAMSIZE))
		return mem[addr - ZXROMSIZE];
	else
		return extmem_rw(EXTM_READ, addr - (ZXROMSIZE + INTRAMSIZE), 0);
}

void zxmem_write(register uint32_t addr, register uint32_t data) {
	if(addr < ZXROMSIZE)
		return;
	else if(addr < (ZXROMSIZE + INTRAMSIZE))
		mem[addr - ZXROMSIZE] = data;
	else
		extmem_rw(EXTM_WRITE, addr - (ZXROMSIZE + INTRAMSIZE), data);
}

uint8_t cpmmem_read(register uint32_t addr) {
	if(addr > CPMMEMSIZE - 1)
		return 0x00;
	else if(addr > (CPMMEMSIZE - 1 - INTRAMSIZE))
		return mem[addr - (CPMMEMSIZE - INTRAMSIZE)];
	else
		return extmem_rw(EXTM_READ, addr, 0);
}

void cpmmem_write(register uint32_t addr, register uint32_t data) {
	if(addr > CPMMEMSIZE - 1)
			return;
	else if(addr > (CPMMEMSIZE - 1 - INTRAMSIZE))
		mem[addr - (CPMMEMSIZE - INTRAMSIZE)] = data;
	else
		extmem_rw(EXTM_WRITE, addr, data);
}

void mem_Init(register memtype_t type) {
	extmem_Init();

	if(type == MEMTYPE_ZX) {
		mem_read = zxmem_read;
		mem_write = zxmem_write;
	}
	else {
		mem_read = cpmmem_read;
		mem_write = cpmmem_write;
	}
}

