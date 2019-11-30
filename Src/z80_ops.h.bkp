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

typedef void (*z80cmd_t)(uint8_t code, int8_t *tstates);

void NONI(uint8_t code, int8_t *tstates);
void NOP(uint8_t code, int8_t *tstates);
void HLT(uint8_t code, int8_t *tstates);
void DI_(uint8_t code, int8_t *tstates);
void EI_(uint8_t code, int8_t *tstates);
void SCF(uint8_t code, int8_t *tstates);
void CCF(uint8_t code, int8_t *tstates);
void RETN(uint8_t code, int8_t *tstates);
void RETI(uint8_t code, int8_t *tstates);
void IM_0(uint8_t code, int8_t *tstates);
void IM_1(uint8_t code, int8_t *tstates);
void IM_2(uint8_t code, int8_t *tstates);
void LDn(uint8_t code, int8_t *tstates);
void LDx(uint8_t code, int8_t *tstates);
void LDm(uint8_t code, int8_t *tstates);
void LDs(uint8_t code, int8_t *tstates);
void LD_(uint8_t code, int8_t *tstates);
void LDBL(uint8_t code, int8_t *tstates);
void LDIR(uint8_t code, int8_t *tstates);
void EDLD(uint8_t code, int8_t *tstates);
void ICx(uint8_t code, int8_t *tstates);
void IC8(uint8_t code, int8_t *tstates);
void DCx(uint8_t code, int8_t *tstates);
void DC8(uint8_t code, int8_t *tstates);
void CPL(uint8_t code, int8_t *tstates);
void ALx(uint8_t code, int8_t *tstates);
void ALn(uint8_t code, int8_t *tstates);
void ALU(uint8_t code, int8_t *tstates);
void NEG_(uint8_t code, int8_t *tstates);
void SBCx(uint8_t code, int8_t *tstates);
void ADCx(uint8_t code, int8_t *tstates);
void CPBL(uint8_t code, int8_t *tstates);
void JR_(uint8_t code, int8_t *tstates);
void RST(uint8_t code, int8_t *tstates);
void JPc(uint8_t code, int8_t *tstates);
void JMP(uint8_t code, int8_t *tstates);
void POP(uint8_t code, int8_t *tstates);
void PSH(uint8_t code, int8_t *tstates);
void SFT(uint8_t code, int8_t *tstates);
void CBSFT(uint8_t code, int8_t *tstates);
void EDSF(uint8_t code, int8_t *tstates);
void BIT(uint8_t code, int8_t *tstates);
void EX_(uint8_t code, int8_t *tstates);
void IO_(uint8_t code, int8_t *tstates);
void EDIN(uint8_t code, int8_t *tstates);
void EDOU(uint8_t code, int8_t *tstates);
void IOBL(uint8_t code, int8_t *tstates);
void DAA(uint8_t code, int8_t *tstates);

void PFX(uint8_t code, int8_t *tstates);

extern const uint8_t optstates[256];
extern const uint8_t edoptstates[96];

extern const z80cmd_t z80ops[256];
extern const z80cmd_t z80edops[96];

#endif /* SRC_Z80_OPS_H_ */
