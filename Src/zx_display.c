/*
 * ZX_display.c
 *
 *  Created on: Oct 14, 2019
 *      Author: artur
 */

#include "zx_display.h"
#include "stdlib.h"
#include "memory.h"

static uint16_t* linebuf;
//static uint8_t *ZXvideomem;
static uint8_t frnum = 0;
uint8_t zxlnum = 0;
uint8_t zx_newline_flag;
uint16_t zx_border_color = 0;

void zxdisp_init() {
	linebuf = malloc(304*2);
//	ZXvideomem = get_ZX_videomem();
	ILI9341_fillArea(ZXD_START_POS, ZXD_START_LINE, ZXD_END_POS, ZXD_END_LINE, BLACK);
}
void zxdisp_deinit() {
	free(linebuf);
}

void zxdisp_drawnextline() {
	register uint8_t lnum = zxlnum;
	register uint8_t frnumi = frnum;
	if(lnum >= ZX_LINES) {
		lnum = 0;
		frnumi++;
		if(frnumi > 0x0f) frnumi = 0;
	}

	register uint8_t *attraddr;
	register uint8_t *lineaddr;
	register uint32_t attr;
	register uint32_t fgbgcolor;
	register uint8_t is_flash_pos;
	register uint8_t pixline;
	register uint16_t *bufptr;

	attraddr = mem;
	lineaddr = attraddr+(((uint16_t)lnum & 0x00c0)<<5)+(((uint16_t)lnum & 0x0038)<<2)+(((uint16_t)lnum & 0x0007)<<8);
	attraddr += 0x1800+(lnum/8)*32;

	bufptr = linebuf+BORDER_WIDTH+7;
	do {
		attr = *(attraddr++);
		fgbgcolor  = (((attr << 26) & 0x08000000) | ((attr << 20) & 0x00400000) | ((attr << 16) & 0x00010000) |
						    ((attr << 7) & 0x0800) | ((attr << 1) & 0x0040) | ((attr >> 3) & 0x0001)) * ((attr & 0x40) ? 0x1f : 0x18);
		is_flash_pos = (((attr & 0x80) >> 4) & frnumi) >> 3;

		pixline = *(lineaddr++);
		for(register uint8_t pixnum=0; pixnum<8; pixnum++) {
			if(((pixline >> pixnum) & 0x01) ^ is_flash_pos)
				*(bufptr-pixnum) = fgbgcolor >> 16;
			else
				*(bufptr-pixnum) = fgbgcolor;
		}
		bufptr += 8;
	} while((attraddr-mem) & 0x1f);
	for(register uint8_t i = 0; i< BORDER_WIDTH; i++)
		linebuf[i] = linebuf[BORDER_WIDTH+ZX_PIXELS+i] = zx_border_color;

	ILI9341_sendDMABuf(ZXD_START_POS-BORDER_WIDTH, ZXD_START_LINE+lnum, ZXD_END_POS+BORDER_WIDTH, ZXD_START_LINE+lnum, linebuf, (ZX_PIXELS+BORDER_WIDTH*2));

	zxlnum = lnum+1;
	frnum = frnumi;

	ZX_NEWLINE_RESET;
}


