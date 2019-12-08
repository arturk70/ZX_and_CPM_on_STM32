/*
 * ZX_system.h
 *
 *  Created on: Oct 14, 2019
 *      Author: artur
 */

#ifndef ZX_SYSTEM_H_
#define ZX_SYSTEM_H_

#include "main.h"
#include "memory.h"
#include "zx_display.h"
#include "z80.h"
#include "kbd_driver.h"

extern uint8_t zxsys_isrun;

void zxsys_Run();
void zxports_out(register uint16_t addr, register uint8_t data);
uint8_t zxports_in(register uint16_t addr);

#endif /* ZX_SYSTEM_H_ */
