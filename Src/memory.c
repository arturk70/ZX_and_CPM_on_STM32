/*
 * memory.c
 *
 *  Created on: Oct 8, 2019
 *      Author: artur
 */

#include <stdlib.h>
#include "memory.h"
#include "ZXROM.h"

#define ZXROMSIZE	0x4000
#define INTRAMSIZE	0x3000

uint8_t mem[INTRAMSIZE];

uint8_t (*mem_read)(register uint16_t addr);
void (*mem_write)(register uint16_t addr, register uint8_t data);

uint8_t zxmem_read(register uint16_t addr) {
	if(addr < ZXROMSIZE)
		return ZXROM[addr];
	else if(addr < (ZXROMSIZE + INTRAMSIZE))
		return mem[addr - ZXROMSIZE];
	else
		return extmem_read(addr - (ZXROMSIZE + INTRAMSIZE));
}

void zxmem_write(register uint16_t addr, register uint8_t data) {
	if(addr < ZXROMSIZE)
		return;
	else if(addr < (ZXROMSIZE + INTRAMSIZE))
		mem[addr - ZXROMSIZE] = data;
	else
		extmem_write(addr - (ZXROMSIZE + INTRAMSIZE), data);
}

uint8_t cpmmem_read(register uint16_t addr) {
	if(addr > 0xbfff)
		return 0x00;
	else if(addr > (0xbfff - INTRAMSIZE))
		return mem[addr - (0xbfff - INTRAMSIZE + 1)];
	else
		return extmem_read(addr);
}

void cpmmem_write(register uint16_t addr, register uint8_t data) {
	if(addr > 0xbfff)
			return;
	else if(addr > (0xbfff - INTRAMSIZE))
		mem[addr - (0xbfff - INTRAMSIZE + 1)] = data;
	else
		extmem_write(addr, data);
}

void mem_Init(uint8_t type) {
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

