/*
 * display.h
 *
 *  Created on: Nov 7, 2019
 *      Author: artur
 */

#ifndef SRC_DISPLAY_H_
#define SRC_DISPLAY_H_

#include "ili9341_driver.h"

extern uint16_t linebuf[304];

#define DISP_CLEAR(bgcolor)	ILI9341_fillArea(8, 24, 311, 215, bgcolor)

#endif /* SRC_DISPLAY_H_ */
