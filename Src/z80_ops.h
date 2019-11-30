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
#define CALC_C(val)	(((val) & 0x100) ? FLAG_C : 0)
#define CALC_Z(val)	(((val) & 0xff) ? 0 : FLAG_Z)
#define CALC_ADD_H(arg1, arg2, res)	((((BIT4(arg1) | BIT4(arg2)) & ~BIT4(res)) | (BIT4(arg1) & BIT4(arg2) & BIT4(res))) ? FLAG_H : 0)
#define CALC_SUB_H(arg1, arg2, res)	((((BIT4(arg1) ^ BIT4(arg2)) & BIT4(arg2)) | (~(BIT4(arg1) ^ BIT4(arg2)) & BIT4(res))) ? FLAG_H : 0)
#define CALC_ADD_V(arg1, arg2, res)	((BIT8(arg1) & BIT8(arg2) & ~BIT8(res)) | ((~BIT8(arg1) & ~BIT8(arg2) & BIT8(res))) ? FLAG_V : 0)
#define CALC_SUB_V(arg1, arg2, res)	((BIT8(res) & BIT8(arg2) & ~BIT8(arg1)) | ((~BIT8(res) & ~BIT8(arg2) & BIT8(arg1))) ? FLAG_V : 0)

typedef uint8_t (*z80cmd_t)(uint8_t code);

uint8_t NONI(uint8_t code);
uint8_t NOP(uint8_t code);
uint8_t HLT(uint8_t code);
uint8_t DI_(uint8_t code);
uint8_t EI_(uint8_t code);
uint8_t SCF(uint8_t code);
uint8_t CCF(uint8_t code);
uint8_t RETN(uint8_t code);
uint8_t RETI(uint8_t code);
uint8_t IM_0(uint8_t code);
uint8_t IM_1(uint8_t code);
uint8_t IM_2(uint8_t code);
uint8_t LDn(uint8_t code);
uint8_t LDx(uint8_t code);
uint8_t LDm(uint8_t code);
uint8_t LDs(uint8_t code);
uint8_t LD_(uint8_t code);
uint8_t LDBL(uint8_t code);
uint8_t LDIR(uint8_t code);
uint8_t EDLD(uint8_t code);
uint8_t ICx(uint8_t code);
uint8_t IC8(uint8_t code);
uint8_t DCx(uint8_t code);
uint8_t DC8(uint8_t code);
uint8_t CPL(uint8_t code);
uint8_t ALx(uint8_t code);
uint8_t ALn(uint8_t code);
uint8_t ALU(uint8_t code);
uint8_t NEG_(uint8_t code);
uint8_t SBCx(uint8_t code);
uint8_t ADCx(uint8_t code);
uint8_t CPBL(uint8_t code);
uint8_t JR_(uint8_t code);
uint8_t RST(uint8_t code);
uint8_t JPc(uint8_t code);
uint8_t JMP(uint8_t code);
uint8_t POP(uint8_t code);
uint8_t PSH(uint8_t code);
uint8_t SFT(uint8_t code);
uint8_t CBSFT(uint8_t code);
uint8_t EDSF(uint8_t code);
uint8_t BIT(uint8_t code);
uint8_t EX_(uint8_t code);
uint8_t IO_(uint8_t code);
uint8_t EDIN(uint8_t code);
uint8_t EDOU(uint8_t code);
uint8_t IOBL(uint8_t code);
uint8_t DAA(uint8_t code);

uint8_t PFX(uint8_t code);

extern const uint8_t optstates[256];
extern const uint8_t edoptstates[96];

extern const z80cmd_t z80ops[256];
extern const z80cmd_t z80edops[96];

#endif /* SRC_Z80_OPS_H_ */
