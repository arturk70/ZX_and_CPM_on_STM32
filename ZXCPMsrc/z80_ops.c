/*
 * z80_ops.c
 *
 *  Created on: Oct 18, 2019
 *      Author: artur
 */

#include "z80_ops.h"

const uint8_t optstates[256] = {
		  4,  10,   7,   6,   4,   4,   7,   4,   4,  11,   7,   6,   4,   4,   7,   4, //0x00-0x0f
		  8,  10,   7,   6,   4,   4,   7,   4,  12,  11,   7,   6,   4,   4,   7,   4, //0x10-0x1f
		  7,  10,  16,   6,   4,   4,   7,   4,   7,  11,  16,   6,   4,   4,   7,   4, //0x20-0x2f
		  7,  10,  13,   6,  11,  11,  10,   4,   7,  11,  13,   6,   4,   4,   7,   4, //0x30-0x3f
		  4,   4,   4,   4,   4,   4,   7,   4,   4,   4,   4,   4,   4,   4,   7,   4, //0x40-0x4f
		  4,   4,   4,   4,   4,   4,   7,   4,   4,   4,   4,   4,   4,   4,   7,   4, //0x50-0x5f
		  4,   4,   4,   4,   4,   4,   7,   4,   4,   4,   4,   4,   4,   4,   7,   4, //0x60-0x6f
		  7,   7,   7,   7,   7,   7,   4,   7,   4,   4,   4,   4,   4,   4,   7,   4, //0x70-0x7f
		  4,   4,   4,   4,   4,   4,   7,   4,   4,   4,   4,   4,   4,   4,   7,   4, //0x80-0x8f
		  4,   4,   4,   4,   4,   4,   7,   4,   4,   4,   4,   4,   4,   4,   7,   4, //0x90-0x9f
		  4,   4,   4,   4,   4,   4,   7,   4,   4,   4,   4,   4,   4,   4,   7,   4, //0xa0-0xaf
		  4,   4,   4,   4,   4,   4,   7,   4,   4,   4,   4,   4,   4,   4,   7,   4, //0xb0-0xbf
		  5,  10,  10,  10,  10,  11,   7,  11,   5,  10,  10,   4,  10,  17,   7,  11, //0xc0-0xcf
		  5,  10,  10,  11,  10,  11,   7,  11,   5,   4,  10,  11,  10,   4,   7,  11, //0xd0-0xdf
		  5,  10,  10,  19,  10,  11,   7,  11,   5,   4,  10,   4,  10,   4,   7,  11, //0xe0-0xef
		  5,  10,  10,   4,  10,  11,   7,  11,   5,   6,  10,   4,  10,   4,   7,  11  //0xf0-0xff
};

const z80cmd_t z80ops[256] = {
		NOP, LDx, LDm, ICx, IC8, DC8, LDn, SFT, EX_, ALx, LDm, DCx, IC8, DC8, LDn, SFT, //0x00-0x0f
		JR_, LDx, LDm, ICx, IC8, DC8, LDn, SFT, JR_, ALx, LDm, DCx, IC8, DC8, LDn, SFT, //0x10-0x1f
		JR_, LDx, LDm, ICx, IC8, DC8, LDn, DAA, JR_, ALx, LDm, DCx, IC8, DC8, LDn, CPL, //0x20-0x2f
		JR_, LDx, LDm, ICx, IC8, DC8, LDn, SCF, JR_, ALx, LDm, DCx, IC8, DC8, LDn, CCF, //0x30-0x3f
		LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, //0x40-0x4f
		LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, //0x50-0x5f
		LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, //0x60-0x6f
		LD_, LD_, LD_, LD_, LD_, LD_, HLT, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, LD_, //0x70-0x7f
		ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, //0x80-0x8f
		ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, //0x90-0x9f
		ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, //0xa0-0xaf
		ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, ALU, //0xb0-0xbf
		JPc, POP, JPc, JMP, JPc, PSH, ALn, RST, JPc, JMP, JPc,   0, JPc, JMP, ALn, RST, //0xc0-0xcf
		JPc, POP, JPc, IO_, JPc, PSH, ALn, RST, JPc, EX_, JPc, IO_, JPc,   0, ALn, RST, //0xd0-0xdf
		JPc, POP, JPc, EX_, JPc, PSH, ALn, RST, JPc, JMP, JPc, EX_, JPc,   0, ALn, RST, //0xe0-0xef
		JPc, POP, JPc, DI_, JPc, PSH, ALn, RST, JPc, LDs, JPc, EI_, JPc,   0, ALn, RST  //0xf0-0xff
};

const uint8_t edoptstates[256] = {
		   8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,   8,    8,  //0x00-0x0f
		   8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,   8,    8,  //0x10-0x1f
		   8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,   8,    8,  //0x20-0x2f
		   8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,   8,    8,  //0x30-0x3f
		  12,   12,   15,   20,    8,   14,    8,    9,   12,   12,   15,   20,    8,   14,   8,    9,  //0x40-0x4f
		  12,   12,   15,   20,    8,   14,    8,    9,   12,   12,   15,   20,    8,   14,   8,    9,  //0x50-0x5f
		  12,   12,   15,   20,    8,   14,    8,   18,   12,   12,   15,   20,    8,   14,   8,   18,  //0x60-0x6f
		  12,   12,   15,   20,    8,   14,    8,    8,   12,   12,   15,   20,    8,   14,   8,    8,  //0x70-0x7f
		   8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,   8,    8,  //0x80-0x8f
		   8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,   8,    8,  //0x90-0x9f
		  16,   16,   16,   16,    8,    8,    8,    8,   16,   16,   16,   16,    8,    8,   8,    8,  //0xa0-0xaf
		  16,   16,   16,   16,    8,    8,    8,    8,   16,   16,   16,   16,    8,    8,   8,    8,  //0xb0-0xbf
		   8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,   8,    8,  //0xc0-0xcf
		   8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,   8,    8,  //0xd0-0xdf
		   8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,   8,    8,  //0xe0-0xef
		   8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,    8,   8,    8   //0xf0-0xff
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

void NOP(register uint32_t code) {
}

void HLT(register uint32_t code) {
	z80_state.halted = 1;
}

void DI_(register uint32_t code) {
	IFF1 = IFF2 = 0;
}

void EI_(register uint32_t code) {
	IFF1 = IFF2 = 1; z80_state.int_blocked = 1;
}

void SCF(register uint32_t code) {
	F = (F & (FLAG_P | FLAG_Z | FLAG_S)) | (A & (FLAG_3 | FLAG_5)) | FLAG_C;
}

void CCF(register uint32_t code) {
	F = (F & (FLAG_P | FLAG_Z | FLAG_S)) | ((F & FLAG_C) ? FLAG_H : FLAG_C) | (A & (FLAG_3 | FLAG_5));
}

void LDn(register uint32_t code) {
	register int8_t tmp;
	register int8_t ixiyshift = 0;

	tmp = mem_read(PC++);

	switch (code >> 3) {
	case 0x00://LD  B,*
		B = tmp;
		break;
	case 0x02://LD  D,*
		D = tmp;
		break;
	case 0x04://LD  H,*
		HLIXIY_REGH = tmp;
		break;
	case 0x06://LD  (HL),*
		if(hlixiyptr != &HL) {
			ixiyshift = tmp;
			tmp = mem_read(PC++);
		}
		mem_write(HLIXIY_REG+ixiyshift, tmp);
		break;
	case 0x01://LD  C,*
		C = tmp;
		break;
	case 0x03://LD  E,*
		E = tmp;
		break;
	case 0x05://LD  L,*
		HLIXIY_REGL = tmp;
		break;
	case 0x07://LD  A,*
		A = tmp;
		break;
	}
}

void LDx(register uint32_t code) {
	register uint16_t tmp;
	tmp = mem_read(PC++);
	tmp |= mem_read(PC++) << 8;
	switch (code >> 4) {
	case 0x00://LD BC,**
		BC = tmp;
		break;
	case 0x01://LD DE,**
		DE = tmp;
		break;
	case 0x02://LD HL,**
		HLIXIY_REG = tmp;
		break;
	case 0x03://LD SP,**
		SP = tmp;
		break;
	}
}

void LDm(register uint32_t code) {
	register uint16_t addr;
	register uint8_t tmp, hcode;
	hcode = code & 0xf0;
	if(hcode  > 0x10) { // (**) ops: LD (**),HL ; LD (**),A ; LD HL,(**) ; LD A,(**)
		addr = mem_read(PC++);
		addr |= mem_read(PC++) << 8;
	}
	else {
		if(code & 0xf0)
			addr = DE;
		else
			addr = BC;
	}

	if(hcode == 0x20) //for HL
		tmp = HLIXIY_REGL;
	else
		tmp = A;

	if(code & 0x08) //read
		tmp = mem_read(addr);
	else //write
		mem_write(addr, tmp);

	if(hcode == 0x20) {//for HL
		if(code & 0x08) {//read
			HLIXIY_REGL = tmp;
			HLIXIY_REGH = mem_read(++addr);
		}
		else {//write
			mem_write(++addr, HLIXIY_REGH);
		}
	}
	else
		A = tmp;
}

void LDs(register uint32_t code) {//LD SP, HL
	SP = HLIXIY_REG;
}

void LD_(register uint32_t code) {
	register int8_t ixiyshift = 0;
	register uint8_t tmp;

	if(hlixiyptr != &HL)
		ixiyshift = mem_read(PC++);

	register uint8_t srcnum = code & 0x07;
	register uint8_t dstnum = (code & 0x38) >> 3;

	if(srcnum == 0x07) {//A
		tmp = A;
	} else if(srcnum == 0x06) {//(HL)
		tmp = mem_read(HLIXIY_REG + ixiyshift);
	} else if(srcnum == 0x04) {//H
		tmp = (code == 0x74) ? H : HLIXIY_REGH;
	} else if(srcnum == 0x05) {//L
		tmp = (code == 0x75) ? L : HLIXIY_REGL;
	}else {//other reg
		tmp = *(regs + (srcnum^1));
	}

	if(dstnum == 0x07) {//A
		A = tmp;
	} else if(dstnum == 0x06) {//(HL)
		mem_write(HLIXIY_REG + ixiyshift, tmp);
	} else if(dstnum == 0x04) {//H
		if(code == 0x66)
			H = tmp;
		else
			HLIXIY_REGH = tmp;
	} else if(dstnum == 0x05) {//L
		if(code == 0x6e)
			L = tmp;
		else
			HLIXIY_REGL = tmp;
	} else {//other reg
		*(regs + (dstnum^1)) = tmp;
	}
}

void ICx(register uint32_t code) {
	switch (code >> 4) {
	case 0x00://INC BC
		BC++;
		break;
	case 0x01://INC DE
		DE++;
		break;
	case 0x02://INC HL
		HLIXIY_REG++;
		break;
	case 0x03://INC SP
		SP++;
		break;
	}
}

void IC8(register uint32_t code) {
	register uint8_t res;
	switch (code >> 3) {
	case 0x00://INC B
		res = ++B;
		break;
	case 0x02://INC D
		res = ++D;
		break;
	case 0x04://INC H
		res = ++HLIXIY_REGH;
		break;
	case 0x06://INC (HL)
	{
		register int8_t ixiyshift = 0;
		if(hlixiyptr != &HL)
			ixiyshift = mem_read(PC++);
		register uint8_t tmp = mem_read(HLIXIY_REG + ixiyshift);
		res = ++tmp;
		mem_write(HLIXIY_REG + ixiyshift, tmp);
	}
	break;
	case 0x01://INC C
		res = ++C;
		break;
	case 0x03://INC E
		res = ++E;
		break;
	case 0x05://INC L
		res = ++HLIXIY_REGL;
		break;
	case 0x07://INC A
		res = ++A;
		break;
	}

	F = (F & FLAG_C) | ((res == 0x80) ? FLAG_V : 0) | ((res & 0x0f) ? 0 : FLAG_H) | (sz53p_table[res] & ~FLAG_P);
}

void DCx(register uint32_t code) {
	switch (code >> 4) {
	case 0x00://DEC BC
		BC--;
		break;
	case 0x01://DEC DE
		DE--;
		break;
	case 0x02://DEC HL
		HLIXIY_REG--;
		break;
	case 0x03://DEC SP
		SP--;
		break;
	}
}

void DC8(register uint32_t code) {
	register uint8_t res;
	switch (code >> 3) {
	case 0x00://DEC B
		res = --B;
		break;
	case 0x02://DEC D
		res = --D;
		break;
	case 0x04://DEC H
		res = --HLIXIY_REGH;
		break;
	case 0x06://DEC (HL)
	{
		register int8_t ixiyshift = 0;
		if(hlixiyptr != &HL)
			ixiyshift = mem_read(PC++);
		register uint8_t tmp = mem_read(HLIXIY_REG + ixiyshift);
		res = --tmp;
		mem_write(HLIXIY_REG + ixiyshift, tmp);
	}
	break;
	case 0x01://DEC C
		res = --C;
		break;
	case 0x03://DEC E
		res = --E;
		break;
	case 0x05://DEC L
		res = --HLIXIY_REGL;
		break;
	case 0x07://DEC A
		res = --A;
		break;
	}

	F = (F & FLAG_C) | (((res & 0x0f) == 0x0f) ? FLAG_H : 0) | FLAG_N | ((res == 0x7f) ? FLAG_V : 0) | (sz53p_table[res] & ~FLAG_P);
}

void CPL(register uint32_t code) {
	A = ~A;
	F = (F & (FLAG_C | FLAG_P | FLAG_Z | FLAG_S)) | (A & (FLAG_3 | FLAG_5)) | FLAG_N | FLAG_H;
}

void ALx(register uint32_t code) {
	register uint32_t tmp;
	register uint8_t hreg;

	switch (code >> 4) {
	case 0x00://ADD HL,BC
		tmp = HLIXIY_REG + BC;
		hreg = B;
		break;
	case 0x01://ADD HL,DE
		tmp = HLIXIY_REG + DE;
		hreg = D;
		break;
	case 0x02://ADD HL,HL
		tmp = HLIXIY_REG + HLIXIY_REG;
		hreg = HLIXIY_REGH;
		break;
	case 0x03://ADD HL,SP
		tmp = HLIXIY_REG + SP;
		hreg = SPH;
		break;
	}

	F = (F & (FLAG_V | FLAG_Z | FLAG_S)) | CALC_C(tmp >> 8) | ((tmp >> 8) & (FLAG_3 | FLAG_5)) | CALC_ADD_H(HLIXIY_REGH, hreg, tmp >> 8);
	HLIXIY_REG = tmp;
}

uint8_t calc_F(register uint8_t src1, register uint8_t src2, register int16_t res, register uint8_t op) {
	register uint8_t newF = CALC_C(res);
	if(op == 0) {//ADD
		newF |= CALC_ADD_H(src1, src2, res) | CALC_ADD_V(src1, src2, res) | (sz53p_table[(res & 0x00ff)] & ~FLAG_P);
	}
	else {
		newF |= FLAG_N | CALC_SUB_H(src1, src2, res) | CALC_SUB_V(src1, src2, res);
		if(op == 1) {//SUB
			newF |= (sz53p_table[(res & 0x00ff)] & ~FLAG_P);
		}
		else {//CP
			newF |= (src2 & (FLAG_3 | FLAG_5)) | (res & FLAG_S) | CALC_Z(res);
		}
	}

	return newF;
}

void ALn(register uint32_t code) {
	register uint8_t src = mem_read(PC++);
	register int16_t res;
	switch ((code & 0x38) >> 3) {
	case 0x00://ADD A,*
		res = A + src;
		F = calc_F(A, src, res, 0);
		A = res;
		break;
	case 0x02://SUB A,*
		res = A - src;
		F = calc_F(A, src, res, 1);
		A = res;
		break;
	case 0x04://AND *
		A &= src;
		F = FLAG_H | sz53p_table[A];
		break;
	case 0x06://OR *
		A |= src;
		F = sz53p_table[A];
		break;
	case 0x01://ADC A,*
		res = A + src + (F & FLAG_C);
		F = calc_F(A, src, res, 0);
		A = res;
		break;
	case 0x03://SBC A,*
		res = A - src - (F & FLAG_C);
		F = calc_F(A, src, res, 1);
		A = res;
		break;
	case 0x05://XOR *
		A ^= src;
		F = sz53p_table[A];
		break;
	case 0x07://CP *
		res = A - src;
		F = calc_F(A, src, res, 2);
		break;
	}
}

void ALU(register uint32_t code) {
	register int8_t ixiyshift = 0;
	register uint8_t src;
	register int16_t res;

	switch(code & 0x07) {
	case 0x07://A
		src = A; break;
	case 0x06://(HL)
		if(hlixiyptr != &HL) {
			ixiyshift = mem_read(PC++);
			z80_tstates+=8;
		}
		src = mem_read(HLIXIY_REG + ixiyshift);
		break;
	case 0x05://L
		src = HLIXIY_REGL; break;
	case 0x04://H
		src = HLIXIY_REGH; break;
	default://other registers
		src = *(regs + ((code & 0x07)^1));
	}

	if((code & 0xf0) == 0x80) {
		if(code & 0x08) {//ADC
			res = A + src + (F & FLAG_C);
			F = calc_F(A, src, res, 0);
			A = res;
		}
		else {//ADD
			res = A + src;
			F = calc_F(A, src, res, 0);
			A = res;
		}
	}
	else if((code & 0xf0) == 0x90) {
		if(code & 0x08) {//SBC
			res = A - src - (F & FLAG_C);
			F = calc_F(A, src, res, 1);
			A = res;
		}
		else {//SUB
			res = A - src;
			F = calc_F(A, src, res, 1);
			A = res;
		}
	}
	else if((code & 0xf0) == 0xa0) {
		if(code & 0x08) {//XOR
			A ^= src;
			F = sz53p_table[A];
		}
		else {//AND
			A &= src;
			F = FLAG_H | sz53p_table[A];
		}
	}
	else if((code & 0xf0) == 0xb0) {
		if(code & 0x08) {//CP
			res = A - src;
			F = calc_F(A, src, res, 2);
		}
		else {//OR
			A |= src;
			F = sz53p_table[A];
		}
	}
}

void JR_(register uint32_t code) {
	register int8_t d = mem_read(PC++);
	register uint8_t cond = 0;

	switch (code >> 3) {
	case 0x02://DJNZ
		cond = --B;
		break;
	case 0x04://JR NZ
		cond = !(F & FLAG_Z);
		break;
	case 0x06://JR NC
		cond = !(F & FLAG_C);
		break;
	case 0x03://JR
		cond = 0xff;
		z80_tstates -= 5;
		break;
	case 0x05://JR Z
		cond = (F & FLAG_Z);
		break;
	case 0x07://JR C
		cond = (F & FLAG_C);
		break;
	}

	if(cond) {
		PC += d;
		z80_tstates += 5;
	}
}

void RST(register uint32_t code) {
	mem_write(--SP, PCH);
	mem_write(--SP, PCL);
	PCH = 0;
	PCL = code & 0x38;
}

void JPc(register uint32_t code) {
	register uint8_t dh, dl;
	register uint8_t cond = 0;

	switch ((code & 0x38) >> 3) {
	case 0x00://NZ
		cond = !(F & FLAG_Z);
		break;
	case 0x02://NC
		cond = !(F & FLAG_C);
		break;
	case 0x04://PO
		cond = !(F & FLAG_P);
		break;
	case 0x06://P
		cond = !(F & FLAG_S);
		break;
	case 0x01://Z
		cond = F & FLAG_Z;
		break;
	case 0x03://C
		cond = F & FLAG_C;
		break;
	case 0x05://PE
		cond = F & FLAG_P;
		break;
	case 0x07://M
		cond = F & FLAG_S;
		break;
	}

	if((code &0x07) != 0x00) {//for JP & CALL
		dl = mem_read(PC++);
		dh = mem_read(PC++);
	}

	if(cond) {
		if((code & 0x07) == 0x00) {//for RET
			z80_tstates += 6;
			dl = mem_read(SP++);
			dh = mem_read(SP++);
		}
		else if(code & 0x04) {//for CALL only
			z80_tstates += 7;
			mem_write(--SP, PCH);
			mem_write(--SP, PCL);
		}

		PCL = dl;
		PCH = dh;
	}
}

void JMP(register uint32_t code) {
	register uint8_t dh, dl;

	if(code == 0xc9) {//RET
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
}

void POP(register uint32_t code) {
	register uint16_t tmp;
	tmp = mem_read(SP++);
	tmp |= mem_read(SP++) << 8;

	switch (code >> 4) {
	case 0x0c://POP BC
		BC = tmp;
		break;
	case 0x0d://POP DE
		DE = tmp;
		break;
	case 0x0e://POP HL
		HLIXIY_REG = tmp;
		break;
	case 0x0f://POP AF
		AF = tmp;
		break;
	}
}

void PSH(register uint32_t code) {
	register uint16_t tmp;

	switch (code >> 4) {
	case 0x0c://PUSH BC
		tmp = BC;
		break;
	case 0x0d://PUSH DE
		tmp = DE;
		break;
	case 0x0e://PUSH HL
		tmp = HLIXIY_REG;
		break;
	case 0x0f://PUSH AF
		tmp = AF;
		break;
	}

	mem_write(--SP, tmp >> 8);
	mem_write(--SP, tmp);
}

void SFT(register uint32_t code) {
	register uint8_t tmp;

	switch (code >> 3) {
	case 0x00://RLCA
		A = (A << 1) | (A >> 7);
		F = (F & (FLAG_P | FLAG_Z | FLAG_S)) | (A & (FLAG_C | FLAG_3 | FLAG_5));
		break;
	case 0x01://RRCA
		F = (F & (FLAG_P | FLAG_Z | FLAG_S)) | (A & FLAG_C);
		A = (A >> 1) | (A << 7);
		F |= (A & (FLAG_3 | FLAG_5));
		break;
	case 0x02://RLA
		tmp = A;
		A = (A << 1) | (F & FLAG_C);
		F = (F & (FLAG_P | FLAG_Z | FLAG_S)) | (A & (FLAG_3 | FLAG_5)) | (tmp >> 7);
		break;
	case 0x03://RRA
		tmp = A;
		A = (A >> 1) | (F << 7);
		F = (F & (FLAG_P | FLAG_Z | FLAG_S)) | (A & (FLAG_3 | FLAG_5)) | (tmp & FLAG_C);
		break;
	}
}

void EX_(register uint32_t code) {
	register uint16_t tmp;

	switch (code) {
	case 0x08: //EX AF
		tmp = AF; AF = AF_; AF_ = tmp; break;
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
	}
}

void IO_(register uint32_t code) {
	if(code == 0xd3) {//OUT (*),A
		port_out(((uint16_t)A << 8) | mem_read(PC++), A);
	}
	else {//0xdb IN A,(*)
		A = port_in(((uint16_t)A << 8) | mem_read(PC++));
	}
}

void DAA(register uint32_t code) {
	register uint8_t add = 0;
	register uint8_t carry = (F & FLAG_C);

	if((F & FLAG_H) || ((A & 0x0f) > 9))
		add = 0x06;

	if(carry || (A > 0x99)) {
		add |= 0x60;
		carry = FLAG_C;
	}
	else
		carry = 0;

	if(!(F & FLAG_N)) {
		F = (((A & 0x0F) > 0x09) ? FLAG_H : 0);
		A += add;
	} else {
		if(F & FLAG_H)
			F = FLAG_N | ((((A & 0x0F)) < 0x06) ? FLAG_H : 0);
		else
			F = FLAG_N;
		A -= add;
	}

	F |= carry | sz53p_table[A];
}

