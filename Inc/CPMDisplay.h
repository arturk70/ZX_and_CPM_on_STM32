/*
 * CPMDisplay.h
 *
 *  Created on: Oct 3, 2019
 *      Author: artur
 */

#ifndef CPMDISPLAY_H_
#define CPMDISPLAY_H_

#include <Display.h>

class CPM_Display: public Display {
public:
	CPM_Display(const uint8_t* font8x8table);
	virtual ~CPM_Display();
	void Clear();
	void putc(uint8_t ch);
	inline void setColors(uint16_t fgclr, uint16_t bgclr) {_fgclr = fgclr; _bgclr = bgclr;}
private:
	uint16_t _fgclr = GREEN;
	uint16_t _bgclr = BLACK;
	const uint8_t* _font8x8table; //2 dim array. 1-st index - symbol number, 2-nd index - line number
	uint16_t _charbuf[8][8];
	uint16_t _cursorxy[2] = {0,0};
};

#endif /* CPMDISPLAY_H_ */
