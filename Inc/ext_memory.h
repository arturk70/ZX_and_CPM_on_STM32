/*
 * ext_memory.h
 *
 *  Created on: Oct 8, 2019
 *      Author: artur
 */

#ifndef EXT_MEMORY_H_
#define EXT_MEMORY_H_

#include "main.h"
#include "ili9341_driver.h"

uint8_t extmem_read(uint16_t addr);
void extmem_write(uint16_t addr, uint8_t data);
void extmem_Init(uint16_t b1l, uint16_t b1r, uint16_t b1t, uint16_t b1b,
		uint16_t b2l, uint16_t b2r, uint16_t b2t, uint16_t b2b,
		uint16_t b3l, uint16_t b3r, uint16_t b3t, uint16_t b3b,
		uint16_t b4l, uint16_t b4r, uint16_t b4t, uint16_t b4b);

#endif /* EXT_MEMORY_H_ */
