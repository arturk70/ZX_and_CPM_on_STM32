/*
 * z80_ops.h
 *
 *  Created on: Oct 18, 2019
 *      Author: artur
 */

#ifndef SRC_Z80_OPS_H_
#define SRC_Z80_OPS_H_

#include <stdio.h>
#include "z80.h"

#define BIT4(val)	((val) & 0x08)
#define BIT8(val)	((val) & 0x80)
#define CALC_C(val)	(((val) >> 8) & FLAG_C)
#define CALC_Z(val)	(((val) & 0xff) ? 0 : FLAG_Z)
#define CALC_ADD_H(arg1, arg2, res)	(BIT4(((arg1 | arg2) & ~res) | (arg1 & arg2 & res)) ? FLAG_H : 0)
#define CALC_SUB_H(arg1, arg2, res)	(BIT4(((arg1 ^ arg2) & arg2) | (~(arg1 ^ arg2) & res)) ? FLAG_H : 0)
#define CALC_ADD_V(arg1, arg2, res)	(BIT8((arg1 & arg2 & ~res) | (~arg1 & ~arg2 & res)) ? FLAG_V : 0)
#define CALC_SUB_V(arg1, arg2, res)	(BIT8((res & arg2 & ~arg1) | (~res & ~arg2 & arg1)) ? FLAG_V : 0)

typedef uint8_t (*z80cmd_t)(register uint8_t code);

uint8_t NONI(register uint8_t code);
uint8_t NOP(register uint8_t code);
uint8_t HLT(register uint8_t code);
uint8_t DI_(register uint8_t code);
uint8_t EI_(register uint8_t code);
uint8_t SCF(register uint8_t code);
uint8_t CCF(register uint8_t code);
uint8_t RETN(register uint8_t code);
uint8_t RETI(register uint8_t code);
uint8_t IM_0(register uint8_t code);
uint8_t IM_1(register uint8_t code);
uint8_t IM_2(register uint8_t code);
uint8_t LDn(register uint8_t code);
uint8_t LDx(register uint8_t code);
uint8_t LDm(register uint8_t code);
uint8_t LDs(register uint8_t code);
uint8_t LD_(register uint8_t code);
uint8_t LDBL(register uint8_t code);
uint8_t LDIR(register uint8_t code);
uint8_t EDLD(register uint8_t code);
uint8_t ICx(register uint8_t code);
uint8_t IC8(register uint8_t code);
uint8_t DCx(register uint8_t code);
uint8_t DC8(register uint8_t code);
uint8_t CPL(register uint8_t code);
uint8_t ALx(register uint8_t code);
uint8_t ALn(register uint8_t code);
uint8_t ALU(register uint8_t code);
uint8_t NEG_(register uint8_t code);
uint8_t SBCx(register uint8_t code);
uint8_t ADCx(register uint8_t code);
uint8_t CPBL(register uint8_t code);
uint8_t JR_(register uint8_t code);
uint8_t RST(register uint8_t code);
uint8_t JPc(register uint8_t code);
uint8_t JMP(register uint8_t code);
uint8_t POP(register uint8_t code);
uint8_t PSH(register uint8_t code);
uint8_t SFT(register uint8_t code);
uint8_t CBSFT(register uint8_t code);
uint8_t EDSF(register uint8_t code);
uint8_t BIT(register uint8_t code);
uint8_t EX_(register uint8_t code);
uint8_t IO_(register uint8_t code);
uint8_t EDIN(register uint8_t code);
uint8_t EDOU(register uint8_t code);
uint8_t IOBL(register uint8_t code);
uint8_t DAA(register uint8_t code);

uint8_t PFX(register uint8_t code);

extern const uint8_t optstates[256];
extern const uint8_t edoptstates[96];

extern const z80cmd_t z80ops[256];
extern const z80cmd_t z80edops[96];

#endif /* SRC_Z80_OPS_H_ */
