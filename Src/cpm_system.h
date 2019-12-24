/*
 * CPM_system.h
 *
 *  Created on: Oct 14, 2019
 *      Author: artur
 */

#ifndef CPM_SYSTEM_H_
#define CPM_SYSTEM_H_

#include "main.h"

#include "cpm_console.h"
#include "memory.h"
#include "z80.h"
#include "kbd_driver.h"

void cpmsys_Run();
void cpmports_out(register uint32_t addr, register uint32_t data);
uint8_t cpmports_in(register uint32_t addr);

#endif /* CPM_SYSTEM_H_ */
