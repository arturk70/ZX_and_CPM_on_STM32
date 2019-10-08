/*
 * dispmem_cashed.h
 *
 *  Created on: Oct 8, 2019
 *      Author: artur
 */

#ifndef DISPMEM_CASHED_H_
#define DISPMEM_CASHED_H_

#include "main.h"

#define CASH_BLOCK_SIZE		128
#define CASH_BLOCK_COUNT	8

uint8_t extmem_read(uint16_t addr);
void extmem_write(uint16_t addr, uint8_t data);
void extmem_Init(uint16_t b1l, uint16_t b1r, uint16_t b1t, uint16_t b1b,
		uint16_t b2l, uint16_t b2r, uint16_t b2t, uint16_t b2b,
		uint16_t b3l, uint16_t b3r, uint16_t b3t, uint16_t b3b,
		uint16_t b4l, uint16_t b4r, uint16_t b4t, uint16_t b4b);

#endif /* DISPMEM_CASHED_H_ */
