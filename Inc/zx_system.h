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

uint8_t is_zx_running;

void ZXsys_Run();

#endif /* ZX_SYSTEM_H_ */
