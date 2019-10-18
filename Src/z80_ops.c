/*
 * z80_ops.c
 *
 *  Created on: Oct 18, 2019
 *      Author: artur
 */

#include "z80_ops.h"

const uint8_t optstates[256] = {
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, //0x00-0x0f
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, //0x10-0x1f
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, //0x20-0x2f
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, //0x30-0x3f
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, //0x40-0x4f
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, //0x50-0x5f
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, //0x60-0x6f
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, //0x70-0x7f
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, //0x80-0x8f
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, //0x90-0x9f
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, //0xa0-0xaf
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, //0xb0-0xbf
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, //0xc0-0xcf
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, //0xd0-0xdf
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, //0xe0-0xef
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4  //0xf0-0xff
};

const z80cmd_t z80ops[256] = {
		NOP, LDX, LD8, INC, INC, DEC, LD8, __N, __N, __N, LD8, DEC, INC, DEC, LD8, __N, //0x00-0x0f
		__N, LDX, LD8, INC, INC, DEC, LD8, __N, __N, __N, LD8, DEC, INC, DEC, LD8, __N, //0x10-0x1f
		__N, LDX, LDX, INC, INC, DEC, LD8, __N, __N, __N, LDX, DEC, INC, DEC, LD8, __N, //0x20-0x2f
		__N, LDX, LD8, INC, INC, DEC, LD8, __N, __N, __N, LD8, DEC, INC, DEC, LD8, __N, //0x30-0x3f
		LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, //0x40-0x4f
		LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, //0x50-0x5f
		LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, //0x60-0x6f
		LD8, LD8, LD8, LD8, LD8, LD8, __N, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, //0x70-0x7f
		__N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, //0x80-0x8f
		__N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, //0x90-0x9f
		__N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, //0xa0-0xaf
		__N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, //0xb0-0xbf
		__N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, //0xc0-0xcf
		__N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, //0xd0-0xdf
		__N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, //0xe0-0xef
		__N, __N, __N, __N, __N, __N, __N, __N, __N, LDX, __N, __N, __N, __N, __N, __N  //0xf0-0xff
};

void __N(uint8_t code) {

}

void NOP(uint8_t code) {

}

void LD8(uint8_t code) {

}

void LDX(uint8_t code) {

}

void INC(uint8_t code) {

}

void DEC(uint8_t code) {

}
