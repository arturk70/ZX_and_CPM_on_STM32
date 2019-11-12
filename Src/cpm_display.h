/*
 * cpm_display.h
 *
 *  Created on: Oct 5, 2019
 *      Author: Artur
 */

#ifndef CPM_DISPLAY_H_
#define CPM_DISPLAY_H_

#include "ili9341_driver.h"

#define SCR_WIDTH	52
#define SCR_HEIGHT	30

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

void cpmdisp_setcursor(uint8_t row, uint8_t col);
void cpmdisp_Init();
//void cpmdisp_deInit();
void cpmdisp_putc(char c);
void cpmdisp_puts(char *s);


#endif /* CPM_DISPLAY_H_ */
