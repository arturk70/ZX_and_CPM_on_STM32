/*
 * z80_ED_ops.h
 *
 *  Created on: Feb 14, 2020
 *      Author: artur
 */

#ifndef SRC_ZXCPMSRC_Z80_ED_OPS_H_
#define SRC_ZXCPMSRC_Z80_ED_OPS_H_

__attribute__((always_inline)) void ed_ops_process(register uint32_t code)
{
switch (code) {

case 0x40:
case 0x50:
case 0x60:
case 0x70:
case 0x48:
case 0x58:
case 0x68:
case 0x78:
{//EDIN
	register uint8_t reg;

	reg = port_in(BC);
	switch ((code & 0x38) >> 3) {
	case 0x00://IN
		B = reg; break;
	case 0x01://IN
		C = reg; break;
	case 0x02://IN
		D = reg; break;
	case 0x03://IN
		E = reg; break;
	case 0x04://IN
		H = reg; break;
	case 0x05://IN
		L = reg; break;
	case 0x06://IN
		break;
	case 0x07://IN
		A = reg; break;
	}

	F = ( F & FLAG_C) | sz53p_table[reg];
}
break;

case 0x41:
case 0x51:
case 0x61:
case 0x71:
case 0x49:
case 0x59:
case 0x69:
case 0x79:
{//EDOU
	register uint8_t reg;

	switch ((code & 0x38) >> 3) {
	case 0x00://OUT
		reg = B; break;
	case 0x01://OUT
		reg = C; break;
	case 0x02://OUT
		reg = D; break;
	case 0x03://OUT
		reg = E; break;
	case 0x04://OUT
		reg = H; break;
	case 0x05://OUT
		reg = L; break;
	case 0x06://OUT
		reg = 0; break;
	case 0x07://OUT
		reg = A; break;
	}
	port_out(BC, reg);
}
break;

case 0x42:
case 0x52:
case 0x62:
case 0x72:
{//SBCx
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
break;

case 0x4a:
case 0x5a:
case 0x6a:
case 0x7a:
{//ADCx
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
break;

case 0x43:
case 0x53:
case 0x63:
case 0x73:
case 0x4b:
case 0x5b:
case 0x6b:
case 0x7b:
{//EDLD
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
break;

case 0x44:
case 0x54:
case 0x64:
case 0x74:
case 0x4c:
case 0x5c:
case 0x6c:
case 0x7c:
{//NEG_
	register uint8_t src;
	register int16_t res;
	src = A;
	A = 0;
	res = A-src;
	F = calc_F(A, src, res, 1);
	A = res;
}
break;

case 0x45:
case 0x55:
case 0x65:
case 0x75:
case 0x5d:
case 0x6d:
case 0x7d://RETN
	IFF1=IFF2;
case 0x4d://RETI
	PCL = mem_read(SP++);
	PCH = mem_read(SP++);
	break;

case 0x46:
case 0x66:
case 0x4e:
case 0x6e://IM_0
	IM = 0;
	break;

case 0x56:
case 0x76://IM_1
	IM = 1;
	break;

case 0x5e:
case 0x7e://IM_2
	IM = 2;
	break;

case 0x47://LD I,A
	I = A; break;

case 0x57://LD A,I
	A = I; break;

case 0x4f://LD R,A
	RR = A & 0x7f; R8 = A & 0x80; break;

case 0x5f://LD A,R
	A = (RR & 0x7f) | (R8 & 0x80);
	F = (F & FLAG_C) | (sz53p_table[A] & ~FLAG_P) | (IFF2 ? FLAG_V : 0);
	break;

case 0x67:
case 0x6f:
{//EDSF
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
break;

case 0xa0:
case 0xb0:
case 0xa8:
case 0xb8:
{//LDBL
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
break;

case 0xa1:
case 0xb1:
case 0xa9:
case 0xb9:
{//CPBL
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
break;

case 0xa2:
case 0xb2:
case 0xa3:
case 0xb3:
case 0xaa:
case 0xba:
case 0xab:
case 0xbb:
{//IOBL
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
			(sz53p_table[(tmp2 & 0x07) ^ B] & FLAG_P) | (sz53p_table[B] & ~FLAG_P);
	if(((code & 0xf0) == 0xb0) && B)
		PC -= 2;
}
break;

default://NONI
	z80_state.int_blocked = 1;
}
}
#endif /* SRC_ZXCPMSRC_Z80_ED_OPS_H_ */
