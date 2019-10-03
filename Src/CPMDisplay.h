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
	CPM_Display();
	virtual ~CPM_Display();
	void Clear();
};

#endif /* CPMDISPLAY_H_ */
