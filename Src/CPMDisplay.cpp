/*
 * CPMDisplay.cpp
 *
 *  Created on: Oct 3, 2019
 *      Author: artur
 */

#include "CPMDisplay.h"

CPM_Display::CPM_Display() {
	// TODO Auto-generated constructor stub

}

CPM_Display::~CPM_Display() {
	// TODO Auto-generated destructor stub
}

void CPM_Display::Clear() {
	_LCD.setFrame(0, 0, LCD_PIXEL_WIDTH-1, LCD_PIXEL_HEIGHT-1);
}
