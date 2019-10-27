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
static uint8_t lnum;
static uint8_t frnum;
uint8_t zx_newline_flag;
uint16_t zx_border_color;

void ZXdisp_clear() {
	ILI9341_fillArea(ZXD_START_POS, ZXD_START_LINE, ZXD_END_POS, ZXD_END_LINE, BLACK);
}


void ZXdisp_Init() {
	ZXvideomem = get_ZX_videomem();
	ILI9341_Init();
	ZXdisp_clear();
	linebuf = malloc((ZX_PIXELS+BORDER_WIDTH*2)*2);
	lnum = 0;
	frnum = 0;
	zx_border_color = 0;

#ifndef __SIMULATION
	LL_TIM_EnableUpdateEvent(TIM3);
	LL_TIM_EnableCounter(TIM3);
	LL_TIM_EnableIT_UPDATE(TIM3);
#endif
}
void ZXdisp_deInit() {
#ifndef __SIMULATION
	LL_TIM_DisableIT_UPDATE(TIM3);
	LL_TIM_DisableCounter(TIM3);
#endif
	if(linebuf != NULL) free(linebuf);
}

uint8_t ZXdisp_drawnextline() {
	register uint8_t *attraddr = ZXvideomem+0x1800+(lnum/8)*32;
	register uint8_t *lineaddr = ZXvideomem+(((uint16_t)lnum & 0x00c0)<<5)+(((uint16_t)lnum & 0x0038)<<2)+(((uint16_t)lnum & 0x0007)<<8);
	register uint32_t attr;
	register uint32_t fgbgcolor;
	register uint16_t bufshft;

	for(register uint8_t colnum=0; colnum<32; colnum++) {
		attr = *(attraddr+colnum);
		fgbgcolor  = (((attr << 26) & 0x08000000) | ((attr << 20) & 0x00400000) | ((attr << 16) & 0x00010000) |
						    ((attr << 7) & 0x0800) | ((attr << 1) & 0x0040) | ((attr >> 3) & 0x0001)) * ((attr & 0x40) ? 0x1f : 0x18);
		register uint8_t is_flash_pos = (((attr & 0x80) >> 4) & frnum) >> 3;

			register uint8_t pixline = *(lineaddr+colnum);
			bufshft = BORDER_WIDTH+colnum*8+7;
			for(register uint8_t pixnum=0; pixnum<8; pixnum++) {
				if(((pixline >> pixnum) & 0x01) ^ is_flash_pos)
					linebuf[bufshft-pixnum] = fgbgcolor >> 16;
				else
					linebuf[bufshft-pixnum] = fgbgcolor;
			}
	}
	for(register uint8_t i = 0; i< BORDER_WIDTH; i++)
		linebuf[i] = linebuf[BORDER_WIDTH+ZX_PIXELS+i] = zx_border_color;

	ILI9341_sendBuf(ZXD_START_POS-BORDER_WIDTH, ZXD_START_LINE+lnum, ZXD_END_POS+BORDER_WIDTH, ZXD_START_LINE+lnum, linebuf, (ZX_PIXELS+BORDER_WIDTH*2));

	lnum++;
	if(lnum >= ZX_LINES) {
		lnum = 0;
		frnum++;
		if(frnum > 0x0f) frnum = 0;
	}

	ZX_NEWLINE_RESET;

	return lnum;
}


