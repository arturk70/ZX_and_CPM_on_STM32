/*
 * kbd_driver.h
 *
 *  Created on: Oct 13, 2019
 *      Author: Artur
 */

#ifndef KBD_DRIVER_H_
#define KBD_DRIVER_H_

#include "main.h"

#define IS_SHIFT	(scan[0] & 0x01)
#define IS_CONTROL	(scan[7] & 0x02)


uint8_t zxkbd_scan(uint8_t addr);

char cpmkbd_read();

#endif /* KBD_DRIVER_H_ */
