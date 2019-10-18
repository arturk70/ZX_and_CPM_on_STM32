/*
 * z80_ops.h
 *
 *  Created on: Oct 18, 2019
 *      Author: artur
 */

#ifndef SRC_Z80_OPS_H_
#define SRC_Z80_OPS_H_

#include "z80.h"

typedef void (*z80cmd_t)(uint8_t code);

void __N(uint8_t code);
void NOP(uint8_t code);
void LD8(uint8_t code);
void LDX(uint8_t code);
void INC(uint8_t code);
void DEC(uint8_t code);

extern const uint8_t optstates[256];

extern const z80cmd_t z80ops[256];

#endif /* SRC_Z80_OPS_H_ */
