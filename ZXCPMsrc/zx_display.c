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
		0x00000000, 0x00000010, 0x00008000, 0x00008010, 0x00000400, 0x00000410, 0x00008400, 0x00008410,
		0x00100000, 0x00100010, 0x00108000, 0x00108010, 0x00100400, 0x00100410, 0x00108400, 0x00108410,
		0x80000000, 0x80000010, 0x80008000, 0x80008010, 0x80000400, 0x80000410, 0x80008400, 0x80008410,
		0x80100000, 0x80100010, 0x80108000, 0x80108010, 0x80100400, 0x80100410, 0x80108400, 0x80108410,
		0x04000000, 0x04000010, 0x04008000, 0x04008010, 0x04000400, 0x04000410, 0x04008400, 0x04008410,
		0x04100000, 0x04100010, 0x04108000, 0x04108010, 0x04100400, 0x04100410, 0x04108400, 0x04108410,
		0x84000000, 0x84000010, 0x84008000, 0x84008010, 0x84000400, 0x84000410, 0x84008400, 0x84008410,
		0x84100000, 0x84100010, 0x84108000, 0x84108010, 0x84100400, 0x84100410, 0x84108400, 0x84108410,
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

void zxdisp_init() {
//	linebuf = malloc((ZX_PIXELS+64)*2);
//	ILI9341_fillArea(ZXD_START_POS, ZXD_START_LINE, ZXD_END_POS, ZXD_END_LINE, BLACK);
}
void zxdisp_deinit() {
//	free(linebuf);
}

void zxdisp_drawnextline() {
	register uint32_t lnum = zxlnum;
	register uint32_t frnumi = frnum;
	if(lnum >= ZX_LINES) {
		lnum = 0;
		frnumi++;
		if(frnumi > 0x0f) frnumi = 0;//16 fps
	}

	register uint8_t *attraddr;
	register uint8_t *lineaddr;
	register uint32_t attr;
	register uint32_t bgcolor, fgcolor;
	register uint32_t bordercolor;
	register uint32_t is_flash;
	register uint32_t pixline;
	register uint16_t *bufptr, *borderptr;

	attraddr = mem;
	lineaddr = attraddr+(((uint16_t)lnum & 0x00c0)<<5)+(((uint16_t)lnum & 0x0038)<<2)+(((uint16_t)lnum & 0x0007)<<8);
	attraddr += 0x1800+(lnum/8)*32;

	bordercolor = zxcolors[zx_border_color];

	borderptr = linebuf;
	bufptr = borderptr+BORDER_WIDTH+7;

	do {
		attr = *(attraddr++);

		is_flash = ((attr >> 7) & (frnumi >> 3));//swap bg/fg colors every 16 frames
		if(is_flash) {
			bgcolor = zxcolors[attr & 0x7f];
			fgcolor = bgcolor >> 16;
		}
		else {
			fgcolor = zxcolors[attr & 0x7f];
			bgcolor = fgcolor >> 16;
		}

		pixline = *(lineaddr++) | 0x0100;
		do {
			if(pixline & 0x01)
				*(bufptr) = fgcolor;
			else
				*(bufptr) = bgcolor;
			pixline >>= 1;
			bufptr--;
		} while(pixline != 1);
		bufptr += 16;
		if(borderptr == linebuf+BORDER_WIDTH)
			borderptr += ZX_PIXELS;
		*(borderptr++) = bordercolor;
	} while(borderptr < linebuf + ZX_PIXELS+BORDER_WIDTH*2 );

	ILI9341_sendDMABuf(ZXD_START_POS-BORDER_WIDTH, ZXD_START_LINE+lnum, ZXD_END_POS+BORDER_WIDTH, ZXD_START_LINE+lnum, linebuf, (ZX_PIXELS+BORDER_WIDTH*2));

	zxlnum = ++lnum;
	frnum = frnumi;

	ZX_NEWLINE_RESET;
}


