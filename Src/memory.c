/*
 * memory.c
 *
 *  Created on: Oct 8, 2019
 *      Author: artur
 */

#include <stdlib.h>
#include "memory.h"
#include "ZXROM.h"
#include "CPMROM.h"

#define EXTRAMSIZE	(ILI9341_PWIDTH*24*2+(ILI9341_PHEIGHT-24*2)*8*2)*2
#define INTRAMSIZE	(49152-EXTRAMSIZE)

static uint16_t RAM_ROM_splitaddr;
static uint8_t mem_type;
static uint8_t mem[INTRAMSIZE];

uint8_t *get_ZX_videomem() { if(mem_type == MEMTYPE_ZX) return mem; else return 0x00000000; }

uint8_t mem_read(uint16_t addr) {
	if(mem_type == MEMTYPE_ZX) {
		if(addr < RAM_ROM_splitaddr)
			return ZXROM[addr];
		else if(addr < (RAM_ROM_splitaddr+INTRAMSIZE))
			return mem[addr - RAM_ROM_splitaddr];
		else
			return extmem_read(addr - RAM_ROM_splitaddr - INTRAMSIZE);
	}
	else {
		if(addr >= RAM_ROM_splitaddr)
			return CPMROM[addr-RAM_ROM_splitaddr];
		else if(addr < INTRAMSIZE)
			return mem[addr];
		else
			return extmem_read(addr-INTRAMSIZE);
	}
}

void mem_write(uint16_t addr, uint8_t data) {
	if(mem_type == MEMTYPE_ZX) {
		if(addr < RAM_ROM_splitaddr)
			return;
		else if(addr < (RAM_ROM_splitaddr+INTRAMSIZE))
			mem[addr - RAM_ROM_splitaddr] = data;
		else
			extmem_write(addr - RAM_ROM_splitaddr - INTRAMSIZE, data);
	}
	else {
		if(addr >= RAM_ROM_splitaddr)
			return;
		else if(addr < INTRAMSIZE)
			mem[addr] = data;
		else
			extmem_write(addr-INTRAMSIZE, data);
	}
}

void mem_Init(uint8_t type) {
	mem_type = type;
	if(mem_type == MEMTYPE_ZX)
		RAM_ROM_splitaddr = 0x4000;
	else
		RAM_ROM_splitaddr = 0xC000;

	extmem_Init();
}

//void mem_clear() {
//	for(uint16_t i=0; i< INTRAMSIZE;i++) mem[i] = 0;
//	ILI9341_fillArea(0, 0, ILI9341_PWIDTH-1, 23, BLACK);
//	ILI9341_fillArea(0, 216, ILI9341_PWIDTH-1, ILI9341_PHEIGHT-1, BLACK);
//	ILI9341_fillArea(0, 24, 7, 215, BLACK);
//	ILI9341_fillArea(312, 24, ILI9341_PWIDTH-1, 215, BLACK);
//}

uint16_t mem_test() {
	register uint8_t tmp;
	register uint16_t errcount = 0;
	register uint16_t straddr, endaddr;
	if(mem_type == MEMTYPE_ZX) {
		straddr = RAM_ROM_splitaddr;
		endaddr = 0xffff;
	} else {
		straddr = 0;
		endaddr = RAM_ROM_splitaddr - 1;
	}

#ifndef __SIMULATION
	srand(SysTick->VAL);
#endif
	for(register uint32_t addr=straddr; addr<=endaddr; addr++) {
		tmp = (uint8_t)addr;
		mem_write(addr, tmp);
	}
	for(register uint32_t addr=straddr; addr<=endaddr; addr++) {
		tmp = (uint8_t)addr;
		if(tmp != mem_read(addr)) errcount++;
	}
//	mem_clear();

	return errcount;
}
uint16_t mem_rnd_test() {
	register uint8_t tmp;
	register uint16_t errcount = 0;
	register uint16_t straddr, endaddr;
	if(mem_type == MEMTYPE_ZX) {
		straddr = RAM_ROM_splitaddr;
		endaddr = 0xffff;
	} else {
		straddr = 0;
		endaddr = RAM_ROM_splitaddr - 1;
	}

#ifndef __SIMULATION
	srand(SysTick->VAL);
#endif
	register uint16_t addr;
	for(register uint16_t i=0; i<0xffff; i++) {
		tmp = (uint8_t)rand();
		addr = (uint16_t)rand();
		if((addr >= straddr) && (addr <= endaddr)) {
			mem_write(addr, tmp);
			if(tmp != mem_read(addr)) errcount++;
		}
	}
//	mem_clear();

	return errcount;
}
