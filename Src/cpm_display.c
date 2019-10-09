/*
 * cpm_display.c
 *
 *  Created on: Oct 6, 2019
 *      Author: Artur
 */

#include "stdlib.h"
#include "cpm_display.h"

static uint8_t cpos[2] = {0,0};
static uint16_t *chbuf;
static uint8_t *scrbuf;

static const uint32_t font[] = { //5x6 font
0x00000000, //' '
0x08021084, //'!'
0x0000014a, //'"'
0x15f52bea, //'#'
0x3f4f97e4, //'$'
0x33a21173, //'%'
0x2e9a88a2, //'&'
0x00000088, //'''
0x08210844, //'('
0x08842104, //')'
0x144f9140, //'*'
0x084f9080, //'+'
0x04c00000, //','
0x000f8000, //'-'
0x18c00000, //'.'
0x02222200, //'/'
0x1d3ae62e, //'0'
0x1c4210c4, //'1'
0x3e17422e, //'2'
0x1d18322e, //'3'
0x11f4a988, //'4'
0x1d183c3f, //'5'
0x1d18bc3e, //'6'
0x0422221f, //'7'
0x1d18ba2e, //'8'
0x1d0f462e, //'9'
0x00c00180, //':'
0x04c00180, //';'
0x00820888, //'<'
0x01f07c00, //'='
0x00222082, //'>'
0x0802222e, //'?'
0x1c14d72e, //'@'
0x231fc62e, //'A'
0x1f18be2f, //'B'
0x1d10862e, //'C'
0x1f18c62f, //'D'
0x3e10bc3f, //'E'
0x0210bc3f, //'F'
0x1d1c862e, //'G'
0x2318fe31, //'H'
0x3e42109f, //'I'
0x1d184210, //'J'
0x23149d31, //'K'
0x3e108421, //'L'
0x231ad771, //'M'
0x239ace31, //'N'
0x1d18c62e, //'O'
0x0217c62f, //'P'
0x1d9ac62e, //'Q'
0x2297c62f, //'R'
0x1d18382e, //'S'
0x0842109f, //'T'
0x1d18c631, //'U'
0x08a8c631, //'V'
0x15bac631, //'W'
0x23151151, //'X'
0x08422a31, //'Y'
0x3e11111f, //'Z'
0x1c21084e, //'['
0x20820820, //'\'
0x1c84210e, //']'
0x084255c4, //'^'
0x3e000000, //'_'
0x22e8c5d1, //'`'
0x3d1f41c0, //'a'
0x1f18bc21, //'b'
0x1d10c5c0, //'c'
0x3d18fa10, //'d'
0x3c17c5c0, //'e'
0x08423898, //'f'
0x1d0f47c0, //'g'
0x2318bc21, //'h'
0x1c421804, //'i'
0x0c942008, //'j'
0x22928da1, //'k'
0x18210842, //'l'
0x2b5ad560, //'m'
0x2318cda0, //'n'
0x1d18c5c0, //'o'
0x0217c5e0, //'p'
0x210f47c0, //'q'
0x021087c0, //'r'
0x1f0705c0, //'s'
0x304211c4, //'t'
0x1d18c620, //'u'
0x08a54620, //'v'
0x155ad620, //'w'
0x22a22a20, //'x'
0x1d0f4620, //'y'
0x3e2223e0, //'z'
0x38420c9c, //'{'
0x08421084, //'|'
0x0e426087, //'}'
0x000001b6, //'~'
0xffffffff //0x3f18c63f //''
};

void cpmdisp_setcursor(uint8_t row, uint8_t col) {
	cpos[COL] = col; cpos[ROW] = row;
}

//s - index in font table
inline static void drawsymbol(uint8_t s, uint8_t row, uint8_t col) {
	for(uint8_t l=0;l<6;l++) {
		for(uint8_t b=0;b<5;b++) {
			if((font[s]>>(l*5+b))&0x00000001)
				chbuf[l*6+b]=FG_COLOR;
			  else
				chbuf[l*6+b]=BG_COLOR;
		}
		chbuf[l*6+5]=BG_COLOR;
	}
	for(uint8_t b=0;b<6;b++) {
		chbuf[FNT_WIDTH*(FNT_HEIGHT-2)+b]=BG_COLOR;
		chbuf[FNT_WIDTH*(FNT_HEIGHT-1)+b]=BG_COLOR;
	}
	ILI9341_sendBuf(
			CPMD_START_POS+col*FNT_WIDTH,
			CPMD_START_LINE+row*FNT_HEIGHT,
			CPMD_START_POS+col*FNT_WIDTH+FNT_WIDTH-1,
			CPMD_START_LINE+row*FNT_HEIGHT+FNT_HEIGHT-1,
			chbuf);
}

void cpmdisp_scroll(uint8_t lnum) {
	for(uint16_t i=0; i< SCR_WIDTH*SCR_HEIGHT-SCR_WIDTH*lnum; i++) {
				scrbuf[i] = scrbuf[i+SCR_WIDTH*lnum];
				drawsymbol(scrbuf[i], i/SCR_WIDTH, i%SCR_WIDTH);
	}

	ILI9341_fillArea(CPMD_START_POS, CPMD_END_LINE-FNT_HEIGHT*lnum+1, CPMD_END_POS, CPMD_END_LINE, BG_COLOR);
}

void cpmdisp_clear() {
	ILI9341_fillArea(CPMD_START_POS, CPMD_START_LINE, CPMD_END_POS, CPMD_END_LINE, BG_COLOR);
}

void cpmdisp_Init() {
	ILI9341_Init();
	cpmdisp_clear();
	cpos[ROW] = 0; cpos[COL] = 0;
	chbuf=malloc(FNT_WIDTH*FNT_HEIGHT);
	scrbuf=malloc(SCR_WIDTH*SCR_HEIGHT);
	for(uint16_t i=0;i<SCR_WIDTH*SCR_HEIGHT;i++) scrbuf[i]=0x00;
	drawsymbol(CURSOR_CHAR, cpos[ROW], cpos[COL]);
}

void cpmdisp_putc(char c) {
	if(c == '\n') {
		for(uint8_t i=cpos[COL]; i< SCR_WIDTH; i++)
			scrbuf[cpos[ROW]*SCR_WIDTH+i] = 0x00;
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

		scrbuf[cpos[ROW]*SCR_WIDTH+cpos[COL]] = (uint8_t)c;
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
	for(uint16_t i=0;s[i]!='\0';i++)
		cpmdisp_putc(s[i]);
}
