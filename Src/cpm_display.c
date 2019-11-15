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
static uint8_t escmode = 0;
static uint16_t chbuf[FNT_WIDTH*FNT_HEIGHT];
static uint8_t scrbuf[SCR_HEIGHT][SCR_WIDTH];

//s - index in font table
static void drawsymbol(uint8_t row, uint8_t col, uint8_t inv) {
	register uint8_t s;
	register uint32_t fonts;
	register uint16_t fg, bg;

	s = scrbuf[row][col];
	if(s != 0x00)
		s -= 0x20;
	fonts = font[s];

	if(inv) {
		fg = BG_COLOR;
		bg = FG_COLOR;
	}
	else {
		fg = FG_COLOR;
		bg = BG_COLOR;
	}

	for(register uint8_t l=0;l<8;l++) {
		for(register uint8_t b=0;b<6;b++) {
			if(b == 5)
				chbuf[l*6+b] = bg;
			else {
				if(l == 0 || l == 7)
					chbuf[l*6+b] = bg;
				else {
					if(((fonts>>((l-1)*5+b)) & 0x00000001))
						chbuf[l*6+b]=fg;
					else
						chbuf[l*6+b]=bg;
				}
			}

		}
	}

	ILI9341_sendBuf(
			CPMD_START_POS+col*FNT_WIDTH,
			CPMD_START_LINE+row*FNT_HEIGHT,
			CPMD_START_POS+col*FNT_WIDTH+FNT_WIDTH-1,
			CPMD_START_LINE+row*FNT_HEIGHT+FNT_HEIGHT-1,
			chbuf, FNT_WIDTH*FNT_HEIGHT);
}

static void setcursor(uint8_t row, uint8_t col) {

	drawsymbol(cpos[ROW], cpos[COL], 0);
	if(row >= SCR_HEIGHT)
		row = SCR_HEIGHT-1;
	if(col >= SCR_WIDTH)
		col = SCR_WIDTH-1;
	cpos[ROW] = row; cpos[COL] = col;
	drawsymbol(row, col, 1);
}

void cpmdisp_scroll(uint8_t lnum) {
	for(register uint8_t i=0; i< SCR_HEIGHT-lnum; i++) {
		for(register uint8_t j=0; j< SCR_WIDTH; j++) {
			scrbuf[i][j] = scrbuf[i+lnum][j];
			drawsymbol(i, j, 0);
		}
	}
	for(register uint8_t i=SCR_HEIGHT-lnum; i< SCR_HEIGHT; i++)
		for(register uint8_t j=0; j< SCR_WIDTH; j++)
			scrbuf[i][j] = 0x00;

	ILI9341_fillArea(CPMD_START_POS, CPMD_END_LINE-FNT_HEIGHT*lnum+1, CPMD_END_POS, CPMD_END_LINE, BG_COLOR);
	setcursor(SCR_HEIGHT-lnum, 0);
}

void cpmdisp_Init() {
	ILI9341_Init();
	ILI9341_clear(BG_COLOR);
	for(register uint8_t i=0;i<SCR_HEIGHT;i++)
		for(register uint8_t j=0; j< SCR_WIDTH; j++)
			scrbuf[i][j]=0x00;
	setcursor(0, 0);
}

//void cpmdisp_deInit() {
//	ILI9341_clear(BG_COLOR);
//}

void cpmdisp_putc(char c) {
	c &= 0x7f;

	if(escmode) {
		if(escmode == 2) {
			register uint8_t newrow = c - 31;
			if(newrow >= SCR_HEIGHT)
				newrow = SCR_HEIGHT-1;
			setcursor(newrow, cpos[COL]);
			escmode = 3;
			return;
		}
		else if(escmode == 3) {
			register uint8_t newcol = c - 31;
			if(newcol >= SCR_WIDTH)
				newcol = SCR_WIDTH-1;
			setcursor(cpos[ROW], newcol);
		}
		else if(c == 'Y') {
			escmode = 2;
			return;
		}
		else if(c == 'A') {
			if(cpos[ROW] > 0)
				setcursor(cpos[ROW]-1, cpos[COL]);
		}
		else if(c == 'B') {
			if(cpos[ROW] < SCR_HEIGHT-1)
				setcursor(cpos[ROW]+1, cpos[COL]);
		}
		else if(c == 'C') {
			if(cpos[COL] < SCR_WIDTH-1)
				setcursor(cpos[ROW], cpos[COL]+1);
		}
		else if(c == 'D') {
			if(cpos[COL] > 0)
				setcursor(cpos[ROW], cpos[COL]-1);
		}
		else if(c == 'H') {
			setcursor(0, 0);
		}
		else if(c == 'J') {
			for(register uint8_t i=cpos[COL]; i<SCR_WIDTH; i++) {
				scrbuf[cpos[ROW]][i] = 0x00;
				drawsymbol(cpos[ROW], i, 0);
			}
			for(register uint8_t i=cpos[ROW]+1; i<SCR_HEIGHT; i++)
				for(register uint8_t j=0; j<SCR_WIDTH; j++) {
					scrbuf[i][j] = 0x00;
					drawsymbol(i, j, 0);
				}
			setcursor(cpos[ROW], cpos[COL]);
		}
		else if(c == 'K') {
			for(register uint8_t i=cpos[COL]; i<SCR_WIDTH; i++) {
				scrbuf[cpos[ROW]][i] = 0x00;
				drawsymbol(cpos[ROW], i, 0);
			}
			setcursor(cpos[ROW], cpos[COL]);
		}

		escmode = 0;
		return;
	}

	if(c == 0x07) {//Bell
	}
	else if(c == '\b') {//Backspace
		for(register uint8_t i=cpos[COL]; i<SCR_WIDTH; i++) {
			scrbuf[cpos[ROW]][i-1] = scrbuf[cpos[ROW]][i];
			drawsymbol(cpos[ROW], i-1, 0);
		}
		scrbuf[cpos[ROW]][SCR_WIDTH-1] = 0x00;
		drawsymbol(cpos[ROW], SCR_WIDTH-1, 0);
		if(cpos[COL] > 0) {
			setcursor(cpos[ROW], cpos[COL]-1);
		}
		else {
			if(cpos[ROW] > 0) {
				setcursor(cpos[ROW]-1, SCR_WIDTH - 1);
			}
		}
	}
	else if(c == '\t') {//Tab
		register uint8_t newcol = (cpos[ROW] & 0xf8) + 8;
		if(newcol >= SCR_WIDTH)
			newcol = SCR_WIDTH-1;
		setcursor(cpos[ROW], newcol);
	}
	else if(c == '\n') {//LineFeed
		if(cpos[ROW] == SCR_HEIGHT-1)
			cpmdisp_scroll(1);
		else
			setcursor(cpos[ROW]+1, 0);
	}
	else if(c == '\r') {//CarriageReturn
		setcursor(cpos[ROW], 0);
	}
	else if(c == 0x1b) {//ESC
		escmode = 1;
	}
	else if(c < 0x20 || c == 0x7f)
		return;
	else {
		//draw ASCII symbol
		scrbuf[cpos[ROW]][cpos[COL]] = (uint8_t)c;
		drawsymbol(cpos[ROW], cpos[COL], 0);

		if(cpos[COL] == SCR_WIDTH-1) {
			if(cpos[ROW] == SCR_HEIGHT-1)
				cpmdisp_scroll(1);
			else
				setcursor(cpos[ROW]+1, 0);
		}
		else
			setcursor(cpos[ROW], cpos[COL]+1);
	}
}

void cpmdisp_puts(char *s) {
	for(register uint16_t i=0;s[i]!='\0';i++)
		cpmdisp_putc(s[i]);
}
