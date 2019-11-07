/*
 * cpm_display.c
 *
 *  Created on: Oct 6, 2019
 *      Author: Artur
 */

#include "stdlib.h"
#include "cpm_display.h"
#include "cpm_font.h"

static uint8_t cpos[2] = {0,0};
static uint16_t chbuf[FNT_WIDTH*FNT_HEIGHT];

void cpmdisp_setcursor(uint8_t row, uint8_t col) {
	cpos[COL] = col; cpos[ROW] = row;
}

//s - index in font table
inline static void drawsymbol(uint8_t s, uint8_t row, uint8_t col) {
	for(register uint8_t l=0;l<6;l++) {
		for(register uint8_t b=0;b<5;b++) {
			if((font[s]>>(l*5+b))&0x00000001)
				chbuf[l*6+b]=FG_COLOR;
			  else
				chbuf[l*6+b]=BG_COLOR;
		}
		chbuf[l*6+5]=BG_COLOR;
	}
	for(register uint8_t b=0;b<6;b++) {
		chbuf[FNT_WIDTH*(FNT_HEIGHT-2)+b]=BG_COLOR;
		chbuf[FNT_WIDTH*(FNT_HEIGHT-1)+b]=BG_COLOR;
	}
	ILI9341_sendBuf(
			CPMD_START_POS+col*FNT_WIDTH,
			CPMD_START_LINE+row*FNT_HEIGHT,
			CPMD_START_POS+col*FNT_WIDTH+FNT_WIDTH-1,
			CPMD_START_LINE+row*FNT_HEIGHT+FNT_HEIGHT-1,
			chbuf, FNT_WIDTH*FNT_HEIGHT);
}

void cpmdisp_scroll(uint8_t lnum) {
	for(register uint16_t i=CPMD_START_LINE; i< CPMD_START_LINE+FNT_HEIGHT*(SCR_HEIGHT-lnum); i++) {
		ILI9341_readBuf(CPMD_START_POS, i+FNT_HEIGHT*lnum, CPMD_END_POS, i+FNT_HEIGHT*lnum, linebuf, SCR_WIDTH*FNT_WIDTH);
		ILI9341_sendBuf(CPMD_START_POS, i, CPMD_END_POS, i, linebuf, SCR_WIDTH*FNT_WIDTH);
	}

	ILI9341_fillArea(CPMD_START_POS, CPMD_END_LINE-FNT_HEIGHT*lnum+1, CPMD_END_POS, CPMD_END_LINE, BG_COLOR);
}

void cpmdisp_Init() {
	ILI9341_Init();
	disp_clear(BG_COLOR);
	cpmdisp_setcursor(0, 0);
	drawsymbol(CURSOR_CHAR, cpos[ROW], cpos[COL]);
}

void cpmdisp_deInit() {
	disp_clear(BG_COLOR);
}

void cpmdisp_putc(char c) {
	if(c == '\0') return;
	if(c == '\n') {
		drawsymbol(0x00, cpos[ROW], cpos[COL]);
		cpos[COL]=0;
		if(++cpos[ROW] == SCR_HEIGHT) {
			cpmdisp_scroll(1);
			cpos[ROW] = SCR_HEIGHT-1;
		}
	} else {
		//draw symbol
		if(c < 0x20)
			c = 0x20;
		else
			c -= 0x20;

		drawsymbol((uint8_t)c, cpos[ROW], cpos[COL]);

		if(++cpos[COL] == SCR_WIDTH) {
			cpos[COL] = 0;
			if(++cpos[ROW] == SCR_HEIGHT) {
				cpmdisp_scroll(1);
				cpos[ROW] = SCR_HEIGHT-1;
			}
		}
	}
	drawsymbol(CURSOR_CHAR, cpos[ROW], cpos[COL]);
}

void cpmdisp_puts(char *s) {
	for(register uint16_t i=0;s[i]!='\0';i++)
		cpmdisp_putc(s[i]);
}
