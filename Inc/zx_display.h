/*
 * ZX_display.h
 *
 *  Created on: Oct 14, 2019
 *      Author: artur
 */

#ifndef ZX_DISPLAY_H_
#define ZX_DISPLAY_H_

#include "main.h"

#define ZX_LINES	192
#define ZX_PIXELS	256

#define ZXD_START_LINE	((ILI9341_PHEIGHT-ZX_LINES)/2)
#define ZXD_END_LINE	(ZXD_START_LINE+ZX_LINES-1)
#define ZXD_START_POS	((ILI9341_PWIDTH-ZX_PIXELS)/2)
#define ZXD_END_POS		(ZXD_START_POS+ZX_PIXELS-1)

void ZXdisp_Init();
void ZXdisp_deInit();
void ZXdisp_drawline(uint8_t lnum);


#endif /* ZX_DISPLAY_H_ */
