/*
 * cpm_memory.c
 *
 *  Created on: Oct 8, 2019
 *      Author: artur
 */

#include "extmem_cashed.h"
#include "cpm_memory.h"
#include "cpm_display.h"

#define INTMEMSIZE	12288
#define EXMEMSIZE	ILI9341_PWIDTH*(ILI9341_PHEIGHT-SCR_HEIGHT*FNT_HEIGHT)*2+(((ILI9341_PWIDTH-SCR_WIDTH*FNT_WIDTH-4)*SCR_HEIGHT*FNT_HEIGHT)/64)*128)
#define SBHEIGHT (((ILI9341_PWIDTH-SCR_WIDTH*FNT_WIDTH-4)*SCR_HEIGHT*FNT_HEIGHT)/64)*32/CPMD_START_LINE-1

static uint8_t mem[INTMEMSIZE];

uint8_t cpmmem_read(uint16_t addr) {
	if(addr<INTMEMSIZE)
		return mem[addr];
	else if(addr<(INTMEMSIZE+EXMEMSIZE)
		return extmem_read(addr-INTMEMSIZE);
	else
		return 0;
}

void cpmmem_write(uint16_t addr, uint8_t data) {
	if(addr<INTMEMSIZE)
		mem[addr] = data;
	else if(addr<(INTMEMSIZE+EXMEMSIZE)
		extmem_write(addr-INTMEMSIZE, data);
}

void cpmmem_Init() {

	extmem_Init(0, ILI9341_PWIDTH-1, 0, CPMD_START_LINE-1,
			0, ILI9341_PWIDTH-1, CPMD_END_LINE+1, ILI9341_PHEIGHT-1,
			0, CPMD_START_POS-3, CPMD_START_LINE, (((ILI9341_PWIDTH-SCR_WIDTH*FNT_WIDTH-4)*SCR_HEIGHT*FNT_HEIGHT)/64)*32/CPMD_START_LINE-1,
			CPMD_END_POS+3, ILI9341_PWIDTH-1, CPMD_START_LINE, (((ILI9341_PWIDTH-SCR_WIDTH*FNT_WIDTH-4)*SCR_HEIGHT*FNT_HEIGHT)/64)*32/CPMD_START_LINE-1
	);
}
