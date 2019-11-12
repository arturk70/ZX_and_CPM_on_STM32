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

#define ZXROMSIZE	0x4000
#define INTRAMSIZE	0x2000
#define INTRAMSIZE2	0x0400

static uint8_t mem_type;
static uint8_t mem[INTRAMSIZE];
static uint8_t mem2[INTRAMSIZE2];

uint8_t *get_ZX_videomem() { if(mem_type == MEMTYPE_ZX) return mem; else return 0x00000000; }

uint8_t mem_read(uint16_t addr) {
	if(mem_type == MEMTYPE_ZX) {
		if(addr < ZXROMSIZE)
			return ZXROM[addr];
		else if(addr < (ZXROMSIZE + INTRAMSIZE))
			return mem[addr - ZXROMSIZE];
		else if(addr > (0xffff - INTRAMSIZE2))
					return mem2[addr - (0xffff - INTRAMSIZE2 + 1)];
		else
			return extmem_read(addr);
	}
	else {
		if(addr < INTRAMSIZE2)
			return mem2[addr];
		else if(addr > (0xffff - INTRAMSIZE))
			return mem[addr - (0xffff - INTRAMSIZE + 1)];
		else
			return extmem_read(addr);
	}
}

void mem_write(uint16_t addr, uint8_t data) {
	if(mem_type == MEMTYPE_ZX) {
			if(addr < ZXROMSIZE)
				return;
			else if(addr < (ZXROMSIZE + INTRAMSIZE))
				mem[addr - ZXROMSIZE] = data;
			else if(addr > (0xffff - INTRAMSIZE2))
				mem2[addr - (0xffff - INTRAMSIZE2 + 1)] = data;
			else
				extmem_write(addr, data);
		}
		else {
			if(addr < INTRAMSIZE2)
				mem2[addr] = data;
			else if(addr > (0xffff - INTRAMSIZE))
				mem[addr - (0xffff - INTRAMSIZE + 1)] = data;
			else
				extmem_write(addr, data);
		}
}

void mem_Init(uint8_t type) {
	mem_type = type;

	extmem_Init();
}

void mem_deInit() {
	extmem_deInit();
}

//void mem_clear() {
//	for(uint16_t i=0; i< INTRAMSIZE;i++) mem[i] = 0;
//	ILI9341_fillArea(0, 0, ILI9341_PWIDTH-1, 23, BLACK);
//	ILI9341_fillArea(0, 216, ILI9341_PWIDTH-1, ILI9341_PHEIGHT-1, BLACK);
//	ILI9341_fillArea(0, 24, 7, 215, BLACK);
//	ILI9341_fillArea(312, 24, ILI9341_PWIDTH-1, 215, BLACK);
//}

uint16_t mem_test() {
	register uint16_t errcount = 0;
	register uint16_t straddr, endaddr = 0xffff;
	if(mem_type == MEMTYPE_ZX) {
		straddr = ZXROMSIZE;
	} else {
		straddr = 0;
	}

	uint8_t tst[50];
	register uint8_t i;
	for(i=0;i<50;i++)
		tst[i] = (uint8_t)rand();

	i = 0;
	for(register uint32_t addr=straddr; addr<=endaddr; addr++) {
		mem_write(addr, tst[i++]);
		if(i == 50) i = 0;
	}

	i = 0;
	for(register uint32_t addr=straddr; addr<=endaddr; addr++) {
		if(tst[i++] != mem_read(addr)) errcount++;
		if(i == 50) i = 0;
	}
//	mem_clear();

	return errcount;
}
uint16_t mem_rnd_test() {
	register uint8_t tmp;
	register uint16_t errcount = 0;
	register uint16_t straddr, endaddr = 0xffff;
	if(mem_type == MEMTYPE_ZX) {
		straddr = ZXROMSIZE;
	} else {
		straddr = 0;
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
