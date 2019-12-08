/*
 * kbd_driver.h
 *
 *  Created on: Oct 13, 2019
 *      Author: Artur
 */

#ifndef KBD_DRIVER_H_
#define KBD_DRIVER_H_

#include "main.h"

//#define IS_SHIFT	(kbdscans[0] & 0x01)
//#define IS_CONTROL	(kbdscans[7] & 0x02)

#ifdef __SIMULATION
extern uint8_t ext_kbdscans[8];
#endif


uint8_t zxkbd_scan(register uint8_t addr);

char cpmkbd_read();

#endif /* KBD_DRIVER_H_ */
