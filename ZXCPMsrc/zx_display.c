/*
 * ZX_display.c
 *
 *  Created on: Oct 14, 2019
 *      Author: artur
 */

#include "zx_display.h"
#include "stdlib.h"
#include "memory.h"

const uint32_t zxcolors[] = {
		//normal colors
		0x00000000, 0x00000014, 0x0000a000, 0x0000a014, 0x00000500, 0x00000514, 0x0000a500, 0x0000a514,
		0x00140000, 0x00140014, 0x0014a000, 0x0014a014, 0x00140500, 0x00140514, 0x0014a500, 0x0014a514,
		0xa0000000, 0xa0000014, 0xa000a000, 0xa000a014, 0xa0000500, 0xa0000514, 0xa000a500, 0xa000a514,
		0xa0140000, 0xa0140014, 0xa014a000, 0xa014a014, 0xa0140500, 0xa0140514, 0xa014a500, 0xa014a514,
		0x05000000, 0x05000014, 0x0500a000, 0x0500a014, 0x05000500, 0x05000514, 0x0500a500, 0x0500a514,
		0x05140000, 0x05140014, 0x0514a000, 0x0514a014, 0x05140500, 0x05140514, 0x0514a500, 0x0514a514,
		0xa5000000, 0xa5000014, 0xa500a000, 0xa500a014, 0xa5000500, 0xa5000514, 0xa500a500, 0xa500a514,
		0xa5140000, 0xa5140014, 0xa514a000, 0xa514a014, 0xa5140500, 0xa5140514, 0xa514a500, 0xa514a514,
		//bright colors
		0x00000000, 0x0000003f, 0x0000f800, 0x0000f81f, 0x000007e0, 0x000007ff, 0x0000ffe0, 0x0000ffff,
		0x003f0000, 0x003f003f, 0x003ff800, 0x003ff81f, 0x003f07e0, 0x003f07ff, 0x003fffe0, 0x003fffff,
		0xf8000000, 0xf800003f, 0xf800f800, 0xf800f81f, 0xf80007e0, 0xf80007ff, 0xf800ffe0, 0xf800ffff,
		0xf81f0000, 0xf81f003f, 0xf81ff800, 0xf81ff81f, 0xf81f07e0, 0xf81f07ff, 0xf81fffe0, 0xf81fffff,
		0x07e00000, 0x07e0003f, 0x07e0f800, 0x07e0f81f, 0x07e007e0, 0x07e007ff, 0x07e0ffe0, 0x07e0ffff,
		0x07ff0000, 0x07ff003f, 0x07fff800, 0x07fff81f, 0x07ff07e0, 0x07ff07ff, 0x07ffffe0, 0x07ffffff,
		0xffe00000, 0xffe0003f, 0xffe0f800, 0xffe0f81f, 0xffe007e0, 0xffe007ff, 0xffe0ffe0, 0xffe0ffff,
		0xffff0000, 0xffff003f, 0xfffff800, 0xfffff81f, 0xffff07e0, 0xffff07ff, 0xffffffe0, 0xffffffff
};

static uint16_t linebuf[ZX_PIXELS+32];
static uint32_t frnum = 0;
uint32_t zxlnum = 0;
uint32_t zx_newline_flag;
uint8_t zx_border_color = 0;

void zxdisp_drawnextline() {
	register uint32_t lnum = zxlnum;
	register uint32_t frnumi = frnum;

	register uint8_t *attraddr;
	register uint8_t *lineaddr;
	register uint32_t attr;
	register uint32_t bgcolor, fgcolor;
	register uint32_t bordercolor;
	register uint32_t pixline;
	register uint16_t *bufptr, *borderptr;

	lineaddr = attraddr = mem;
	lineaddr += ((lnum & 0x00c0)<<5) | ((lnum & 0x0038)<<2) | ((lnum & 0x0007)<<8);
	attraddr += 0x1800+(lnum/8)*32;

	bordercolor = zxcolors[zx_border_color];

	borderptr = linebuf;
	bufptr = borderptr+BORDER_WIDTH;

	do {
		attr = *(attraddr++);

		if((attr >> 7) & (frnumi >> 3)) {//swap bg/fg colors every 16 frames for flash effect
			bgcolor = zxcolors[attr & 0x7f];
			fgcolor = bgcolor >> 16;
		}
		else {
			fgcolor = zxcolors[attr & 0x7f];
			bgcolor = fgcolor >> 16;
		}

		pixline = *(lineaddr++) | 0x0100;
		do {
			if(pixline & 0x80)
				*(bufptr) = fgcolor;
			else
				*(bufptr) = bgcolor;
			pixline <<= 1;
			bufptr++;
		} while(pixline < 0x10000);

		if(borderptr == linebuf+BORDER_WIDTH)
			borderptr += ZX_PIXELS;
		*(borderptr++) = bordercolor;
	} while(borderptr < linebuf + ZX_PIXELS+BORDER_WIDTH*2 );

	pixline = lnum+ZXD_START_LINE;
	ILI9341_sendDMABuf(ZXD_START_POS-BORDER_WIDTH, pixline, ZXD_END_POS+BORDER_WIDTH, pixline, linebuf, (ZX_PIXELS+BORDER_WIDTH*2));

	if(++lnum >= ZX_LINES) {
		zxlnum = 0;
		frnum = frnumi+1;
	}
	else
		zxlnum = lnum;

	ZX_NEWLINE_RESET;
}


