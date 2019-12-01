/*
 * ZX_display.h
 *
 *  Created on: Oct 14, 2019
 *      Author: artur
 */

#ifndef ZX_DISPLAY_H_
#define ZX_DISPLAY_H_

#include "main.h"
#include "ili9341_driver.h"
#include "ext_memory.h"

#define ZX_LINES		192
#define ZX_PIXELS		256
#define BORDER_WIDTH	24

#define ZXD_START_LINE	((ILI9341_PHEIGHT-ZX_LINES)/2)
#define ZXD_END_LINE	(ZXD_START_LINE+ZX_LINES-1)
#define ZXD_START_POS	((ILI9341_PWIDTH-ZX_PIXELS)/2)
#define ZXD_END_POS		(ZXD_START_POS+ZX_PIXELS-1)

#define ZX_NEWLINE_SET {zx_newline_flag = 1;}
#define ZX_NEWLINE_RESET {zx_newline_flag = 0;}

#ifndef __SIMULATION
#define TIM3_IRQ_HANDLER()	{if(LL_TIM_IsActiveFlag_UPDATE(TIM3)) {LL_TIM_ClearFlag_UPDATE(TIM3);ZX_NEWLINE_SET;/*mem_time++;*/}}
#endif

extern uint8_t zx_newline_flag;
extern uint16_t zx_border_color;

void zxdisp_init();
void zxdisp_deinit();
void zxdisp_drawnextline();


#endif /* ZX_DISPLAY_H_ */
