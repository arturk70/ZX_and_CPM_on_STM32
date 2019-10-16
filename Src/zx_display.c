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

void ZXdisp_clear() {
	ILI9341_fillArea(ZXD_START_POS, ZXD_START_LINE, ZXD_END_POS, ZXD_END_LINE, BLACK);
}


void ZXdisp_Init() {
	ZXvideomem = get_ZX_videomem();
	ILI9341_Init();
	ZXdisp_clear();
	linebuf = malloc(ZX_PIXELS*8*2);
	lnum = 0;
	frnum = 0;

	LL_TIM_EnableUpdateEvent(TIM3);
	LL_TIM_EnableCounter(TIM3);
	LL_TIM_EnableIT_UPDATE(TIM3);
}
void ZXdisp_deInit() {
	LL_TIM_DisableIT_UPDATE(TIM3);
	LL_TIM_DisableCounter(TIM3);
	free(linebuf);
}

void ZXdisp_drawnextline() {
	uint8_t *attraddr = ZXvideomem+0x1800+lnum*32;

	uint8_t attr;
	uint16_t fgcolor;
	uint16_t bgcolor;
	uint16_t tmp;

	for(uint8_t bnum=0; bnum<32; bnum++) {
		attr = *(attraddr+bnum);
		fgcolor  = (attr & 0x02) ? 0xf800 : 0;
		fgcolor |= (attr & 0x04) ? 0x07e0 : 0;
		fgcolor |= (attr & 0x01) ? 0x001f : 0;
		bgcolor  = (attr & 0x10) ? 0xf800 : 0;
		bgcolor |= (attr & 0x20) ? 0x07e0 : 0;
		bgcolor |= (attr & 0x08) ? 0x001f : 0;
		if(!(attr & 0x40)) {
			fgcolor &= 0xc618;
			bgcolor &= 0xc618;
		}
		if((attr & 0x80) && (frnum >25)) {
			tmp = fgcolor;
			fgcolor = bgcolor;
			bgcolor = tmp;
		}

		uint8_t *chladdr = ZXvideomem+(lnum<<8)+bnum;
		for(uint8_t ln=0; ln<8; ln++) {
			for(uint8_t pnum=0; pnum<8; pnum++) {
				if(((*(chladdr)) >> pnum) & 0x01)
					linebuf[ln*ZX_PIXELS+bnum*8+7-pnum] = fgcolor;
				else
					linebuf[ln*ZX_PIXELS+bnum*8+7-pnum] = bgcolor;
			}
			chladdr +=32;
		}
	}
	ILI9341_sendBuf(ZXD_START_POS, ZXD_START_LINE+lnum*8, ZXD_END_POS, ZXD_START_LINE+lnum*8+7, linebuf);

	lnum++;
	if(lnum >= ZX_LINES/8) {
		lnum = 0;
		frnum++;
		if(frnum >=50) frnum = 0;
	}

	ZX_NEWLINE_RESET;
}


