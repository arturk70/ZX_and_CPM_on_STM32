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
		0x00000000, 0x00000018, 0x0000c000, 0x0000c018, 0x00000600, 0x00000618, 0x0000c600, 0x0000c618,
		0x00180000, 0x00180018, 0x0018c000, 0x0018c018, 0x00180600, 0x00180618, 0x0018c600, 0x0018c618,
		0xc0000000, 0xc0000018, 0xc000c000, 0xc000c018, 0xc0000600, 0xc0000618, 0xc000c600, 0xc000c618,
		0xc0180000, 0xc0180018, 0xc018c000, 0xc018c018, 0xc0180600, 0xc0180618, 0xc018c600, 0xc018c618,
		0x06000000, 0x06000018, 0x0600c000, 0x0600c018, 0x06000600, 0x06000618, 0x0600c600, 0x0600c618,
		0x06180000, 0x06180018, 0x0618c000, 0x0618c018, 0x06180600, 0x06180618, 0x0618c600, 0x0618c618,
		0xc6000000, 0xc6000018, 0xc600c000, 0xc600c018, 0xc6000600, 0xc6000618, 0xc600c600, 0xc600c618,
		0xc6180000, 0xc6180018, 0xc618c000, 0xc618c018, 0xc6180600, 0xc6180618, 0xc618c600, 0xc618c618,
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


