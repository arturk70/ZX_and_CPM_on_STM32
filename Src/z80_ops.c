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
		JPc, POP, JPc, JMP, JPc, PSH, ALn, RST, JPc, JMP, JPc, PFX, JPc, JMP, ALn, RST, //0xc0-0xcf
		JPc, POP, JPc, IO_, JPc, PSH, ALn, RST, JPc, EX_, JPc, IO_, JPc, PFX, ALn, RST, //0xd0-0xdf
		JPc, POP, JPc, EX_, JPc, PSH, ALn, RST, JPc, JMP, JPc, EX_, JPc, PFX, ALn, RST, //0xe0-0xef
		JPc, POP, JPc, DI_, JPc, PSH, ALn, RST, JPc, LDs, JPc, EI_, JPc, PFX, ALn, RST  //0xf0-0xff
};

const uint8_t edoptstates[96] = {
		   8,    8,   11,   16,    4,   10,    4,    5,    8,    8,   11,   16,    4,   10,   4,    5,  //0x40-0x4f
		   8,    8,   11,   16,    4,   10,    4,    5,    8,    8,   11,   16,    4,   10,   4,    5,  //0x50-0x5f
		   8,    8,   11,   16,    4,   10,    4,   14,    8,    8,   11,   16,    4,   10,   4,   14,  //0x60-0x6f
		   8,    8,   11,   16,    4,   10,    4,   14,    8,    8,   11,   16,    4,   10,   4,   14,  //0x70-0x7f
		  12,   12,   12,   12,    4,    4,    4,    4,   12,   12,   12,   12,    4,    4,   4,    4,  //0xa0-0xaf
		  12,   12,   12,   12,    4,    4,    4,    4,   12,   12,   12,   12,    4,    4,   4,    4   //0xb0-0xbf
};

const z80cmd_t z80edops[96] = {
		EDIN, EDOU, SBCx, EDLD, NEG_, RETN, IM_0, LDIR, EDIN, EDOU, ADCx, EDLD, NEG_, RETI, IM_0, LDIR, //0x40-0x4f
		EDIN, EDOU, SBCx, EDLD, NEG_, RETN, IM_1, LDIR, EDIN, EDOU, ADCx, EDLD, NEG_, RETN, IM_2, LDIR, //0x50-0x5f
		EDIN, EDOU, SBCx, EDLD, NEG_, RETN, IM_0, EDSF, EDIN, EDOU, ADCx, EDLD, NEG_, RETN, IM_0, EDSF, //0x60-0x6f
		EDIN, EDOU, SBCx, EDLD, NEG_, RETN, IM_1, NONI, EDIN, EDOU, ADCx, EDLD, NEG_, RETN, IM_2, NONI, //0x70-0x7f
		LDBL, CPBL, IOBL, IOBL, NONI, NONI, NONI, NONI, LDBL, CPBL, IOBL, IOBL, NONI, NONI, NONI, NONI, //0xa0-0xaf
		LDBL, CPBL, IOBL, IOBL, NONI, NONI, NONI, NONI, LDBL, CPBL, IOBL, IOBL, NONI, NONI, NONI, NONI  //0xb0-0xbf
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

void NONI(register uint8_t code) {
	state.int_blocked = 1;
}

void NOP(register uint8_t code) {
}

void HLT(register uint8_t code) {
	state.halted = 1;
}

void DI_(register uint8_t code) {
	IFF1 = IFF2 = 0;
}

void EI_(register uint8_t code) {
	IFF1 = IFF2 = 1; state.int_blocked = 1;
}

void SCF(register uint8_t code) {
	F = (F & (FLAG_P | FLAG_Z | FLAG_S)) | (A & (FLAG_3 | FLAG_5)) | FLAG_C;
}

void CCF(register uint8_t code) {
	F = (F & (FLAG_P | FLAG_Z | FLAG_S)) | ((F & FLAG_C) ? FLAG_H : FLAG_C) | (A & (FLAG_3 | FLAG_5));
}

void RETN(register uint8_t code) {
		IFF1=IFF2;
		PCL = mem_read(SP++);
		PCH = mem_read(SP++);
}

void RETI(register uint8_t code) {
		PCL = mem_read(SP++);
		PCH = mem_read(SP++);
}

void IM_0(register uint8_t code) {
		IM = 0;
}

void IM_1(register uint8_t code) {
		IM = 1;
}

void IM_2(register uint8_t code) {
		IM = 2;
}

void LDn(register uint8_t code) {
	register int8_t tmp;
	register int8_t ixiyshift = 0;

	if((code == 0x36) && IS_DDFD_PREFIX) {
		ixiyshift = mem_read(PC++);
	}
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

void LDx(register uint8_t code) {
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

void LDm(register uint8_t code) {
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

void LDs(register uint8_t code) {//LD SP, HL
	SP = HLIXIY_REG;
}

void LD_(register uint8_t code) {
	register int8_t ixiyshift = 0;
	register uint8_t tmp;

	if(IS_DDFD_PREFIX)
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
		tmp = *((uint8_t*)(&regs) + (srcnum^1));
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
		*((uint8_t*)(&regs) + (dstnum^1)) = tmp;
	}
}

void LDBL(register uint8_t code) {
	register uint8_t tmp;
	tmp=mem_read(HL);
	BC--;
	mem_write(DE,tmp);

	if((code & 0x0f) == 0x00) {//LDI , LDIR
		DE++; HL++;
	}
	else {//LDD , LDDR
		DE--; HL--;
	}

	tmp += A;
	F = (F & (FLAG_C | FLAG_Z | FLAG_S)) | (BC ? FLAG_V : 0) | (tmp & FLAG_3) | ((tmp & 0x02) ? FLAG_5 : 0);

	if((code & 0x10) && BC) {//LDIR , LDDR
		PC -= 2;
	}
}

void LDIR(register uint8_t code) {
	register uint8_t *regptr;
	if((code & 0x0f) == 0x07) {//r = I
		regptr = &I;
	}
	else {//r = R
		regptr = &R;
	}

	if((code & 0xf0) == 0x40) {//LD r, A
		*regptr = A;
	}
	else {//LD A, r
		A = *regptr;
		F = (F & FLAG_C) | (sz53p_table[A] & ~FLAG_P) | (IFF2 ? FLAG_V : 0);
	}
}

void EDLD(register uint8_t code) {
	register uint16_t addr;
	register uint16_t* reg;
	addr = mem_read(PC++);
	addr |= mem_read(PC++) << 8;

	switch ((code & 0x30) >> 4) {
	case 0x00://LD (**),BC
		reg = &BC;
		break;
	case 0x01://LD (**),DE
		reg = &DE;
		break;
	case 0x02://LD (**),HL
		reg = &HL;
		break;
	case 0x03://LD (**),SP
		reg = &SP;
		break;
	}
	if(code & 0x08) {
		*reg = mem_read(addr) | (mem_read(addr+1) << 8);
	}
	else {
		mem_write(addr, (*reg & 0x00ff));
		mem_write(addr+1, (*reg >> 8));
	}
}

void ICx(register uint8_t code) {
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

void IC8(register uint8_t code) {
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
		if(IS_DDFD_PREFIX)
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

void DCx(register uint8_t code) {
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

void DC8(register uint8_t code) {
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
		if(IS_DDFD_PREFIX)
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

void CPL(register uint8_t code) {
	A = ~A;
	F = (F & (FLAG_C | FLAG_P | FLAG_Z | FLAG_S)) | (A & (FLAG_3 | FLAG_5)) | FLAG_N | FLAG_H;
}

void ALx(register uint8_t code) {
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

static uint8_t calc_F(register uint8_t src1, register uint8_t src2, register int16_t res, register uint8_t op) {
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

void ALn(register uint8_t code) {
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

void ALU(register uint8_t code) {
	register int8_t ixiyshift = 0;
	register uint8_t src;
	register int16_t res;

	switch(code & 0x07) {
	case 0x07://A
		src = A; break;
	case 0x06://(HL)
		if(IS_DDFD_PREFIX) {
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
		src = *((uint8_t*)(&regs) + ((code & 0x07)^1));
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

void NEG_(register uint8_t code) {
	register uint8_t src;
	register int16_t res;
	src = A;
	A = 0;
	res = A-src;
	F = calc_F(A, src, res, 1);
	A = res;
}

void SBCx(register uint8_t code) {
	register uint32_t tmp;
	register uint8_t reg;

	switch ((code & 0x30) >> 4) {
	case 0x00://SBC HL, BC
		tmp = HL - BC - (F & FLAG_C);
		reg = B;
		break;
	case 0x01://SBC HL, DE
		tmp = HL - DE - (F & FLAG_C);
		reg = D;
		break;
	case 0x02://SBC HL, HL
		tmp = HL - HL - (F & FLAG_C);
		reg = H;
		break;
	case 0x03://SBC HL, SP
		tmp = HL - SP - (F & FLAG_C);
		reg = SPH;
		break;
	}

	F = (calc_F(H, reg, (tmp >> 8), 1) & ~FLAG_Z)  | ((tmp & 0xffff) ? 0 : FLAG_Z);
	HL = (uint16_t)tmp;
}

void ADCx(register uint8_t code) {
	register uint32_t tmp;
	register uint8_t reg;

	switch ((code & 0x30) >> 4) {
	case 0x00://ADC HL, BC
		tmp = HL + BC + (F & FLAG_C);
		reg = B;
		break;
	case 0x01://ADC HL, DE
		tmp = HL + DE + (F & FLAG_C);
		reg = D;
		break;
	case 0x02://ADC HL, HL
		tmp = HL + HL + (F & FLAG_C);
		reg = H;
		break;
	case 0x03://ADC HL, SP
		tmp = HL + SP + (F & FLAG_C);
		reg = SPH;
		break;
	}

	F = (calc_F(H, reg, (tmp >> 8), 0) & ~FLAG_Z)  | ((tmp & 0xffff) ? 0 : FLAG_Z);
	HL = (uint16_t)tmp;
}

void CPBL(register uint8_t code) {
	register uint8_t tmp1, tmp2;

	tmp2 = mem_read(HL);
	tmp1 = A - tmp2;
	if(code & 0x08)
		HL--;
	else
		HL++;
	BC--;

	F = (F & FLAG_C) | (BC ? (FLAG_V | FLAG_N) : FLAG_N) | (tmp1 ? 0 : FLAG_Z) | (tmp1 & FLAG_S);
	F |= CALC_SUB_H(A, tmp2, tmp1);
	F |= (((F & FLAG_H) ? tmp1-1 : tmp1) & FLAG_3) | ((((F & FLAG_H) ? tmp1-1 : tmp1) & 0x02) ? FLAG_5 : 0);

	if(((code & 0xf0) == 0xb0) && ((F & (FLAG_V | FLAG_Z)) == FLAG_V))
		PC -= 2;
}

void JR_(register uint8_t code) {
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

void RST(register uint8_t code) {
	mem_write(--SP, PCH);
	mem_write(--SP, PCL);
	PCH = 0;
	PCL = code & 0x38;
}

void JPc(register uint8_t code) {
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

void JMP(register uint8_t code) {
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

void POP(register uint8_t code) {
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

void PSH(register uint8_t code) {
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

void SFT(register uint8_t code) {
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

void CBSFT(register uint8_t code) {
	register uint8_t tmp, tmpres;
	register uint8_t regnum = code & 0x07;

	if(IS_DDFD_PREFIX) {
		tmpres = mem_read(HLIXIY_REG+regs.ixiyshift);
		z80_tstates += 15;
	}
	else if(regnum == 0x07) {//A
		tmpres = A;
	}
	else if(regnum == 0x06) {//F
		tmpres = mem_read(HL);
		z80_tstates += 7;
	}
	else
		tmpres = *((uint8_t*)(&regs) + (regnum^1));

	switch ((code & 0x38) >> 3) {
	case 0x00: //RLC
		tmpres = (tmpres << 1) | (tmpres >> 7);
		F = (tmpres & FLAG_C);
		break;
	case 0x01: //RRC
		F = tmpres & FLAG_C;
		tmpres = (tmpres >> 1) | (tmpres << 7);
		break;
	case 0x02: //RL
		tmp = tmpres;
		tmpres = (tmpres << 1) | (F & FLAG_C);
		F = (tmp >> 7);
		break;
	case 0x03: //RR
		tmp = tmpres;
		tmpres = (tmpres >> 1) | (F << 7);
		F = (tmp & FLAG_C);
		break;
	case 0x04: //SLA
		F = tmpres >> 7;
		tmpres <<= 1;
		break;
	case 0x05: //SRA
		F = tmpres & FLAG_C;
		tmpres = (tmpres & 0x80) | (tmpres >> 1);
		break;
	case 0x06: //SLL
		F = tmpres >> 7;
		tmpres = ( tmpres << 1 ) | 0x01;
		break;
	case 0x07: //SRL
		F = tmpres & FLAG_C;
		tmpres >>= 1;
		break;
	}
	F |= sz53p_table[tmpres];

	if(IS_DDFD_PREFIX) {
		mem_write(HLIXIY_REG+regs.ixiyshift, tmpres);
		regs.ixiyshift = 0;
	}
	else if(regnum == 0x07) {//A
		A = tmpres;
	}
	else if(regnum == 0x06) {//F
		mem_write(HL, tmpres);
	}
	else
		*((uint8_t*)(&regs) + (regnum^1)) = tmpres;
}

void EDSF(register uint8_t code) {
	register uint8_t tmp;

	tmp = mem_read(HL);

	if(code == 0x67) {//RRD
		mem_write(HL, (A << 4) | (tmp >> 4));
		A = (A & 0xf0) | (tmp & 0x0f);
	}
	else {//RLD
		mem_write(HL, (tmp << 4) | (A & 0x0f));
		A = (A & 0xf0) | (tmp >> 4);
	}

	F = (F & FLAG_C) | sz53p_table[A];
}

void BIT(register uint8_t code) {
	register uint8_t tmpres, hidden;
	register uint8_t bitmask = 0x01 << ((code & 0x38) >> 3);
	register uint8_t regnum = code & 0x07;

	if(IS_DDFD_PREFIX) {
		tmpres = mem_read(HLIXIY_REG+regs.ixiyshift);
		hidden = ((HLIXIY_REG + regs.ixiyshift) >> 8) & 0x00ff;
		z80_tstates += 15;
	}
	else if(regnum == 0x07) {//A
		tmpres = A;
	}
	else if(regnum == 0x06) {//(HL)
		tmpres = mem_read(HL);
		hidden = H;
		z80_tstates += 7;
	}
	else
		tmpres = *((uint8_t*)(&regs) + (regnum^1));

	switch (code & 0xc0) {
	case 0x40: //BIT
		F = (F & FLAG_C) | FLAG_H;
		if(regnum == 0x06)//(HL)
			F |= (hidden & (FLAG_5 | FLAG_3));
		else
			F |= (tmpres & (FLAG_5 | FLAG_3));

		tmpres &= bitmask;

		if(tmpres)
			F |= (tmpres & FLAG_S);
		else
			F |= FLAG_P | FLAG_Z;

		return;//not need write back result
		break;
	case 0x80: //RES
		tmpres &= ~bitmask; if(regnum == 0x06) z80_tstates += 3; break;
	case 0xc0: //SET
		tmpres |= bitmask; if(regnum == 0x06) z80_tstates += 3; break;
	}

	if(IS_DDFD_PREFIX) {
		mem_write(HLIXIY_REG+regs.ixiyshift, tmpres);
		regs.ixiyshift = 0;
	}
	else if(regnum == 0x07) {//A
		A = tmpres;
	}
	else if(regnum == 0x06) {//F
		mem_write(HL, tmpres);
	}
	else
		*((uint8_t*)(&regs) + (regnum^1)) = tmpres;
}

void EX_(register uint8_t code) {
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

void IO_(register uint8_t code) {
	if(code == 0xd3) {//OUT (*),A
		port_out(((uint16_t)A << 8) | mem_read(PC++), A);
	}
	else {//0xdb IN A,(*)
		A = port_in(((uint16_t)A << 8) | mem_read(PC++));
	}
}

void EDIN(register uint8_t code) {
	register uint8_t tmp1, reg;

	switch ((code & 0x38) >> 3) {
	case 0x00://IN
		reg = B = port_in(BC); break;
	case 0x01://IN
		reg = C = port_in(BC); break;
	case 0x02://IN
		reg = D = port_in(BC); break;
	case 0x03://IN
		reg = E = port_in(BC); break;
	case 0x04://IN
		reg = H = port_in(BC); break;
	case 0x05://IN
		reg = L = port_in(BC); break;
	case 0x06://IN
		reg = tmp1 = port_in(BC); break;
	case 0x07://IN
		reg = A = port_in(BC); break;
	}

	F = ( F & FLAG_C) | sz53p_table[reg];
}

void EDOU(register uint8_t code) {
	switch ((code & 0x38) >> 3) {
	case 0x00://OUT
		port_out(BC, B); break;
	case 0x01://OUT
		port_out(BC, C); break;
	case 0x02://OUT
		port_out(BC, D); break;
	case 0x03://OUT
		port_out(BC, E); break;
	case 0x04://OUT
		port_out(BC, H); break;
	case 0x05://OUT
		port_out(BC, L); break;
	case 0x06://OUT
		port_out(BC, 0); break;
	case 0x07://OUT
		port_out(BC, A); break;
	}
}

void IOBL(register uint8_t code) {
	register uint8_t tmp1, tmp2;

	if((code & 0x03) == 0x02) {//INI , INIR, IND, INDR
		tmp1 = port_in(BC);
		mem_write(HL, tmp1);
		B--;
		tmp2 = tmp1 + C;
		if(code & 0x08) {//decrement
			HL--; tmp2--;
		}
		else {//increment
			HL++; tmp2++;
		}

	}
	else {//OUTI, OUTIR, OUTD, OUTDR
		tmp1 = mem_read(HL);
		B--;

		if(code & 0x08) {//decrement
			HL--;
		}
		else {//increment
			HL++;
		}

		port_out(BC, tmp1);
		tmp2 = tmp1 + L;
	}

	F = ((tmp1 & 0x80) ? FLAG_N : 0) | ((tmp2 < tmp1 ) ? FLAG_H | FLAG_C : 0) |
			((sz53p_table[(tmp2 & 0x07) ^ B] & FLAG_P) ? FLAG_P : 0) | (sz53p_table[B] & ~FLAG_P);
	if(((code & 0xf0) == 0xb0) && B)
		PC -= 2;
}

void DAA(register uint8_t code) {
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

void PFX(register uint8_t code) {
	switch (code) {
	case 0xed://ED
		state.prefix = 0x00ed; break;
	case 0xcb://CB
		state.prefix = (state.prefix & 0xff00) | 0x00cb; break;
	case 0xdd://DD
		state.prefix = 0xdd00; break;
	case 0xfd://FD
		state.prefix = 0xfd00; break;
	}

	state.int_blocked = 1;
}

