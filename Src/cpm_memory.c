/*
 * cpm_memory.c
 *
 *  Created on: Oct 8, 2019
 *      Author: artur
 */

#include "cpm_memory.h"

#define EXTMEMSIZE	(ILI9341_PWIDTH*24*2+(ILI9341_PHEIGHT-24*2)*8*2)*2
#define INTMEMSIZE	(49152-EXTMEMSIZE)

static uint8_t mem[INTMEMSIZE];

uint8_t cpmmem_read(uint16_t addr) {
	if(addr<INTMEMSIZE)
		return mem[addr];
	else if(addr<(INTMEMSIZE+EXTMEMSIZE))
		return extmem_read(addr-INTMEMSIZE);
	else
		return 0;
}

void cpmmem_write(uint16_t addr, uint8_t data) {
	if(addr<INTMEMSIZE)
		mem[addr] = data;
	else if(addr<(INTMEMSIZE+EXTMEMSIZE))
		extmem_write(addr-INTMEMSIZE, data);
}

void cpmmem_Init() {
	extmem_Init(0, ILI9341_PWIDTH-1, 0, 23,
			0, ILI9341_PWIDTH-1, 216, ILI9341_PHEIGHT-1,
			0, 7, 24, 215,
			312, ILI9341_PWIDTH-1, 24, 215
	);
}

uint16_t cpmmem_test() {
	uint8_t tmp;
	uint16_t errcount = 0;
	for(uint16_t addr=0; addr<0xbfff; addr++) {
	  cpmmem_write(addr, (uint8_t)(addr&0xff));
	  tmp = cpmmem_read(addr);
	  if(tmp != (uint8_t)(addr&0xff)) errcount++;
	}

	return errcount;
}
