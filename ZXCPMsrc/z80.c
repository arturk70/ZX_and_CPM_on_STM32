/*
 * z80.c
 *
 *  Created on: Oct 14, 2019
 *      Author: artur
 */

#include "z80_ops.h"
#include "z80.h"

uint8_t regs[30];
uint16_t* hlixiyptr;
int8_t gixiyshift;
z80_state_t z80_state;
int z80_tstates;

void (*port_out)(register uint32_t addr, register uint32_t data);
uint8_t (*port_in)(register uint32_t addr);

void z80_Init(void (*outfn)(register uint32_t addr, register uint32_t data), uint8_t (*infn)(register uint32_t addr)) {
	port_out = outfn;
	port_in = infn;

	z80_state.int_req = 0;
	z80_state.int_blocked = 0;

	BC = DE = HL = 0;
	BC_= DE_= HL_= 0;
	IX = IY = 0;
	z80_reset();
}

void z80_reset() {
	AF = AF_ = 0xffff;
	I= RR = R8 = 0;
	PC = 0;
	SP = 0xffff;
	IFF1 = IFF2 = IM = 0;
	hlixiyptr = &(HL);
	gixiyshift = 0;
	z80_state.halted = 0;
	z80_tstates = 0;
}

void z80_step() {
	RR++;
	if(z80_state.int_req && !z80_state.int_blocked) {
		if(z80_state.int_req == INT_REQ) {
			if(IFF1) {// process INT request
				z80_tstates += 13;

				z80_state.halted = 0;

				IFF1 = IFF2 = 0;

				mem_write( --SP, PCH );
				mem_write( --SP, PCL );

				switch(IM) {
				case 0:
					PC = 0x0038;
					break;
				case 1:
					/* RST 38 */
					PC = 0x0038;
					break;
				case 2:
				{
					register uint32_t inttemp=(0x100*I)+0xff;
					PCL = mem_read(inttemp++); PCH = mem_read(inttemp);
					z80_tstates += 6;
					break;
				}
				}
			}
		}
		else {//process NMI request
			z80_state.halted = 0;

			IFF1 = 0;

			mem_write( --SP, PCH );
			mem_write( --SP, PCL );

			PC = 0x0066;

			z80_tstates += 11;
		}

		z80_state.int_req = 0;
	}
	else if(z80_state.halted) {
		z80_tstates += 4;
	}
	else {
		register uint32_t code;

		z80_state.int_blocked = 0;
nextcode:
		code = mem_read(PC++);

		if(code == 0xed) {
			RR++;
			hlixiyptr = &(HL);
			code = mem_read(PC++);

			if(code < 0x40 || code > 0xbf) {
				z80_tstates += 4;
				NONI(code); //incorrect op NONI
			}
			else {
				register uint32_t codeid = code-0x40;
				z80_tstates += edoptstates[codeid];
				z80edops[codeid](code);
			}
		}
		else if(code == 0xdd) {
			RR++;
			z80_tstates += 4;
			hlixiyptr = &(IX);
			goto nextcode;
		}
		else if(code == 0xfd) {
			RR++;
			z80_tstates += 4;
			hlixiyptr = &(IY);
			goto nextcode;
		}
		else if(code == 0xcb) {
			RR++;
			z80_tstates += 4;
			code = mem_read(PC++);
			if(hlixiyptr != &(HL)) {
				gixiyshift = code;
				code = mem_read(PC++);
			}

			z80_tstates += 4;
			if(code < 0x40) {
				CBSFT(code);
			}
			else {
				BIT(code);
			}

			hlixiyptr = &(HL);
		}
		else {
			z80_tstates += optstates[code];
			z80ops[code](code);

			hlixiyptr = &(HL);
		}
	}
}

