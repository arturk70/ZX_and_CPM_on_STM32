/*
 * ZX_display.c
 *
 *  Created on: Oct 14, 2019
 *      Author: artur
 */

#include "zx_display.h"
#include "stdlib.h"
#include "memory.h"

static uint8_t *ZXvideomem;
static uint16_t *linebuf;
static uint8_t flash;

void ZXdisp_clear() {
	ILI9341_fillArea(ZXD_START_POS, ZXD_START_LINE, ZXD_END_POS, ZXD_END_LINE, BLACK);
}


void ZXdisp_Init() {
	ZXvideomem = get_ZX_videomem();
	ILI9341_Init();
	ZXdisp_clear();
	linebuf = malloc(ZX_PIXELS*2);
	flash = 0;
}
void ZXdisp_deInit() {
	free(linebuf);
}

void ZXdisp_drawline(uint8_t lnum) {
	uint8_t *lineaddr = ZXvideomem+((lnum & 0xc0) | ((lnum & 0x07)<<3) | ((lnum & 0x38)>>3))*32;
	uint8_t *attraddr = ZXvideomem+0x1800+(lnum/8)*32;

	uint8_t attr;
	uint16_t fgcolor;
	uint16_t bgcolor;
	uint16_t tmp;

	for(uint8_t bnum=0; bnum<32; bnum++) {
		attr = *(attraddr+bnum);
		fgcolor=((attr & 0x02) << 13) | ((attr & 0x04) << 7) | ((attr & 0x01) << 3);
		bgcolor=((attr & 0x10) << 10) | ((attr & 0x20) << 4) | ((attr & 0x08));
		if(attr & 0x40) {
			fgcolor |= fgcolor << 1;
			bgcolor |= bgcolor << 1;
		}
		if((attr & 0x80) && flash) {
			tmp = fgcolor;
			fgcolor = bgcolor;
			bgcolor = tmp;
		}

		for(uint8_t pnum=0; pnum<8; pnum++) {
			if((*(lineaddr+bnum)) & (0x01 << (7-pnum)))
				linebuf[bnum*8+pnum] = fgcolor;
			else
				linebuf[bnum*8+pnum] = bgcolor;
		}
	}

	ILI9341_sendBuf(ZXD_START_POS, ZXD_START_LINE+lnum, ZXD_END_POS, ZXD_START_LINE+lnum, linebuf);
}


