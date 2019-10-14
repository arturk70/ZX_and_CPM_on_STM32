/*
 * memory.c
 *
 *  Created on: Oct 8, 2019
 *      Author: artur
 */

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
		else
			addr -= RAM_ROM_splitaddr;
	}
	else {
		if(addr >= RAM_ROM_splitaddr)
			return CPMROM[addr-RAM_ROM_splitaddr];
	}

	if(addr<INTRAMSIZE)
		return mem[addr];
	else if(addr<(INTRAMSIZE+EXTRAMSIZE))
		return extmem_read(addr-INTRAMSIZE);
	else
		return 0;
}

void mem_write(uint16_t addr, uint8_t data) {
	if(mem_type == MEMTYPE_ZX) {
		if(addr < RAM_ROM_splitaddr)
			return;
		else
			addr -= RAM_ROM_splitaddr;
	}
	else {
		if(addr >= RAM_ROM_splitaddr)
			return;
	}

	if(addr<INTRAMSIZE)
		mem[addr] = data;
	else if(addr<(INTRAMSIZE+EXTRAMSIZE))
		extmem_write(addr-INTRAMSIZE, data);
}

void mem_Init(uint8_t type) {
	mem_type = type;
	if(mem_type == MEMTYPE_ZX)
		RAM_ROM_splitaddr = 0x4000;
	else
		RAM_ROM_splitaddr = 0xC000;

	extmem_Init(0, ILI9341_PWIDTH-1, 0, 23,
			0, ILI9341_PWIDTH-1, 216, ILI9341_PHEIGHT-1,
			0, 7, 24, 215,
			312, ILI9341_PWIDTH-1, 24, 215
	);
}

void mem_clear() {
	ILI9341_fillArea(0, 0, ILI9341_PWIDTH-1, 23, BLACK);
	ILI9341_fillArea(0, 216, ILI9341_PWIDTH-1, ILI9341_PHEIGHT-1, BLACK);
	ILI9341_fillArea(0, 24, 7, 215, BLACK);
	ILI9341_fillArea(312, 24, ILI9341_PWIDTH-1, 215, BLACK);
}

uint16_t mem_test() {
	uint8_t tmp;
	uint16_t errcount = 0;
	uint16_t straddr = 0;
	if(mem_type == MEMTYPE_ZX)
		straddr += RAM_ROM_splitaddr;

	for(uint16_t addr=straddr; addr<straddr+0xbfff; addr++) {
	  mem_write(addr, (uint8_t)(addr&0xff));
	  tmp = mem_read(addr);
	  if(tmp != (uint8_t)(addr&0xff)) errcount++;
	}
	mem_clear();

	return errcount;
}
