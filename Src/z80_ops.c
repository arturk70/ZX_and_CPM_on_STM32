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
		4, 4, 4, 11, 4, 4, 4, 4, 4, 4, 4, 11, 4, 4, 4, 4, //0xd0-0xdf
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, //0xe0-0xef
		4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4  //0xf0-0xff
};

const z80cmd_t z80ops[256] = {
		CTR, LDX, LD8, INC, INC, DEC, LD8, SFT, EX_, ALU, LD8, DEC, INC, DEC, LD8, SFT, //0x00-0x0f
		JMP, LDX, LD8, INC, INC, DEC, LD8, SFT, JMP, ALU, LD8, DEC, INC, DEC, LD8, SFT, //0x10-0x1f
		JMP, LDX, LDX, INC, INC, DEC, LD8, DAA, JMP, ALU, LDX, DEC, INC, DEC, LD8, ALU, //0x20-0x2f
		JMP, LDX, LD8, INC, INC, DEC, LD8, CTR, JMP, ALU, LD8, DEC, INC, DEC, LD8, CTR, //0x30-0x3f
		LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, //0x40-0x4f
		LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, //0x50-0x5f
		LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, //0x60-0x6f
		LD8, LD8, LD8, LD8, LD8, LD8, CTR, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, LD8, //0x70-0x7f
		ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, //0x80-0x8f
		ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, //0x90-0x9f
		ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, //0xa0-0xaf
		ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, //0xb0-0xbf
		JMP, STK, JMP, JMP, JMP, STK, ALU, JMP, JMP, JMP, JMP, PFX, JMP, JMP, ALU, JMP, //0xc0-0xcf
		JMP, STK, JMP, IO_, JMP, STK, ALU, JMP, JMP, EX_, JMP, IO_, JMP, PFX, ALU, JMP, //0xd0-0xdf
		JMP, STK, JMP, EX_, JMP, STK, ALU, JMP, JMP, JMP, JMP, EX_, JMP, PFX, ALU, JMP, //0xe0-0xef
		JMP, STK, JMP, CTR, JMP, STK, ALU, JMP, JMP, LDX, JMP, CTR, JMP, PFX, ALU, JMP  //0xf0-0xff
};

const z80cmd_t z80edops[96] = {
		__N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, //0x40-0x4f
		__N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, //0x50-0x5f
		__N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, //0x60-0x6f
		__N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, //0x70-0x7f
		__N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, //0xa0-0xaf
		__N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N, __N  //0xb0-0xbf
};

uint8_t __N(uint8_t code) {
	ERROR(code);

	return 0;
}

uint8_t CTR(uint8_t code) {
	uint8_t tstates = optstates[code];
	//TODO extended ops
	switch (code) {
	case 0x76://HALT
		state.halted = 1; break;
	case 0x37://SCF
		F = (F & (FLAG_P | FLAG_Z | FLAG_S)) | (A & (FLAG_3 | FLAG_5)) | FLAG_C; break;
	case 0x3f://CCF
		F = (F & (FLAG_P | FLAG_Z | FLAG_S)) | ((F & FLAG_C) ? FLAG_H : FLAG_C) | (A & (FLAG_3 | FLAG_5)); break;
	case 0xf3://DI
		IFF1 = IFF2 = 0; break;
	case 0xfb://EI
		IFF1 = IFF2 = 1; state.int_blocked = 2; break;
	case 0x00://NOP
		break;
	case 0xff://NONI
		state.int_blocked = 2; break;
	default:
		ERROR(code); break;
	}

	return tstates;
}

uint8_t LD8(uint8_t code) {
	uint8_t tstates = optstates[code];
	//TODO all
	switch (code) {
	default:
		ERROR(code); break;
	}

	return tstates;
}

uint8_t LDX(uint8_t code) {
	uint8_t tstates = optstates[code];
	//TODO all
	switch (code) {
	default:
		ERROR(code); break;
	}

	return tstates;
}

uint8_t INC(uint8_t code) {
	uint8_t tstates = optstates[code];
	//TODO all
	switch (code) {
	default:
		ERROR(code); break;
	}

	return tstates;
}

uint8_t DEC(uint8_t code) {
	uint8_t tstates = optstates[code];
	//TODO all
	switch (code) {
	default:
		ERROR(code); break;
	}

	return tstates;
}

uint8_t ALU(uint8_t code) {
	uint8_t tstates = optstates[code];
	//TODO all
	switch (code) {
	default:
		ERROR(code); break;
	}

	return tstates;
}

uint8_t JMP(uint8_t code) {
	uint8_t tstates = optstates[code];
	//TODO all
	switch (code) {
	default:
		ERROR(code); break;
	}

	return tstates;
}

uint8_t STK(uint8_t code) {
	uint8_t tstates = optstates[code];
	//TODO all
	switch (code) {
	default:
		ERROR(code); break;
	}

	return tstates;
}

uint8_t SFT(uint8_t code) {
	uint8_t tstates = optstates[code];
	//TODO all
	switch (code) {
	default:
		ERROR(code); break;
	}

	return tstates;
}

uint8_t BIT(uint8_t code) {
	uint8_t tstates = optstates[code];
	//TODO all
	switch (code) {
	default:
		ERROR(code); break;
	}

	return tstates;
}

uint8_t EX_(uint8_t code) {
	uint8_t tstates = optstates[code];
	//TODO all
	switch (code) {
	default:
		ERROR(code); break;
	}

	return tstates;
}

uint8_t IO_(uint8_t code) {
	uint8_t tstates = optstates[code];
	if(!IS_PREFIX) {
		switch (code) {
		case 0xd3://OUT (*),A
			port_out(((uint16_t)A << 8) | mem_read(PC++), A); break;
		case 0xdb://IN A,(*)
			A = port_in(((uint16_t)A << 8) | mem_read(PC++)); break;
		default:
			ERROR(code); break;
		}
	}
	else {
		ERROR(code);
	}

	return tstates;
}

uint8_t DAA(uint8_t code) {
	uint8_t tstates = optstates[code];
	//TODO all
	ERROR(code);

	return tstates;
}

uint8_t PFX(uint8_t code) {
	uint8_t tstates = optstates[code];

	switch (code) {
	case 0xed://ED
		state.prefix = 0x00ed; break;
	case 0xcb://CB
		state.prefix = (state.prefix & 0xff00) | 0x00cb; break;
	case 0xdd://DD
		state.prefix = 0xdd00; break;
	case 0xfd://FD
		state.prefix = 0xfd00; break;
	default:
		ERROR(code); break;
	}

	state.int_blocked = 2;

	return tstates;
}

