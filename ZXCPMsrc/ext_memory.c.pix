/*
 * ext_memory.c
 *
 *  Created on: Oct 8, 2019
 *      Author: artur
 */

#include "ext_memory.h"

static uint16_t bl[4], br[4], bt[4], bb[4]; // 0-top bar, 1 - left bar, 2 - right bar, 3 - bottom bar
static uint16_t bl_straddr[4], bl_endaddr[4];

uint32_t mem_time = 0;

uint8_t extmem_read(uint16_t addr) {
	register uint16_t x, y, pixnum;
	register uint8_t blnum = 0;

	while((addr < bl_straddr[blnum]) || (addr > bl_endaddr[blnum])) blnum++;
	pixnum = (addr - bl_straddr[blnum]) / 2;
	x = bl[blnum] + pixnum % (br[blnum] - bl[blnum] + 1);
	y = bt[blnum] + pixnum / (br[blnum] - bl[blnum] + 1);

	uint16_t pix;

	ILI9341_readPix(x, y, &pix);

	if(addr & 0x0001)
		return pix & 0x00ff;
	else
		return pix >> 8;
}

void extmem_write(uint16_t addr, uint8_t data) {
	register uint16_t x, y, pixnum;
	register uint8_t blnum = 0;

	while((addr < bl_straddr[blnum]) || (addr > bl_endaddr[blnum])) blnum++;
	pixnum = (addr - bl_straddr[blnum]) / 2;
	x = bl[blnum] + pixnum % (br[blnum] - bl[blnum] + 1);
	y = bt[blnum] + pixnum / (br[blnum] - bl[blnum] + 1);

	uint16_t pix;

	ILI9341_readPix(x, y, &pix);

	if(addr & 0x0001)
		pix = (pix & 0xff00) | data;
	else
		pix = (pix & 0x00ff) | ((uint16_t)data << 8);

	ILI9341_writePix(x, y, pix);
}

void extmem_Init() {

	bl[0] = 0;   br[0] = ILI9341_PWIDTH-1; bt[0] = 0;   bb[0] = 23;
	bl[1] = 0;   br[1] = ILI9341_PWIDTH-1; bt[1] = 216; bb[1] = ILI9341_PHEIGHT-1;
	bl[2] = 0;   br[2] = 7;                bt[2] = 24;  bb[2] = 215;
	bl[3] = 312; br[3] = ILI9341_PWIDTH-1; bt[3] = 24;  bb[3] = 215;
	bl_straddr[0] = 0;
	bl_endaddr[0]=(br[0]-bl[0]+1)*(bb[0]-bt[0]+1)*2-1;
	bl_straddr[1] = bl_endaddr[0]+1;
	bl_endaddr[1]=bl_straddr[1]+(br[1]-bl[1]+1)*(bb[1]-bt[1]+1)*2-1;
	bl_straddr[2] = bl_endaddr[1]+1;
	bl_endaddr[2]=bl_straddr[2]+(br[2]-bl[2]+1)*(bb[2]-bt[2]+1)*2-1;
	bl_straddr[3] = bl_endaddr[2]+1;
	bl_endaddr[3]=bl_straddr[3]+(br[3]-bl[3]+1)*(bb[3]-bt[3]+1)*2-1;
}

//void extmem_deInit() {
//}
