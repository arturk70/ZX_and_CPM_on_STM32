/*
 * ZX_display.c
 *
 *  Created on: Oct 14, 2019
 *      Author: artur
 */

#include "zx_display.h"
#include "stdlib.h"
#include "memory.h"

static uint32_t zxcolors[] = {
		0x00000000, 0x0000003f, 0x0000f800, 0x0000f83f, 0x000003c0, 0x000003ff, 0x0000ffc0, 0x0000ffff,
		0x003f0000, 0x003f003f, 0x003ff800, 0x003ff83f, 0x003f03c0, 0x003f03ff, 0x003fffc0, 0x003fffff,
		0xf8000000, 0xf800003f, 0xf800f800, 0xf800f83f, 0xf80003c0, 0xf80003ff, 0xf800ffc0, 0xf800ffff,
		0xf83f0000, 0xf83f003f, 0xf83ff800, 0xf83ff83f, 0xf83f03c0, 0xf83f03ff, 0xf83fffc0, 0xf83fffff,
		0x03c00000, 0x03c0003f, 0x03c0f800, 0x03c0f83f, 0x03c003c0, 0x03c003ff, 0x03c0ffc0, 0x03c0ffff,
		0x03ff0000, 0x03ff003f, 0x03fff800, 0x03fff83f, 0x03ff03c0, 0x03ff03ff, 0x03ffffc0, 0x03ffffff,
		0xffc00000, 0xffc0003f, 0xffc0f800, 0xffc0f83f, 0xffc003c0, 0xffc003ff, 0xffc0ffc0, 0xffc0ffff,
		0xffff0000, 0xffff003f, 0xfffff800, 0xfffff83f, 0xffff03c0, 0xffff03ff, 0xffffffc0, 0xffffffff
};

static uint16_t* linebuf;
static uint8_t frnum = 0;
uint8_t zxlnum = 0;
uint8_t zx_newline_flag;
uint16_t zx_border_color = 0;

void zxdisp_init() {
	linebuf = malloc((ZX_PIXELS+64)*2);
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
	register uint8_t attr;
	register uint32_t bgfgcolor;
	register uint8_t is_flash;
	register uint8_t pixline;
	register uint16_t *bufptr;
	register uint8_t bnum;

	attraddr = mem;
	lineaddr = attraddr+(((uint16_t)lnum & 0x00c0)<<5)+(((uint16_t)lnum & 0x0038)<<2)+(((uint16_t)lnum & 0x0007)<<8);
	attraddr += 0x1800+(lnum/8)*32;

	bufptr = linebuf+32+7;
	bnum = 0;
	do {
		attr = *(attraddr++);
		bgfgcolor = zxcolors[attr & 0x3f];
		if(!(attr & 0x40)) //not bright
			bgfgcolor &= 0xc618c618;
		is_flash = (((attr & 0x80) >> 4) & frnumi) >> 3;

		pixline = *(lineaddr++);
		for(register uint8_t pixnum=0; pixnum<8; pixnum++) {
			if(((pixline >> pixnum) & 0x01) ^ is_flash)
				*(bufptr-pixnum) = bgfgcolor;
			else
				*(bufptr-pixnum) = bgfgcolor >> 16;
		}
		bufptr += 8;
		linebuf[bnum] = linebuf[bnum+32+ZX_PIXELS] = zx_border_color;
	} while(++bnum < 32);

	ILI9341_sendDMABuf(ZXD_START_POS-BORDER_WIDTH, ZXD_START_LINE+lnum, ZXD_END_POS+BORDER_WIDTH, ZXD_START_LINE+lnum, linebuf+32-BORDER_WIDTH, (ZX_PIXELS+BORDER_WIDTH*2));

	zxlnum = lnum+1;
	frnum = frnumi;

	ZX_NEWLINE_RESET;
}


