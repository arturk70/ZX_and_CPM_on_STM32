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
		 8,  8, 11, 16, 4, 10, 4,  5,  8,  8, 11, 16, 4, 10, 4,  5, //0x40-0x4f
		 8,  8, 11, 16, 4, 10, 4,  5,  8,  8, 11, 16, 4, 10, 4,  5, //0x50-0x5f
		 8,  8, 11, 16, 4, 10, 4, 14,  8,  8, 11, 16, 4, 10, 4, 14, //0x60-0x6f
		 8,  8, 11, 16, 4, 10, 4, 14,  8,  8, 11, 16, 4, 10, 4, 14, //0x70-0x7f
		12, 12, 12, 12, 4,  4, 4,  4, 12, 12, 12, 12, 4,  4, 4,  4, //0xa0-0xaf
		12, 12, 12, 12, 4,  4, 4,  4, 12, 12, 12, 12, 4,  4, 4,  4  //0xb0-0xbf
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

void NONI(uint8_t code, int8_t *tstates) {
	state.int_blocked = 1;

	tstates += 4;
}

void NOP(uint8_t code, int8_t *tstates) {

}

void HLT(uint8_t code, int8_t *tstates) {
	state.halted = 1;
}

void DI_(uint8_t code, int8_t *tstates) {
	IFF1 = IFF2 = 0;
}

void EI_(uint8_t code, int8_t *tstates) {
	IFF1 = IFF2 = 1; state.int_blocked = 1;
}

void SCF(uint8_t code, int8_t *tstates) {
	F = (F & (FLAG_P | FLAG_Z | FLAG_S)) | (A & (FLAG_3 | FLAG_5)) | FLAG_C;
}

void CCF(uint8_t code, int8_t *tstates) {
	F = (F & (FLAG_P | FLAG_Z | FLAG_S)) | ((F & FLAG_C) ? FLAG_H : FLAG_C) | (A & (FLAG_3 | FLAG_5));
}

void RETN(uint8_t code, int8_t *tstates) {
		IFF1=IFF2;
		PCL = mem_read(SP++);
		PCH = mem_read(SP++);
}

void RETI(uint8_t code, int8_t *tstates) {
		PCL = mem_read(SP++);
		PCH = mem_read(SP++);
}

void IM_0(uint8_t code, int8_t *tstates) {
		IM = 0;
}

void IM_1(uint8_t code, int8_t *tstates) {
		IM = 1;
}

void IM_2(uint8_t code, int8_t *tstates) {
		IM = 2;
}

void LDn(uint8_t code, int8_t *tstates) {
	switch (code >> 3) {
	case 0x00://LD  B,*
		B = mem_read(PC++);
		break;
	case 0x02://LD  D,*
		D = mem_read(PC++);
		break;
	case 0x04://LD  H,*
		HLIXIY_REGH = mem_read(PC++);
		break;
	case 0x06://LD  (HL),*
	{
		register int8_t tmp;
		register int8_t ixiyshift = 0;
		if(IS_DDFD_PREFIX) {
			ixiyshift = mem_read(PC++);
		}
		tmp = mem_read(PC++);
		mem_write(HLIXIY_REG+ixiyshift, tmp);
		break;
	}
	case 0x01://LD  C,*
		C = mem_read(PC++);
		break;
	case 0x03://LD  E,*
		E = mem_read(PC++);
		break;
	case 0x05://LD  L,*
		HLIXIY_REGL = mem_read(PC++);
		break;
	case 0x07://LD  A,*
		A = mem_read(PC++);
		break;
	}
}

void LDx(uint8_t code, int8_t *tstates) {
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

void LDm(uint8_t code, int8_t *tstates) {
	register uint16_t tmp;
	if((code >> 3) > 0x03) {
		tmp = mem_read(PC++);
		tmp |= mem_read(PC++) << 8;
	}

	switch (code >> 3) {
	case 0x00://LD (BC),A
		mem_write(BC, A);
		break;
	case 0x02://LD (DE),A
		mem_write(DE, A);
		break;
	case 0x04://LD (**),HL
		mem_write(tmp++, HLIXIY_REGL);
		mem_write(tmp, HLIXIY_REGH);
		break;
	case 0x06://LD (**),A
		mem_write(tmp, A);
		break;
	case 0x01://LD A,(BC)
		A = mem_read(BC);
		break;
	case 0x03://LD A,(DE)
		A = mem_read(DE);
		break;
	case 0x05://LD HL,(**)
		HLIXIY_REGL = mem_read(tmp++);
		HLIXIY_REGH = mem_read(tmp);
		break;
	case 0x07://LD A,(**)
		A = mem_read(tmp);
		break;
	}
}

void LDs(uint8_t code, int8_t *tstates) {//LD SP, HL
	SP = HLIXIY_REG;
}

void LD_(uint8_t code, int8_t *tstates) {
	register int8_t ixiyshift = 0;
	register uint8_t tmp;

	if(IS_DDFD_PREFIX)
		ixiyshift = mem_read(PC++);

	register uint8_t srcnum = code & 0x07;
	register uint8_t dstnum = (code & 0x38) >> 3;

	if(srcnum == 0x07) {//A
		tmp = A;
	} else if(srcnum == 0x06) {//F
		tmp = mem_read(HLIXIY_REG + ixiyshift);
	} else if(srcnum == 0x04) {//H
		if(code == 0x74)
			tmp = H;
		else
			tmp = HLIXIY_REGH;
	} else if(srcnum == 0x05) {//L
		if(code == 0x75)
			tmp = L;
		else
			tmp = HLIXIY_REGL;
	}else {//other reg
		tmp = *((uint8_t*)(&regs) + (srcnum^1));
	}

	if(dstnum == 0x07) {//A
		A = tmp;
	} else if(dstnum == 0x06) {//F
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

void LDBL(uint8_t code, int8_t *tstates) {
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

void LDIR(uint8_t code, int8_t *tstates) {
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

//	switch (code) {
//	case 0x47://LD I, A
//		I = A;
//		break;
//	case 0x57://LD A, I
//		A = I;
//		F = (F & FLAG_C) | (sz53p_table[A] & ~FLAG_P) | (IFF2 ? FLAG_V : 0);
//		break;
//	case 0x4f://LD R, A
//		R = A;
//		break;
//	case 0x5f://LD A, R
//		A = R;
//		F = (F & FLAG_C) | (sz53p_table[A] & ~FLAG_P) | (IFF2 ? FLAG_V : 0);
//		break;
//	}

}

void EDLD(uint8_t code, int8_t *tstates) {
	uint16_t tmp;
	tmp = mem_read(PC++);
	tmp |= mem_read(PC++) << 8;

	switch ((code & 0x38) >> 3) {
	case 0x00://LD (**),BC
		mem_write(tmp++, C);
		mem_write(tmp, B);
		break;
	case 0x02://LD (**),DE
		mem_write(tmp++, E);
		mem_write(tmp, D);
		break;
	case 0x04://LD (**),HL
		mem_write(tmp++, L);
		mem_write(tmp, H);
		break;
	case 0x06://LD (**),SP
		mem_write(tmp++, SPL);
		mem_write(tmp, SPH);
		break;
	case 0x01://LD BC,(**)
		C = mem_read(tmp++);
		B = mem_read(tmp);
		break;
	case 0x03://LD DE,(**)
		E = mem_read(tmp++);
		D = mem_read(tmp);
		break;
	case 0x05://LD HL,(**)
		L = mem_read(tmp++);
		H = mem_read(tmp);
		break;
	case 0x07://LD SP,(**)
		SPL = mem_read(tmp++);
		SPH = mem_read(tmp);
		break;
	}
}

void ICx(uint8_t code, int8_t *tstates) {
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

void IC8(uint8_t code, int8_t *tstates) {
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

void DCx(uint8_t code, int8_t *tstates) {
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

void DC8(uint8_t code, int8_t *tstates) {
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

void CPL(uint8_t code, int8_t *tstates) {
	A = ~A;
	F = (F & (FLAG_C | FLAG_P | FLAG_Z | FLAG_S)) | (A & (FLAG_3 | FLAG_5)) | FLAG_N | FLAG_H;
}

void ALx(uint8_t code, int8_t *tstates) {
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

void ALn(uint8_t code, int8_t *tstates) {
	register uint8_t tmp1 = mem_read(PC++);
	register int16_t tmp2;
	switch ((code & 0x38) >> 3) {
	case 0x00://ADD A,*
		tmp2 = A + tmp1;
		F = CALC_C(tmp2) | CALC_ADD_H(A, tmp1, tmp2) | CALC_ADD_V(A, tmp1, tmp2) | (sz53p_table[(tmp2 & 0x00ff)] & ~FLAG_P);
		A = tmp2;
		break;
	case 0x02://SUB A,*
		tmp2 = A - tmp1;
		F = CALC_C(tmp2) | FLAG_N |	CALC_SUB_H(A, tmp1, tmp2) | CALC_SUB_V(A, tmp1, tmp2) | (sz53p_table[(tmp2 & 0x00ff)] & ~FLAG_P);
		A = tmp2;
		break;
	case 0x04://AND *
		A &= tmp1;
		F = FLAG_H | sz53p_table[A];
		break;
	case 0x06://OR *
		A |= tmp1;
		F = sz53p_table[A];
		break;
	case 0x01://ADC A,*
		tmp2 = A + tmp1 + (F & FLAG_C);
		F = CALC_C(tmp2) | CALC_ADD_H(A, tmp1, tmp2) | CALC_ADD_V(A, tmp1, tmp2) | (sz53p_table[(tmp2 & 0x00ff)] & ~FLAG_P);
		A = tmp2;
		break;
	case 0x03://SBC A,*
		tmp2 = A - tmp1 - (F & FLAG_C);
		F = CALC_C(tmp2) | FLAG_N |	CALC_SUB_H(A, tmp1, tmp2) | CALC_SUB_V(A, tmp1, tmp2) | (sz53p_table[(tmp2 & 0x00ff)] & ~FLAG_P);
		A = tmp2;
		break;
	case 0x05://XOR *
		A ^= tmp1;
		F = sz53p_table[A];
		break;
	case 0x07://CP *
		tmp2 = A - tmp1;
		F = CALC_C(tmp2) | FLAG_N | CALC_SUB_H(A, tmp1, tmp2) | CALC_SUB_V(A, tmp1, tmp2) | (tmp1 & (FLAG_3 | FLAG_5)) | (tmp2 & FLAG_S) | CALC_Z(tmp2);
		break;
	}
}

void ALU(uint8_t code, int8_t *tstates) {
	register int8_t ixiyshift = 0;
	register uint8_t src;
	register uint16_t res;

	switch(code & 0x07) {
	case 0x07://A
		src = A; break;
	case 0x06://(HL)
		if(IS_DDFD_PREFIX) {
			ixiyshift = mem_read(PC++);
			tstates+=8;
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
			F = CALC_C(res) | CALC_ADD_H(A, src, res) | CALC_ADD_V(A, src, res) | (sz53p_table[(res & 0x00ff)] & ~FLAG_P);
			A = res;
		}
		else {//ADD
			res = A + src;
			F = CALC_C(res) | CALC_ADD_H(A, src, res) | CALC_ADD_V(A, src, res) | (sz53p_table[(res & 0x00ff)] & ~FLAG_P);
			A = res;
		}
	}
	else if((code & 0xf0) == 0x90) {
		if(code & 0x08) {//SBC
			res = A - src - (F & FLAG_C);
			F = CALC_C(res) | FLAG_N |	CALC_SUB_H(A, src, res) | CALC_SUB_V(A, src, res) | (sz53p_table[(res & 0x00ff)] & ~FLAG_P);
			A = res;
		}
		else {//SUB
			res = A - src;
			F = CALC_C(res) | FLAG_N |	CALC_SUB_H(A, src, res) | CALC_SUB_V(A, src, res) | (sz53p_table[(res & 0x00ff)] & ~FLAG_P);
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
			F = CALC_C(res) | FLAG_N | CALC_SUB_H(A, src, res) | CALC_SUB_V(A, src, res) | (src & (FLAG_3 | FLAG_5)) | (res & FLAG_S) | CALC_Z(res);
		}
		else {//OR
			A |= src;
			F = sz53p_table[A];
		}
	}
}

void NEG_(uint8_t code, int8_t *tstates) {
	register uint8_t src;
	register uint16_t res;
	src = A;
	A = 0;
	res = A-src;
	F = CALC_C(res) | FLAG_N |	CALC_SUB_H(A, src, res) | CALC_SUB_V(A, src, res) | (sz53p_table[(res & 0x00ff)] & ~FLAG_P);
	A = res;
}

void SBCx(uint8_t code, int8_t *tstates) {
	register uint32_t tmp;
	register uint16_t reg;

	switch ((code & 0x30) >> 4) {
	case 0x00://SBC HL, BC
		tmp = HL - BC - (F & FLAG_C);
		reg = BC;
		break;
	case 0x01://SBC HL, DE
		tmp = HL - DE - (F & FLAG_C);
		reg = DE;
		break;
	case 0x02://SBC HL, HL
		tmp = HL - HL - (F & FLAG_C);
		reg = HL;
		break;
	case 0x03://SBC HL, SP
		tmp = HL - SP - (F & FLAG_C);
		reg = SP;
		break;
	}

	F = CALC_C(tmp >> 8) | FLAG_N | CALC_SUB_V(H, (reg >> 8), (tmp >> 8)) | ((tmp >> 8) & (FLAG_3 | FLAG_5 | FLAG_S)) |
					CALC_SUB_H(H, (reg >> 8), (tmp >> 8)) | ((tmp & 0xffff) ? 0 : FLAG_Z);
	HL = (uint16_t)tmp;
}

void ADCx(uint8_t code, int8_t *tstates) {
	register uint32_t tmp;
	register uint16_t reg;

	switch ((code & 0x30) >> 4) {
	case 0x00://ADC HL, BC
		tmp = HL + BC + (F & FLAG_C);
		reg = BC;
		break;
	case 0x01://ADC HL, DE
		tmp = HL + DE + (F & FLAG_C);
		reg = DE;
		break;
	case 0x02://ADC HL, HL
		tmp = HL + HL + (F & FLAG_C);
		reg = HL;
		break;
	case 0x03://ADC HL, SP
		tmp = HL + SP + (F & FLAG_C);
		reg = SP;
		break;
	}

	F = CALC_C(tmp >> 8) | CALC_ADD_V(H, (reg >> 8), (tmp >> 8)) | ((tmp >> 8) & (FLAG_3 | FLAG_5 | FLAG_S)) |
					CALC_ADD_H(H, (reg >> 8), (tmp >> 8)) | ((tmp & 0xffff) ? 0 : FLAG_Z);
	HL = (uint16_t)tmp;
}

void CPBL(uint8_t code, int8_t *tstates) {
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

void JR_(uint8_t code, int8_t *tstates) {
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
		tstates -= 5;
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
		tstates += 5;
	}
}

void RST(uint8_t code, int8_t *tstates) {
	mem_write(--SP, PCH);
	mem_write(--SP, PCL);
	PCH = 0;
	PCL = code & 0x38;
}

void JPc(uint8_t code, int8_t *tstates) {
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
			tstates += 6;
			dl = mem_read(SP++);
			dh = mem_read(SP++);
		}
		else if(code & 0x04) {//for CALL only
			tstates += 7;
			mem_write(--SP, PCH);
			mem_write(--SP, PCL);
		}

		PCL = dl;
		PCH = dh;
	}
}

void JMP(uint8_t code, int8_t *tstates) {
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

void POP(uint8_t code, int8_t *tstates) {
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

void PSH(uint8_t code, int8_t *tstates) {
	register uint8_t dh, dl;

	switch (code >> 4) {
	case 0x0c://PUSH BC
		dh = B;	dl = C;
		break;
	case 0x0d://PUSH DE
		dh = D;	dl = E;
		break;
	case 0x0e://PUSH HL
		dh = HLIXIY_REGH; dl = HLIXIY_REGL;
		break;
	case 0x0f://PUSH AF
		dh = A;	dl = F;
		break;
	}

	mem_write(--SP, dh);
	mem_write(--SP, dl);
}

void SFT(uint8_t code, int8_t *tstates) {
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

void CBSFT(uint8_t code, int8_t *tstates) {
	register uint8_t tmp, tmpres;
	register uint8_t regnum = code & 0x07;

	if(IS_DDFD_PREFIX) {
		tmpres = mem_read(HLIXIY_REG+regs.ixiyshift);
		tstates += 15;
	}
	else if(regnum == 0x07) {//A
		tmpres = A;
	}
	else if(regnum == 0x06) {//F
		tmpres = mem_read(HL);
		tstates += 7;
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

void EDSF(uint8_t code, int8_t *tstates) {
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

void BIT(uint8_t code, int8_t *tstates) {
	register uint8_t tmpres;
	register uint8_t bitmask = 0x01 << ((code & 0x38) >> 3);
	register uint8_t regnum = code & 0x07;

	if(IS_DDFD_PREFIX) {
		tmpres = mem_read(HLIXIY_REG+regs.ixiyshift);
		tstates += 15;
	}
	else if(regnum == 0x07) {//A
		tmpres = A;
	}
	else if(regnum == 0x06) {//F
		tmpres = mem_read(HL);
		tstates += 7;
	}
	else
		tmpres = *((uint8_t*)(&regs) + (regnum^1));

	switch (code & 0xc0) {
	case 0x40: //BIT
		//for (HL) operations flags 3 & 5 must be set in other way - not realized
		F = (F & FLAG_C) | FLAG_H | (tmpres & (FLAG_3 | FLAG_5));
		if(!(tmpres & bitmask)) F |= FLAG_P | FLAG_Z;
		if((bitmask == 0x80) && (tmpres & 0x80)) F |= FLAG_S;
		return;//not need write back result
		break;
	case 0x80: //RES
		tmpres &= ~bitmask; if(regnum == 0x06) tstates += 3; break;
	case 0xc0: //SET
		tmpres |= bitmask; if(regnum == 0x06) tstates += 3; break;
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

void EX_(uint8_t code, int8_t *tstates) {
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

void IO_(uint8_t code, int8_t *tstates) {
	if(code == 0xd3) {//OUT (*),A
		port_out(((uint16_t)A << 8) | mem_read(PC++), A);
	}
	else {//0xdb IN A,(*)
		A = port_in(((uint16_t)A << 8) | mem_read(PC++));
	}
}

void EDIN(uint8_t code, int8_t *tstates) {
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

void EDOU(uint8_t code, int8_t *tstates) {
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

void IOBL(uint8_t code, int8_t *tstates) {
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

void DAA(uint8_t code, int8_t *tstates) {
	uint8_t add = 0;
	uint8_t carry = (F & FLAG_C);
	uint16_t tmp2;

	if((F & FLAG_H) || ((A & 0x0f) > 9))
		add = 6;
	if(carry || (A > 0x99))
		add |= 0x60;
	if(A > 0x99)
		carry = FLAG_C;
	if((F = (F & FLAG_N))) {
		tmp2 = A - add;
		F |= CALC_SUB_H(A, add, tmp2);
		A = tmp2;
	} else {
		tmp2 = A + add;
		F |= CALC_ADD_H(A, add, tmp2);
		A = tmp2;
	}

	F |= CALC_C(tmp2) | sz53p_table[A];
}

void PFX(uint8_t code, int8_t *tstates) {
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

