/*
 * cpm_display.h
 *
 *  Created on: Oct 5, 2019
 *      Author: Artur
 */

#ifndef CPM_CONSOLE_H_
#define CPM_CONSOLE_H_

#include "ili9341_driver.h"
#include "kbd_driver.h"

#define SCR_WIDTH	50
#define SCR_HEIGHT	24

#define FNT_WIDTH	6
#define FNT_HEIGHT	8

#define CPMD_START_LINE	((ILI9341_PHEIGHT-FNT_HEIGHT*SCR_HEIGHT)/2)
#define CPMD_END_LINE	(CPMD_START_LINE+FNT_HEIGHT*SCR_HEIGHT-1)
#define CPMD_START_POS	((ILI9341_PWIDTH-FNT_WIDTH*SCR_WIDTH)/2)
#define CPMD_END_POS		(CPMD_START_POS+FNT_WIDTH*SCR_WIDTH-1)

#define BG_COLOR	BLACK
#define FG_COLOR	GREEN

#define COL	0
#define ROW	1
#define CURSOR_CHAR	0x5f

extern uint8_t cpmconsst;
extern char cpmconsch;


void cpmcons_init();
void cpmcons_clear();
void cpmcons_deinit();
void cpmcons_putc(register char c);
void cpmcons_puts(register const char *s);
void cpmcons_errmsg(register uint32_t errno, register const char *s);
void cpmcons_getkey();
char cpmcons_getc();
void cpmcons_gets(register char *buf, register uint32_t num);


#endif /* CPM_CONSOLE_H_ */
