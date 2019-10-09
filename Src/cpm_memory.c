/*
 * cpm_memory.c
 *
 *  Created on: Oct 8, 2019
 *      Author: artur
 */

#include "extmem_cashed.h"
#include "cpm_memory.h"
#include "cpm_display.h"

static uint8_t mem[8192];

static uint8_t cpmmem_read(uint16_t addr) {
	if(addr<8192)
		return mem[addr];
	else if(addr-8192<28800)
		return extmem_read(addr-8192);
	else
		return 0;
}

static void cpmmem_write(uint16_t addr, uint8_t data) {
	if(addr<8192)
		mem[addr] = data;
	else if(addr<(8192+ILI9341_PWIDTH*(ILI9341_PHEIGHT-CPMD_END_LINE+CPMD_START_LINE)*2+CPMD_START_POS*16*12*2))
		extmem_write(addr-8192, data);
}

void cpmmem_Init() {
	memread = cpmmem_read;
	memwrite = cpmmem_write;

	extmem_Init(0, ILI9341_PWIDTH-1, 0, CPMD_START_LINE-1,
			0, CPMD_START_POS-1, CPMD_START_LINE, CPMD_START_LINE+16*12-1,
			CPMD_END_POS+1, ILI9341_PWIDTH-1, CPMD_START_LINE, CPMD_START_LINE+16*12-1,
			CPMD_END_LINE+1, ILI9341_PWIDTH-1, CPMD_END_LINE+1, CPMD_START_LINE-1);
}
