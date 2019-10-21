/*
 * z80_ops.c
 *
 *  Created on: Oct 18, 2019
 *      Author: artur
 */

#include "z80_ops.h"

const uint8_t optstates[256] = {
		4, 10,  7,  6,  4,  4,  7,  4,  4, 11,  7,  6,  4,  4, 7,  4, //0x00-0x0f
		8, 10,  7,  6,  4,  4,  7,  4, 12, 11,  7,  6,  4,  4, 7,  4, //0x10-0x1f
		7, 10, 16,  6,  4,  4,  7,  4,  7, 11, 16,  6,  4,  4, 7,  4, //0x20-0x2f
		7, 10, 13,  6, 11, 11, 10,  4,  7, 11, 13,  6,  4,  4, 7,  4, //0x30-0x3f
		4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4, 7,  4, //0x40-0x4f
		4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4, 7,  4, //0x50-0x5f
		4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4, 7,  4, //0x60-0x6f
		7,  7,  7,  7,  7,  7,  4,  7,  4,  4,  4,  4,  4,  4, 7,  4, //0x70-0x7f
		4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4, 7,  4, //0x80-0x8f
		4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4, 7,  4, //0x90-0x9f
		4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4, 7,  4, //0xa0-0xaf
		4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4, 7,  4, //0xb0-0xbf
		5, 10, 10, 10, 10, 11,  7, 11,  5, 10, 10,  4, 10, 17, 7, 11, //0xc0-0xcf
		5, 10, 10, 11, 10, 11,  7, 11,  5,  4, 10, 11, 10,  4, 7, 11, //0xd0-0xdf
		5, 10, 10, 19, 10, 11,  7, 11,  5,  4, 10,  4, 10,  4, 7, 11, //0xe0-0xef
		5, 10, 10,  4, 10, 11,  7, 11,  5,  6, 10,  4, 10,  4, 7, 11  //0xf0-0xff
};

const z80cmd_t z80ops[256] = {
		CTR, LD_, LD_, INC, INC, DEC, LD_, SFT, EX_, ALU, LD_, DEC, INC, DEC, LD_, SFT, //0x00-0x0f
		JMP, LD_, LD_, INC, INC, DEC, LD_, SFT, JMP, ALU, LD_, DEC, INC, DEC, LD_, SFT, //0x10-0x1f
		JMP, LD_, LD_, INC, INC, DEC, LD_, DAA, JMP, ALU, LD_, DEC, INC, DEC, LD_, ALU, //0x20-0x2f
		JMP, LD_, LD_, INC, INC, DEC, LD_, CTR, JMP, ALU, LD_, DEC, INC, DEC, LD_, CTR, //0x30-0x3f
		LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, //0x40-0x4f
		LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, //0x50-0x5f
		LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, //0x60-0x6f
		LD_, LD_, LD_, LD_, LD_, LD_, CTR, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, //0x70-0x7f
		ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, //0x80-0x8f
		ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, //0x90-0x9f
		ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, //0xa0-0xaf
		ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, //0xb0-0xbf
		JMP, STK, JMP, JMP, JMP, STK, ALU, JMP, JMP, JMP, JMP, PFX, JMP, JMP, ALU, JMP, //0xc0-0xcf
		JMP, STK, JMP, IO_, JMP, STK, ALU, JMP, JMP, EX_, JMP, IO_, JMP, PFX, ALU, JMP, //0xd0-0xdf
		JMP, STK, JMP, EX_, JMP, STK, ALU, JMP, JMP, JMP, JMP, EX_, JMP, PFX, ALU, JMP, //0xe0-0xef
		JMP, STK, JMP, CTR, JMP, STK, ALU, JMP, JMP, LD_, JMP, CTR, JMP, PFX, ALU, JMP  //0xf0-0xff
};

const uint8_t edoptstates[96] = {
		 8,  8, 11, 16, 4, 10, 4,  5,  8,  8, 11, 16, 4, 10, 4,  5, //0x40-0x4f
		 8,  8, 11, 16, 4, 10, 4,  5,  8,  8, 11, 16, 4, 10, 4,  5, //0x50-0x5f
		 8,  8, 11, 16, 4, 10, 4, 14,  8,  8, 11, 16, 4, 10, 4, 14, //0x60-0x6f
		 8,  8, 11, 16, 4, 10, 4, 14,  8,  8, 11, 16, 4, 10, 4, 14, //0x70-0x7f
		12, 12, 12, 12, 4,  4, 4,  4, 12, 12, 12, 12, 4,  4, 4,  4, //0xa0-0xaf
		12, 12, 12, 12, 4,  4, 4,  4, 12, 12, 12, 12, 4,  4, 4,  4  //0xb0-0xbf
};

const z80cmd_t z80edops[96] = {
		EDIO, EDIO, EDAL, EDLD, EDAL, EDCT, EDCT, EDLD, EDIO, EDIO, EDAL, EDLD, EDAL, EDCT, EDCT, EDLD, //0x40-0x4f
		EDIO, EDIO, EDAL, EDLD, EDAL, EDCT, EDCT, EDLD, EDIO, EDIO, EDAL, EDLD, EDAL, EDCT, EDCT, EDLD, //0x50-0x5f
		EDIO, EDIO, EDAL, EDLD, EDAL, EDCT, EDCT, EDSF, EDIO, EDIO, EDAL, EDLD, EDAL, EDCT, EDCT, EDSF, //0x60-0x6f
		EDIO, EDIO, EDAL, EDLD, EDAL, EDCT, EDCT, EDSF, EDIO, EDIO, EDAL, EDLD, EDAL, EDCT, EDCT, EDSF, //0x70-0x7f
		EDLD, EDAL, EDIO, EDIO, EDCT, EDCT, EDCT, EDCT, EDLD, EDAL, EDIO, EDIO, EDCT, EDCT, EDCT, EDCT, //0xa0-0xaf
		EDLD, EDAL, EDIO, EDIO, EDCT, EDCT, EDCT, EDCT, EDLD, EDAL, EDIO, EDIO, EDCT, EDCT, EDCT, EDCT  //0xb0-0xbf
};

const uint8_t sz53p_table[256] = {
		0x44,0x00,0x00,0x04,0x00,0x04,0x04,0x00,0x08,0x0c,0x0c,0x08,0x0c,0x08,0x08,0x0c,
		0x00,0x04,0x04,0x00,0x04,0x00,0x00,0x04,0x0c,0x08,0x08,0x0c,0x08,0x0c,0x0c,0x08,
		0x20,0x24,0x24,0x20,0x24,0x20,0x20,0x24,0x2c,0x28,0x28,0x2c,0x28,0x2c,0x2c,0x28,
		0x24,0x20,0x20,0x24,0x20,0x24,0x24,0x20,0x28,0x2c,0x2c,0x28,0x2c,0x28,0x28,0x2c,
		0x00,0x04,0x04,0x00,0x04,0x00,0x00,0x04,0x0c,0x08,0x08,0x0c,0x08,0x0c,0x0c,0x08,
		0x04,0x00,0x00,0x04,0x00,0x04,0x04,0x00,0x08,0x0c,0x0c,0x08,0x0c,0x08,0x08,0x0c,
		0x24,0x20,0x20,0x24,0x20,0x24,0x24,0x20,0x28,0x2c,0x2c,0x28,0x2c,0x28,0x28,0x2c,
		0x20,0x24,0x24,0x20,0x24,0x20,0x20,0x24,0x2c,0x28,0x28,0x2c,0x28,0x2c,0x2c,0x28,
		0x80,0x84,0x84,0x80,0x84,0x80,0x80,0x84,0x8c,0x88,0x88,0x8c,0x88,0x8c,0x8c,0x88,
		0x84,0x80,0x80,0x84,0x80,0x84,0x84,0x80,0x88,0x8c,0x8c,0x88,0x8c,0x88,0x88,0x8c,
		0xa4,0xa0,0xa0,0xa4,0xa0,0xa4,0xa4,0xa0,0xa8,0xac,0xac,0xa8,0xac,0xa8,0xa8,0xac,
		0xa0,0xa4,0xa4,0xa0,0xa4,0xa0,0xa0,0xa4,0xac,0xa8,0xa8,0xac,0xa8,0xac,0xac,0xa8,
		0x84,0x80,0x80,0x84,0x80,0x84,0x84,0x80,0x88,0x8c,0x8c,0x88,0x8c,0x88,0x88,0x8c,
		0x80,0x84,0x84,0x80,0x84,0x80,0x80,0x84,0x8c,0x88,0x88,0x8c,0x88,0x8c,0x8c,0x88,
		0xa0,0xa4,0xa4,0xa0,0xa4,0xa0,0xa0,0xa4,0xac,0xa8,0xa8,0xac,0xa8,0xac,0xac,0xa8,
		0xa4,0xa0,0xa0,0xa4,0xa0,0xa4,0xa4,0xa0,0xa8,0xac,0xac,0xa8,0xac,0xa8,0xa8,0xac
};

uint8_t NONI(uint8_t code) {
	state.int_blocked = 2;

	return 4;
}

uint8_t CTR(uint8_t code) {
	uint8_t tstates = optstates[code];
	if(code == 0xff)
		state.int_blocked = 2; //incorrect op NONI
	else {
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
		default:
			ERROR(code); break;
		}
	}
	return tstates;
}

uint8_t EDCT(uint8_t code) {
	uint8_t tstates;
	if(code < 0x80)
		tstates = edoptstates[code-0x40];
	else
		tstates = edoptstates[code-0x60];

	if((code == 0x45) || (code == 0x55) || (code == 0x65) || (code == 0x75) ||
	   (code == 0x4d) || (code == 0x5d) || (code == 0x6d) || (code == 0x7d)) { //RETI/RETN
		IFF1=IFF2;
		PCL = mem_read(SP++);
		PCH = mem_read(SP++);
	}
	else if((code == 0x46) || (code == 0x4e) || (code == 0x66) || (code == 0x6e)) {//IM 0
		IM = 0;
	}
	else if((code == 0x56) || (code == 0x76)) {//IM 1
		IM = 1;
	}
	else if((code == 0x5e) || (code == 0x7e)) {//IM 2
		IM = 2;
	}
	else
		state.int_blocked = 2;  //incorrect op NONI

	return tstates;
}

uint8_t LD_(uint8_t code) {
	uint8_t tstates = optstates[code];
	//TODO all
	switch (code) {
	default:
		ERROR(code); break;
	}

	return tstates;
}

uint8_t EDLD(uint8_t code) {
	uint8_t tstates;
	if(code < 0x80)
		tstates = edoptstates[code-0x40];
	else
		tstates = edoptstates[code-0x60];

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

uint8_t EDAL(uint8_t code) {
	uint8_t tstates;
	if(code < 0x80)
		tstates = edoptstates[code-0x40];
	else
		tstates = edoptstates[code-0x60];

	if(code & 0x04) {//NEG
		//TODO
	}
	else {
		//TODO CPI CPD
	}

	switch (code) {
	default:
		ERROR(code); break;
	}

	return tstates;
}

uint8_t JMP(uint8_t code) {
	uint8_t tstates = optstates[code];
	uint8_t dh, dl;
	int8_t d;

	if(code < 0x40) {
		d = mem_read(PC++);
		switch (code) {
		case 0x10://DJNZ
			if(--B) {
				PC += d;
				tstates += 5;
			}
			break;
		case 0x20://JR NZ
			if(!(F & FLAG_Z)) {
				PC += d;
				tstates += 5;
			}
			break;
		case 0x30://JR NC
			if(!(F & FLAG_C)) {
				PC += d;
				tstates += 5;
			}
			break;
		case 0x18://JR
			PC += d;
			break;
		case 0x28://JR Z
			if(F & FLAG_Z) {
				PC += d;
				tstates += 5;
			}
			break;
		case 0x38://JR C
			if(F & FLAG_C) {
				PC += d;
				tstates += 5;
			}
			break;
		default:
			ERROR(code); break;
		}
	}
	else if(((code &0x07) == 0x00) || ((code &0x07) == 0x02) || ((code &0x07) == 0x04)) {
		if((code &0x07) == 0x00) {//for RET
			dl = mem_read(SP++);
			dh = mem_read(SP++);
		}
		else {//for JP & CALL
			dl = mem_read(PC++);
			dh = mem_read(PC++);
		}

		switch (code) {
		case 0xc0://RET NZ
		case 0xc4://CALL NZ
		case 0xc2://JP NZ
			if(!(F & FLAG_Z)) {
				if(!(code & 0x07)) tstates += 6;//for RET only
				if(code & 0x04) {//for CALL only
					tstates += 7;
					mem_write(--SP, PCH);
					mem_write(--SP, PCL);
				}
				PCL = dl;
				PCH = dh;
			}
			break;
		case 0xd0://RET NC
		case 0xd4://CALL NC
		case 0xd2://JP NC
			if(!(F & FLAG_C)) {
				if(!(code & 0x07)) tstates += 6;//for RET only
				if(code & 0x04) {//for CALL only
					tstates += 7;
					mem_write(--SP, PCH);
					mem_write(--SP, PCL);
				}
				PCL = dl;
				PCH = dh;
			}
			break;
		case 0xe0://RET PO
		case 0xe4://CALL PO
		case 0xe2://JP PO
			if(!(F & FLAG_P)) {
				if(!(code & 0x07)) tstates += 6;//for RET only
				if(code & 0x04) {//for CALL only
					tstates += 7;
					mem_write(--SP, PCH);
					mem_write(--SP, PCL);
				}
				PCL = dl;
				PCH = dh;
			}
			break;
		case 0xf0://RET P
		case 0xf4://CALL P
		case 0xf2://JP P
			if(!(F & FLAG_S)) {
				if(!(code & 0x07)) tstates += 6;//for RET only
				if(code & 0x04) {//for CALL only
					tstates += 7;
					mem_write(--SP, PCH);
					mem_write(--SP, PCL);
				}
				PCL = dl;
				PCH = dh;
			}
			break;
		case 0xc8://RET Z
		case 0xcc://CALL Z
		case 0xca://JP Z
			if(F & FLAG_Z) {
				if(!(code & 0x07)) tstates += 6;//for RET only
				if(code & 0x04) {//for CALL only
					tstates += 7;
					mem_write(--SP, PCH);
					mem_write(--SP, PCL);
				}
				PCL = dl;
				PCH = dh;
			}
			break;
		case 0xd8://RET C
		case 0xdc://CALL C
		case 0xda://JP C
			if(F & FLAG_C) {
				if(!(code & 0x07)) tstates += 6;//for RET only
				if(code & 0x04) {//for CALL only
					tstates += 7;
					mem_write(--SP, PCH);
					mem_write(--SP, PCL);
				}
				PCL = dl;
				PCH = dh;
			}
			break;
		case 0xe8://RET PE
		case 0xec://CALL PE
		case 0xea://JP PE
			if(F & FLAG_P) {
				if(!(code & 0x07)) tstates += 6;//for RET only
				if(code & 0x04) {//for CALL only
					tstates += 7;
					mem_write(--SP, PCH);
					mem_write(--SP, PCL);
				}
				PCL = dl;
				PCH = dh;
			}
			break;
		case 0xf8://RET M
		case 0xfc://CALL M
		case 0xfa://JP M
			if(F & FLAG_S) {
				if(!(code & 0x07)) tstates += 6;//for RET only
				if(code & 0x04) {//for CALL only
					tstates += 7;
					mem_write(--SP, PCH);
					mem_write(--SP, PCL);
				}
				PCL = dl;
				PCH = dh;
			}
			break;
		default:
			ERROR(code); break;
		}

	}
	else if(code == 0xc9) {//RET
		PCL = mem_read(SP++);
		PCH = mem_read(SP++);
	}
	else if(code == 0xcd) {//CALL
		dl = mem_read(PC++);
		dh = mem_read(PC++);
		mem_write(--SP, PCH);
		mem_write(--SP, PCL);
		PCL = dl; PCH = dh;
	}
	else if(code == 0xe9) {//JP (HL)
		PC = HLIXIY_REG;
	}
	else if(code == 0xc3) {//JP
		dl = mem_read(PC++);
		dh = mem_read(PC++);
		PCL = dl; PCH = dh;
	}
	else if((code & 0xc7) == 0xc7) {//RST
		mem_write(--SP, PCH);
		mem_write(--SP, PCL);
		PCH = 0;
		PCL = code & 0x38;
	}
	else {
		ERROR(code);
	}

	return tstates;
}

uint8_t STK(uint8_t code) {
	uint8_t tstates = optstates[code];
	switch (code) {
	case 0xc1://POP BC
		C = mem_read(SP++);
		B = mem_read(SP++);
		break;
	case 0xd1://POP DE
		E = mem_read(SP++);
		D = mem_read(SP++);
		break;
	case 0xe1://POP HL
		HLIXIY_REGL = mem_read(SP++);
		HLIXIY_REGH = mem_read(SP++);
		break;
	case 0xf1://POP AF
		//byte order ???
		F = mem_read(SP++);
		A = mem_read(SP++);
		break;
	case 0xc5://PUSH BC
		mem_write(--SP, B);
		mem_write(--SP, C);
		break;
	case 0xd5://PUSH DE
		mem_write(--SP, D);
		mem_write(--SP, E);
		break;
	case 0xe5://PUSH HL
		mem_write(--SP, HLIXIY_REGH);
		mem_write(--SP, HLIXIY_REGL);
		break;
	case 0xf5://PUSH AF
		//byte order ???
		mem_write(--SP, A);
		mem_write(--SP, F);
		break;
	default:
		ERROR(code); break;
	}

	return tstates;
}

uint8_t SFT(uint8_t code) {
	uint8_t tstates = optstates[code];
	uint8_t tmp;

	if(IS_CB_PREFIX) {
		tstates = 4;
		uint8_t *regptr = (uint8_t*)(&regs) + (code & 0x07);
		uint8_t tmpres;

		if((regptr == &F) || IS_DDFD_PREFIX)
			tmpres = mem_read(HLIXIY_REG+regs.ixiyshift);
		else
			tmpres = *regptr;
		if(IS_DDFD_PREFIX)
			tstates += 15;

		else if(regptr == &F) {
			regptr = &tmpres;
			if(!IS_DDFD_PREFIX)
				tstates += 7;
		}

		switch (code & 0x38) {
		case 0x00: //RLC
			tmpres = (tmpres << 1) | (tmpres >> 7);
			F = (tmpres & FLAG_C) | sz53p_table[tmpres];
			break;
		case 0x08: //RRC
			F = tmpres & FLAG_C;
			tmpres = (tmpres >> 1) | (tmpres << 7);
			F |= sz53p_table[tmpres];
			break;
		case 0x10: //RL
			tmp = tmpres;
			tmpres = (tmpres << 1) | (F & FLAG_C);
			F = (tmp >> 7) | sz53p_table[tmpres];
			break;
		case 0x18: //RR
			tmp = tmpres;
			tmpres = (tmpres >> 1) | (F << 7);
			F = (tmp & FLAG_C) | sz53p_table[tmpres];
			break;
		case 0x20: //SLA
			F = tmpres >> 7;
			tmpres <<= 1;
			F |= sz53p_table[tmpres];
			break;
		case 0x28: //SRA
			F = tmpres & FLAG_C;
			tmpres = (tmpres & 0x80) | (tmpres >> 1);
			F |= sz53p_table[tmpres];
			break;
		case 0x30: //SLL
			F = tmpres >> 7;
			tmpres = ( tmpres << 1 ) | 0x01;
			F |= sz53p_table[tmpres];
			break;
		case 0x38: //SRL
			F = tmpres & FLAG_C;
			tmpres >>= 1;
			F |= sz53p_table[tmpres];
			break;
		default:
			ERROR(code); break;
		}

		if((regptr == &tmpres)  || IS_DDFD_PREFIX) {
			mem_write(HLIXIY_REG+regs.ixiyshift, tmpres);
			regs.ixiyshift = 0;
		}
		else
			*regptr = tmpres;
	}
	else {
		switch (code) {
		case 0x07://RLCA
			A = (A << 1) | (A >> 7);
			F = (F & (FLAG_P | FLAG_Z | FLAG_S)) | (A & (FLAG_C | FLAG_3 | FLAG_5));
			break;
		case 0x0f://RRCA
		      F = (F & (FLAG_P | FLAG_Z | FLAG_S)) | (A & FLAG_C);
		      A = (A >> 1) | (A << 7);
		      F |= (A & (FLAG_3 | FLAG_5));
			break;
		case 0x17://RLA
			tmp = A;
			A = (A << 1) | (F & FLAG_C);
			F = (F & (FLAG_P | FLAG_Z | FLAG_S)) | (A & (FLAG_3 | FLAG_5)) | (tmp >> 7);
			break;
		case 0x1f://RRA
			tmp = A;
			A = (A >> 1) | (F << 7);
			F = (F & (FLAG_P | FLAG_Z | FLAG_S)) | (A & (FLAG_3 | FLAG_5)) | (tmp & FLAG_C);
			break;
		default:
			ERROR(code); break;
		}
	}

	return tstates;
}

uint8_t EDSF(uint8_t code) {
	uint8_t tstates;
	if(code < 0x80)
		tstates = edoptstates[code-0x40];
	else
		tstates = edoptstates[code-0x60];

	uint8_t tmp;
	switch (code) {
	case 0x67://RRD
	case 0x77:
		tmp = mem_read(HL);
		mem_write(HL, (A << 4) | (tmp >> 4));
		A = (A & 0xf0) | (tmp & 0x0f);
		F = (F & FLAG_C) | sz53p_table[A];
		break;
	case 0x6f://RLD
	case 0x7f:
		tmp = mem_read(HL);
		mem_write(HL, (tmp << 4) | (A & 0x0f));
		A = (A & 0xf0) | (tmp >> 4);
		F = (F & FLAG_C) | sz53p_table[A];
		break;
	default:
		ERROR(code); break;
	}

	switch (code) {
	default:
		ERROR(code); break;
	}

	return tstates;
}

uint8_t BIT(uint8_t code) {
	uint8_t tstates = 4;

	uint8_t bitmask = 0x01 << ((code & 0x38) >> 3);
	uint8_t *regptr = (uint8_t*)(&regs) + (code & 0x07);
	uint8_t tmpres;

	if((regptr == &F) || IS_DDFD_PREFIX)
		tmpres = mem_read(HLIXIY_REG+regs.ixiyshift);
	else
		tmpres = *regptr;
	if(IS_DDFD_PREFIX)
		tstates += 12;

	else if(regptr == &F) {
		regptr = &tmpres;
		if(!IS_DDFD_PREFIX)
			tstates += 4;
	}

	switch (code & 0xc0) {
	case 0x40: //BIT
		//for (HL) operations flags 3 & 5 must be set in other way - not realized
		F = (F & FLAG_C) | FLAG_H | (tmpres & (FLAG_3 | FLAG_5));
		if(!(tmpres & bitmask)) F |= FLAG_P | FLAG_Z;
		if((bitmask == 0x80) && (tmpres & 0x80)) F |= FLAG_S;
		break;
	case 0x80: //RES
		tmpres &= ~bitmask; if(regptr == &tmpres) tstates += 3; break;
	case 0xc0: //SET
		tmpres |= bitmask; if(regptr == &tmpres) tstates += 3; break;
	default:
		ERROR(code); break;
	}

	if((regptr == &tmpres)  || IS_DDFD_PREFIX) {
		mem_write(HLIXIY_REG+regs.ixiyshift, tmpres);
		regs.ixiyshift = 0;
	}
	else
		*regptr = tmpres;

	return tstates;
}

uint8_t EX_(uint8_t code) {
	uint8_t tstates = optstates[code];

//	if(IS_DDFD_PREFIX && (code != 0xe3))
//		return NONI(code);

	uint16_t tmp;
	switch (code) {
	case 0x08: //EX AF
		tmp = FA; FA = FA_; FA_ = tmp; break;
	case 0xD9: //EXX
		tmp = BC; BC = BC_; BC_ = tmp;
		tmp = DE; DE = DE_; DE_ = tmp;
		tmp = HL; HL = HL_; HL_ = tmp;
		break;
	case 0xe3: //EX (SP), HL
		tmp = mem_read(SP); mem_write(SP, HLIXIY_REGL); ; HLIXIY_REGL = tmp;
		tmp = mem_read(SP+1); mem_write(SP+1, HLIXIY_REGH); ; HLIXIY_REGH = tmp;
		break;
	case 0xeb: //EX DE, HL
		tmp = DE; DE = HL; HL = tmp; break;
	default:
		ERROR(code); break;
	}

	return tstates;
}

uint8_t IO_(uint8_t code) {
	uint8_t tstates = optstates[code];

		switch (code) {
		case 0xd3://OUT (*),A
			port_out(((uint16_t)A << 8) | mem_read(PC++), A); break;
		case 0xdb://IN A,(*)
			A = port_in(((uint16_t)A << 8) | mem_read(PC++)); break;
		default:
			ERROR(code); break;
		}

	return tstates;
}

uint8_t EDIO(uint8_t code) {
	uint8_t tstates;
	if(code < 0x80)
		tstates = edoptstates[code-0x40];
	else
		tstates = edoptstates[code-0x60];

	uint8_t tmp1, tmp2;
	switch (code) {
	case 0x40://IN
		B = port_in(BC);
		F = ( F & FLAG_C) | sz53p_table[B]; break;
	case 0x48://IN
		C = port_in(BC);
		F = ( F & FLAG_C) | sz53p_table[C]; break;
	case 0x50://IN
		D = port_in(BC);
		F = ( F & FLAG_C) | sz53p_table[D]; break;
	case 0x58://IN
		E = port_in(BC);
		F = ( F & FLAG_C) | sz53p_table[E]; break;
	case 0x60://IN
		H = port_in(BC);
		F = ( F & FLAG_C) | sz53p_table[H]; break;
	case 0x68://IN
		L = port_in(BC);
		F = ( F & FLAG_C) | sz53p_table[L]; break;
	case 0x70://IN
		tmp1 = port_in(BC);
		F = ( F & FLAG_C) | sz53p_table[tmp1]; break;
	case 0x78://IN
		A = port_in(BC);
		F = ( F & FLAG_C) | sz53p_table[A]; break;
	case 0x41://OUT
		port_out(BC, B); break;
	case 0x49://OUT
		port_out(BC, C); break;
	case 0x51://OUT
		port_out(BC, D); break;
	case 0x59://OUT
		port_out(BC, E); break;
	case 0x61://OUT
		port_out(BC, H); break;
	case 0x69://OUT
		port_out(BC, L); break;
	case 0x71://OUT
		port_out(BC, 0); break;
	case 0x79://OUT
		port_out(BC, A); break;
	case 0xa2://INI
	case 0xb2://INIR
		tmp1 = port_in(BC);
		mem_write(HL, tmp1);
		B--;HL++;
        tmp2 = tmp1 + C + 1;
		F = ((tmp1 & 0x80) ? FLAG_N : 0) | ((tmp2 < tmp1) ? FLAG_H | FLAG_C : 0) |
		    ((sz53p_table[(tmp2 & 0x07) ^ B] & FLAG_P) ? FLAG_P : 0) | (sz53p_table[B] & ~FLAG_P);
		if(((code & 0xf0) == 0xb0) && B) PC -= 2;
		break;
	case 0xa3://OUTI
	case 0xb3://OUTIR
		tmp1 = mem_read(HL);
		B--; HL++;
		port_out(BC, tmp1);
		tmp2 = tmp1 + L;
		F = ((tmp1 & 0x80) ? FLAG_N : 0) | ((tmp2 < tmp1 ) ? FLAG_H | FLAG_C : 0) |
			((sz53p_table[(tmp2 & 0x07) ^ B] & FLAG_P) ? FLAG_P : 0) | (sz53p_table[B] & ~FLAG_P);
		if(((code & 0xf0) == 0xb0) && B) PC -= 2;
		break;
	case 0xaa://IND
	case 0xba://INDR
		tmp1 = port_in(BC);
		mem_write(HL, tmp1);
        B--; HL--;
        tmp2 = tmp1 + C - 1;
		F = ((tmp1 & 0x80) ? FLAG_N : 0) | ((tmp2 < tmp1) ? FLAG_H | FLAG_C : 0) |
		    ((sz53p_table[(tmp2 & 0x07) ^ B] & FLAG_P) ? FLAG_P : 0) | (sz53p_table[B] & ~FLAG_P);
		if(((code & 0xf0) == 0xb0) && B) PC -= 2;
		break;
	case 0xab://OUTD
	case 0xbb://OUTDR
		tmp1 = mem_read(HL);
		B--; HL--;
		port_out(BC, tmp1);
		tmp2 = tmp1 + L;
		F = ((tmp1 & 0x80) ? FLAG_N : 0) | ((tmp2 < tmp1 ) ? FLAG_H | FLAG_C : 0) |
			((sz53p_table[(tmp2 & 0x07) ^ B] & FLAG_P) ? FLAG_P : 0) | (sz53p_table[B] & ~FLAG_P);
		if(((code & 0xf0) == 0xb0) && B) PC -= 2;
		break;
	default:
		ERROR(code); break;
	}

	switch (code) {
	default:
		ERROR(code); break;
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

