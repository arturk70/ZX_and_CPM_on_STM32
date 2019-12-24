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
z80_state_t state;
int z80_tstates;

void (*port_out)(register uint32_t addr, register uint32_t data);
uint8_t (*port_in)(register uint32_t addr);

void z80_Init(void (*outfn)(register uint32_t addr, register uint32_t data), uint8_t (*infn)(register uint32_t addr)) {
	port_out = outfn;
	port_in = infn;

	state.int_req = 0;
	state.int_blocked = 0;

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
	state.halted = 0;
	state.prefix = 0;
	z80_tstates = 0;
}

void z80_interrupt() {
	z80_tstates += 13;

	state.halted = 0;

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

void z80_nmi() {
	state.halted = 0;

	IFF1 = 0;

	mem_write( --SP, PCH );
	mem_write( --SP, PCL );

	PC = 0x0066;

	z80_tstates += 11;
}

//set interrupt requests for type
void req_int(register uint32_t type) {
	state.int_req = type;
}

void z80_step() {
	RR++;
	if(state.int_req && !state.int_blocked) {
		if(state.int_req == INT_REQ) {
			if(IFF1)
				z80_interrupt();
		}
		else
			z80_nmi();
	}
	else if(state.halted) {
		z80_tstates += 4;
	}
	else {
#ifdef __SIMULATION
//		uint16_t prvPC = PC;
#endif
		state.int_blocked = 0;
		register uint32_t code = mem_read(PC++);

#ifdef __SIMULATION
//		printf("Exec 0x%04x: (0x%04x)0x%02x\n", prvPC, state.prefix, code);
#endif

		if(IS_ED_PREFIX) {
			if(code > 0x3f && code < 0x80) {
				z80_tstates += edoptstates[code-0x40];
				z80edops[code-0x40](code);
			}
			else if(code > 0x9f && code < 0xc0){
				z80_tstates += edoptstates[code-0x60];
				z80edops[code-0x60](code);
			}
			else
				NONI(code); //incorrect op NONI

			CLR_PREFIX();
		}
		else {
			if(IS_DD_PREFIX)
					hlixiyptr = &(IX);
				else if(IS_FD_PREFIX)
					hlixiyptr = &(IY);
				else
					hlixiyptr = &(HL);

			if(IS_CB_PREFIX) {
				if(hlixiyptr != &HL) {
					gixiyshift = code;
					code = mem_read(PC++);
				}

				if(code < 0x40) {
					z80_tstates += 4;
					CBSFT(code);
				}
				else {
					z80_tstates += 4;
					BIT(code);
				}

				CLR_PREFIX();
			}
			else {
				z80_tstates += optstates[code];
				z80ops[code](code);

				if(IS_PREFIX && !((code == 0xcb) | (code == 0xdd) | (code == 0xfd) | (code == 0xed)))
					CLR_PREFIX();
			}
		}
	}

	state.int_req = 0;
}

