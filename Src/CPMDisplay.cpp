/*
 * CPMDisplay.cpp
 *
 *  Created on: Oct 3, 2019
 *      Author: artur
 */

#include "CPMDisplay.h"

CPM_Display::CPM_Display(const uint8_t* font8x8table) {
	_font8x8table = font8x8table;

}

CPM_Display::~CPM_Display() {
	// TODO Auto-generated destructor stub
}

void CPM_Display::Clear() {
	_LCD.fillArea(0, 0, LCD_PIXEL_WIDTH-1, LCD_PIXEL_HEIGHT-1, 0x0000);
}
