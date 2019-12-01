/*
 * cpm_display.c
 *
 *  Created on: Oct 6, 2019
 *      Author: Artur
 */

#include "cpm_console.h"

#include "stdlib.h"
#include "cpm_font.h"

static uint8_t cpos[2] = {0,0};
static uint8_t escmode = 0;
static uint16_t* chbuf;
static uint8_t* scrbuf;

uint8_t cpmconsst = 0x00;
char cpmconsch = '\0';

//s - index in font table
static void drawsymbol(uint8_t row, uint8_t col, uint8_t inv) {
	register uint8_t s;
	register uint32_t fonts;
	register uint16_t fg, bg;

	s = scrbuf[row*SCR_WIDTH+col];
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

void cpmcons_scroll(uint8_t lnum) {
	for(register uint8_t i=0; i< SCR_HEIGHT-lnum; i++) {
		for(register uint8_t j=0; j< SCR_WIDTH; j++) {
			scrbuf[i*SCR_WIDTH+j] = scrbuf[(i+lnum)*SCR_WIDTH+j];
			drawsymbol(i, j, 0);
		}
	}
	for(register uint8_t i=SCR_HEIGHT-lnum; i< SCR_HEIGHT; i++)
		for(register uint8_t j=0; j< SCR_WIDTH; j++)
			scrbuf[i*SCR_WIDTH+j] = 0x00;

	ILI9341_fillArea(CPMD_START_POS, CPMD_END_LINE-FNT_HEIGHT*lnum+1, CPMD_END_POS, CPMD_END_LINE, BG_COLOR);
}

void cpmcons_init() {
	scrbuf = malloc(SCR_HEIGHT*SCR_WIDTH);
	chbuf = malloc(FNT_WIDTH*FNT_HEIGHT*2);
	cpmcons_clear();
}

void cpmcons_deinit() {
	free(scrbuf);
	free(chbuf);
}

void cpmcons_clear() {
	for(register uint8_t i=0;i<SCR_HEIGHT;i++)
		for(register uint8_t j=0; j< SCR_WIDTH; j++)
			scrbuf[i*SCR_WIDTH+j]=0x00;
	ILI9341_fillArea(CPMD_START_POS-2, CPMD_START_LINE, CPMD_END_POS+2, CPMD_END_LINE, BG_COLOR);
	setcursor(0, 0);
}

void cpmcons_putc(char c) {
	c &= 0x7f;
	register uint8_t newrow = cpos[ROW], newcol = cpos[COL];

	if(escmode) {
		if(escmode == 2) {
			newrow = c - 0x20;
			if(newrow >= SCR_HEIGHT)
				newrow = SCR_HEIGHT-1;
			escmode = 3;
		}
		else if(escmode == 3) {
			newcol = c - 0x20;
			if(newcol >= SCR_WIDTH)
				newcol = SCR_WIDTH-1;
			escmode = 1;
		}
		else if(c == 'Y') {
			escmode = 2;
		}
		else if(c == 'A') {
			if(newrow > 0)
				newrow--;
		}
		else if(c == 'B') {
			if(newrow < SCR_HEIGHT-1)
				newrow++;
		}
		else if(c == 'C') {
			if(newcol < SCR_WIDTH-1)
				newcol++;
		}
		else if(c == 'D') {
			if(newcol > 0)
				newcol--;
		}
		else if(c == 'H') {
			newrow = 0; newcol = 0;
		}
		else if(c == 'J') {
			for(register uint8_t i=newcol; i<SCR_WIDTH; i++) {
				scrbuf[newrow*SCR_WIDTH+i] = 0x00;
				drawsymbol(newrow, i, 0);
			}
			for(register uint8_t i=newrow+1; i<SCR_HEIGHT; i++)
				for(register uint8_t j=0; j<SCR_WIDTH; j++) {
					scrbuf[i*SCR_WIDTH+j] = 0x00;
					drawsymbol(i, j, 0);
				}
		}
		else if(c == 'K') {
			for(register uint8_t i=newcol; i<SCR_WIDTH; i++) {
				scrbuf[newrow*SCR_WIDTH+i] = 0x00;
				drawsymbol(newrow, i, 0);
			}
		}

		if(escmode == 1)
			escmode = 0;
	}
	else if(c == 0x07) {//Bell
	}
	else if(c == '\b') {//Backspace
		for(register uint8_t i=newcol; i<SCR_WIDTH; i++) {
			scrbuf[newrow*SCR_WIDTH+(i-1)] = scrbuf[newrow*SCR_WIDTH+i];
			drawsymbol(newrow, i-1, 0);
		}
		scrbuf[newrow*SCR_WIDTH+(SCR_WIDTH-1)] = 0x00;
		drawsymbol(newrow, SCR_WIDTH-1, 0);
		if(newcol > 0) {
			newcol--;
		}
		else {
			if(newrow > 0) {
				newrow--; newcol = SCR_WIDTH - 1;
			}
		}
	}
	else if(c == '\t') {//Tab
		newcol = (newcol & 0xf8) + 8;
		if(newcol >= SCR_WIDTH)
			newcol = SCR_WIDTH-1;
	}
	else if(c == '\n') {//LineFeed
		if(newrow == SCR_HEIGHT-1) {
			cpmcons_scroll(1);
			newrow = SCR_HEIGHT-1; newcol = 0;
		}
		else {
			newrow++; newcol = 0;
		}
	}
	else if(c == '\r') {//CarriageReturn
		newcol = 0;
	}
	else if(c == 0x1b) {//ESC
		escmode = 1;
	}
	else if(c < 0x20 || c == 0x7f)
		return;
	else {//draw ASCII symbol
		scrbuf[newrow*SCR_WIDTH+newcol] = (uint8_t)c;
		drawsymbol(newrow, newcol, 0);

		if(newcol == SCR_WIDTH-1) {
			if(newrow == SCR_HEIGHT-1) {
				cpmcons_scroll(1);
				newrow = SCR_HEIGHT-1; newcol = 0;
			}
			else {
				newrow++; newcol = 0;
			}
		}
		else
			newcol++;
	}

	setcursor(newrow, newcol);
}

void cpmcons_puts(const char *s) {
	for(register uint16_t i=0;s[i]!='\0';i++)
		cpmcons_putc(s[i]);
}

void cpmcons_errmsg(uint8_t errno, const char *s) {
	//char buf[4];
	cpmcons_puts("Error #");
	cpmcons_putc('0'+errno);
	//cpmcons_puts(utoa(errno, buf, 10));
	cpmcons_putc(' ');
	cpmcons_puts(s);
	cpmcons_putc('\n');
}

void cpmcons_getkey() {
	register char sym = '\0';
	sym = cpmkbd_read();
	if(sym != '\0') {
		cpmconsst = 0xff;
		cpmconsch = sym;
	}
	else {
		cpmconsst = 0x00;
		cpmconsch = '\0';
	}
}

char cpmcons_getc() {
	do {
		cpmcons_getkey();
	} while(cpmconsst == 0x00);

	return cpmconsch;
}

void cpmcons_gets(char* buf, uint8_t num) {
	register uint8_t ptr = 0;
	register char sym;
	do {
		sym = cpmcons_getc();
		cpmcons_putc(sym);
		if(sym == '\n')
			break;
		else if(sym == '\b') {
			ptr--;
		}
		else
			buf[ptr++] = sym;
	} while(ptr < num);
	buf[ptr] = '\0';
}
