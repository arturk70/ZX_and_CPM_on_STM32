/*
 * Display.cpp
 *
 *  Created on: Oct 3, 2019
 *      Author: artur
 */

#include "Display.h"

Display::Display() {
	// TODO Auto-generated constructor stub

}

Display::~Display() {
	// TODO Auto-generated destructor stub
}

void Display::Init() {
	_LCD.Init();
	_LCD.fillArea(0, 0, 319, 239, 0x0000);
}

