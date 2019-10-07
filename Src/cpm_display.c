/*
 * cpm_display.c
 *
 *  Created on: Oct 6, 2019
 *      Author: Artur
 */

#include "cpm_display.h"

typedef uint8_t symbol [8];

static uint8_t cpos[2] = {0,0};

static const symbol font[96] = {
	{0x00, 0x00, 0x00, 0x00, 0x00},
	{0x00, 0x5e, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0b00111100,
	 0b01100010,
	 0b01010010,
	 0b01001010,
	 0b00111100},
	{0b00000000,
	 0b01000100,
	 0b01111110,
	 0b01000000,
	 0b00000000},
	{0b01100100,
	 0b01010010,
	 0b01010010,
	 0b01010010,
	 0b01001100},
	{0b00100100,
	 0b01000010,
	 0b01001010,
	 0b01001010,
	 0b00110100},
	{0b00110000,
	 0b00101000,
	 0b00100100,
	 0b01111110,
	 0b00100000},
	{0b00101110,
	 0b01001010,
	 0b01001010,
	 0b01001010,
	 0b00110010},

	{0b00111100,
	 0b01001010,
	 0b01001010,
	 0b01001010,
	 0b00110010},
	{0b00000010,
	 0b01100010,
	 0b00010010,
	 0b00001010,
	 0b00000110},
	{0b00110100,
	 0b01001010,
	 0b01001010,
	 0b01001010,
	 0b00110100},
	{0b00001100,
	 0b01010010,
	 0b01010010,
	 0b01010010,
	 0b00111100},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x00, 0x00, 0x00, 0x00},
	{0xff, 0x81, 0x81, 0x81, 0xff}
};

void cpmdisp_setcursor(uint8_t row, uint8_t col) {
	cpos[COL] = col; cpos[ROW] = row;
}

void cpmdisp_scroll(uint8_t lnum) {
	//TODO scroll
}

inline static void drawsymbol(uint8_t s) {
	uint16_t chbuf[FNT_WIDTH*FNT_HEIGHT];

	for(uint8_t l=0;l<FNT_HEIGHT;l++) {
		for(uint8_t b=0;b<FNT_WIDTH-1;b++) {
			if((font[s][b]>>l)&0x01)
				chbuf[l*FNT_WIDTH+b]=FG_COLOR;
			  else
				chbuf[l*FNT_WIDTH+b]=BG_COLOR;
			chbuf[l*FNT_WIDTH+FNT_WIDTH-1]=BG_COLOR;
		}
	}
	ILI9341_sendBuf(
			START_POS+cpos[COL]*FNT_WIDTH,
			START_LINE+cpos[ROW]*FNT_HEIGHT,
			START_POS+cpos[COL]*FNT_WIDTH+FNT_WIDTH-1,
			START_LINE+cpos[ROW]*FNT_HEIGHT+FNT_HEIGHT-1,
			chbuf, FNT_WIDTH*FNT_HEIGHT);
}

void cpmdisp_clear() {
	ILI9341_fillArea(START_POS, START_LINE, END_POS, END_LINE, BG_COLOR);
}

void cpmdisp_Init() {
	ILI9341_Init();
	cpmdisp_clear();
	cpos[ROW] = 0; cpos[COL] = 0;
	drawsymbol(CURSOR_CHAR);
}

void cpmdisp_putc(char c) {
	if(c == '\n') {
		drawsymbol(0x00);
		cpos[COL]=0;
		if(++cpos[ROW] >= SCR_HEIGHT) {
			cpmdisp_scroll(1);
			cpos[ROW] = SCR_HEIGHT-1;
		}
	} else {
		//draw symbol
		if(c < 0x20)
			c = 0x20;
		else
			c -= 0x20;

		drawsymbol((uint8_t)c);

		if(++cpos[COL] == SCR_WIDTH) {
			cpos[COL] = 0;
			if(++cpos[ROW] == SCR_HEIGHT) {
				cpmdisp_scroll(1);
				cpos[ROW] = SCR_HEIGHT-1;
			}
		}
	}
	drawsymbol(CURSOR_CHAR);
}

void cpmdisp_puts(char *s) {
	for(uint32_t i;s[i]!='\0';i++)
		cpmdisp_putc(s[i]);
}
