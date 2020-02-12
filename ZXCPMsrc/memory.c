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
void (*mem_write)(register uint32_t addr, register uint8_t data);

uint8_t zxmem_read(register uint32_t addr) {
	register uint8_t* armaddr;

	if(addr < ZXROMSIZE)
		armaddr = (uint8_t*)ZXROM + addr;
	else if(addr < (ZXROMSIZE + INTRAMSIZE))
		armaddr = mem + addr - ZXROMSIZE;
	else
		armaddr = extmem_armaddr(addr - (ZXROMSIZE + INTRAMSIZE));

	return *(armaddr);
}

void zxmem_write(register uint32_t addr, register uint8_t data) {
	register uint8_t* armaddr;

	if(addr < ZXROMSIZE)
		return;
	else if(addr < (ZXROMSIZE + INTRAMSIZE))
		armaddr = mem + addr - ZXROMSIZE;
	else
		armaddr = extmem_armaddr((addr - (ZXROMSIZE + INTRAMSIZE)) | WRITE_OP);

	*(armaddr) = data;
}

uint8_t cpmmem_read(register uint32_t addr) {
	register uint8_t* armaddr;

	if(addr > CPMMEMSIZE - 1)
		return 0x00;
	else if(addr > (CPMMEMSIZE - 1 - INTRAMSIZE))
		armaddr = mem + addr - (CPMMEMSIZE - INTRAMSIZE);
	else
		armaddr = extmem_armaddr(addr);

	return *(armaddr);
}

void cpmmem_write(register uint32_t addr, register uint8_t data) {
	register uint8_t* armaddr;

	if(addr > CPMMEMSIZE - 1)
		return;
	else if(addr > (CPMMEMSIZE - 1 - INTRAMSIZE))
		armaddr = mem + addr - (CPMMEMSIZE - INTRAMSIZE);
	else
		armaddr = extmem_armaddr(addr | WRITE_OP);

	*(armaddr) = data;
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

