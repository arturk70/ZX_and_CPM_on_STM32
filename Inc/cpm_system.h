/*
 * CPM_system.h
 *
 *  Created on: Oct 14, 2019
 *      Author: artur
 */

#ifndef CPM_SYSTEM_H_
#define CPM_SYSTEM_H_

#include "main.h"
#include "memory.h"
#include "cpm_display.h"
#include "z80.h"
#include "kbd_driver.h"

void CPMsys_Run() {
	cpmdisp_Init();
	mem_Init(MEMTYPE_CPM);

}

#endif /* CPM_SYSTEM_H_ */
