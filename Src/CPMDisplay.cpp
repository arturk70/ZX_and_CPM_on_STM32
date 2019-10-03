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
	_cursorxy[0]=0;
	_cursorxy[1]=0;
}

void CPM_Display::putc(uint8_t ch) {
	uint8_t* chindex=(uint8_t*)((ch<0x20||ch>0xEF) ? _font8x8table+(ch+95)*8 : _font8x8table+(ch-0x20)*8);
	for(uint8_t l=0;l<8;l++) {
	  for(uint8_t b=0;b<8;b++) {
		  if((chindex[l]>>b)&0x01)
			_charbuf[l][b]=GREEN;
		  else
			_charbuf[l][b]=BLACK;
	  }
	}
	_LCD.setFrame(_cursorxy[0], _cursorxy[1], _cursorxy[0]+7, _cursorxy[1]+7);
	_LCD.SendBuf(&_charbuf[0][0], 8*8);
	_cursorxy[0]++;
	if(_cursorxy[0]>=LCD_PIXEL_WIDTH/8) {
		_cursorxy[0]=0;
		_cursorxy[1]++;
		if(_cursorxy[1]>=LCD_PIXEL_HEIGHT/8) {
			//TODO scroll
			_cursorxy[1]=LCD_PIXEL_HEIGHT/8-1;
		}
	}
}
