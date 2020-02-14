/*
 * z80_CB_ops.h
 *
 *  Created on: Feb 12, 2020
 *      Author: artur
 */

#ifndef SRC_ZXCPMSRC_Z80_CB_OPS_H_
#define SRC_ZXCPMSRC_Z80_CB_OPS_H_

if(code < 0x40) {//CB SHIFT
	register uint8_t tmp, tmpres;
	register uint8_t regnum = code & 0x07;

	if(hlixiyptr != &HL) {
		tmpres = mem_read(HLIXIY_REG+gixiyshift);
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
		tmpres = *(regs + (regnum^1));

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

	if(hlixiyptr != &HL) {
		mem_write(HLIXIY_REG+gixiyshift, tmpres);
		gixiyshift = 0;
	}
	else if(regnum == 0x07) {//A
		A = tmpres;
	}
	else if(regnum == 0x06) {//F
		mem_write(HL, tmpres);
	}
	else
		*(regs + (regnum^1)) = tmpres;
}
else {//CB BIT
	register uint8_t tmpres, hidden;
	register uint8_t bitmask = 0x01 << ((code & 0x38) >> 3);
	register uint8_t regnum = code & 0x07;

	if(hlixiyptr != &HL) {
		tmpres = mem_read(HLIXIY_REG+gixiyshift);
		hidden = ((HLIXIY_REG + gixiyshift) >> 8) & 0x00ff;
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
		tmpres = *(regs + (regnum^1));

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

	if(hlixiyptr != &HL) {
		mem_write(HLIXIY_REG+gixiyshift, tmpres);
		gixiyshift = 0;
	}
	else if(regnum == 0x07) {//A
		A = tmpres;
	}
	else if(regnum == 0x06) {//F
		mem_write(HL, tmpres);
	}
	else
		*(regs + (regnum^1)) = tmpres;
}

#endif /* SRC_ZXCPMSRC_Z80_CB_OPS_H_ */
