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

typedef void (*z80cmd_t)(register uint32_t code);

uint8_t calc_F(register uint8_t src1, register uint8_t src2, register int16_t res, register uint8_t op);

void NOP(register uint32_t code);
void HLT(register uint32_t code);
void DI_(register uint32_t code);
void EI_(register uint32_t code);
void SCF(register uint32_t code);
void CCF(register uint32_t code);
void LDn(register uint32_t code);
void LDx(register uint32_t code);
void LDm(register uint32_t code);
void LDs(register uint32_t code);
void LD_(register uint32_t code);
void ICx(register uint32_t code);
void IC8(register uint32_t code);
void DCx(register uint32_t code);
void DC8(register uint32_t code);
void CPL(register uint32_t code);
void ALx(register uint32_t code);
void ALn(register uint32_t code);
void ALU(register uint32_t code);
void JR_(register uint32_t code);
void RST(register uint32_t code);
void JPc(register uint32_t code);
void JMP(register uint32_t code);
void POP(register uint32_t code);
void PSH(register uint32_t code);
void SFT(register uint32_t code);
void EX_(register uint32_t code);
void IO_(register uint32_t code);
void DAA(register uint32_t code);

extern const uint8_t optstates[256];
extern const uint8_t edoptstates[256];

extern const z80cmd_t z80ops[256];

extern const uint8_t sz53p_table[256];

#endif /* SRC_Z80_OPS_H_ */
