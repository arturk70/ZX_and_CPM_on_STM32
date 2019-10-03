/*
 * Display.h
 *
 *  Created on: Oct 3, 2019
 *      Author: artur
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "ILI9341driver.h"

class Display {
public:
	Display();
	virtual ~Display();
	void Init();
	virtual void Clear()=0;
private:
	ILI9341driver _LCD;
};

#endif /* DISPLAY_H_ */
