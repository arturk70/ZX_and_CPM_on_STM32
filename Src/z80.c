/*
 * z80.c
 *
 *  Created on: Oct 14, 2019
 *      Author: artur
 */

#include "z80_ops.h"
#include "z80.h"

z80_registers_t regs;
z80_state_t state;

void (*port_out)(register uint16_t addr, register uint8_t data);
uint8_t (*port_in)(register uint16_t addr);

void z80_Init(void (*outfn)(register uint16_t addr, register uint8_t data), uint8_t (*infn)(register uint16_t addr)) {
	port_out = outfn;
	port_in = infn;

	state.int_req = 0;
	state.nmi_req = 0;
	state.int_blocked = 0;

	BC = DE = HL = 0;
	BC_= DE_= HL_= 0;
	IX = IY = 0;
	z80_reset();
}

void z80_reset() {
	AF = AF_ = 0xffff;
	I= R = 0;
	PC = 0;
	SP = 0xffff;
	IFF1 = IFF2 = IM = 0;
	regs.hlixiyptr = &(regs.hl);
	regs.ixiyshift = 0;
	state.halted = 0;
	state.prefix = 0;

}

uint8_t z80_interrupt() {
	register uint8_t tstates = 13;

	if( IFF1) {

		if( state.halted ) { PC++; state.halted = 0; }

		IFF1 = IFF2 = 0;
		INC_R();

		mem_write( --SP, PCH );
		mem_write( --SP, PCL );

		switch(IM) {
		case 0:
			/* We assume 0xff (RST 38) is on the data bus, as the Spectrum leaves
		   it pulled high when the end-of-frame interrupt is delivered.  Only
		   the first byte is provided directly to the Z80: all remaining bytes
		   of the instruction are fetched from memory using PC, which is
		   incremented as normal.  As RST 38 takes a single byte, we do not
		   emulate fetching of additional bytes. */
			PC = 0x0038;
			break;
		case 1:
			/* RST 38 */
			PC = 0x0038;
			break;
		case 2:
			/* We assume 0xff is on the data bus, as the Spectrum leaves it pulled
		   high when the end-of-frame interrupt is delivered.  Our interrupt
		   vector is therefore 0xff. */
		{
			register uint16_t inttemp=(0x100*I)+0xff;
			PCL = mem_read(inttemp++); PCH = mem_read(inttemp);
			tstates = 19;
			break;
		}
		}
	}

	return tstates;
}

uint8_t z80_nmi() {
	if(state.halted) state.halted = 0;

	IFF1 = 0;
	INC_R();

	mem_write( --SP, PCH );
	mem_write( --SP, PCL );

	PC = 0x0066;

	return 11;
}

//set interrupt requests for tstates time
void req_int(register uint32_t tstates) {
	if(state.int_req > 0)
		state.int_req += tstates;
	else
		state.int_req = tstates;
}

void req_nmi(register uint32_t tstates) {
	if(state.nmi_req > 0)
		state.nmi_req += tstates;
	else
		state.nmi_req = tstates;
}

uint8_t z80_step() {
	register int8_t tstates = 0;

	if((state.nmi_req>0) && !state.int_blocked) {
#ifdef __SIMULATION
//		printf("Process NMI at PC=0x%04x\n", PC);
#endif
		tstates += z80_nmi();
	}
	else if((state.int_req>0) && IFF1 && !state.int_blocked) {
#ifdef __SIMULATION
//		printf("Process INT at PC=0x%04x, IM=0x%02x\n", PC, IM);
#endif
		tstates += z80_interrupt();
	}
	else if(state.halted) {
		tstates += 4;
		INC_R();
	}
	else {
#ifdef __SIMULATION
//		uint16_t prvPC = PC;
#endif
		state.int_blocked = 0;
		register uint8_t code = mem_read(PC++);
		INC_R();

		if(IS_DDFDCB_PREFIX) {
			regs.ixiyshift = code;
			code = mem_read(PC++);
		}

#ifdef __SIMULATION
//		printf("Exec 0x%04x: (0x%04x)0x%02x\n", prvPC, state.prefix, code);
#endif

		if(IS_DD_PREFIX)
			regs.hlixiyptr = &(regs.ix);
		else if(IS_FD_PREFIX)
			regs.hlixiyptr = &(regs.iy);
		else
			regs.hlixiyptr = &(regs.hl);

		if(IS_ED_PREFIX) {
			if((code < 0x40) || (code > 0xbf) || (code > 0x7f && code < 0xa0))
				tstates += NONI(code); //incorrect op NONI
			else {
				if(code < 0x80) {
					tstates += edoptstates[code-0x40];
					tstates += z80edops[code-0x40](code);
				}
				else {
					tstates += edoptstates[code-0x60];
					tstates += z80edops[code-0x60](code);
				}
			}

			CLR_PREFIX();
		}
		else if(IS_CB_PREFIX) {
			if(code < 0x40) {
				tstates += 4;
				tstates += CBSFT(code);
			}
			else {
				tstates += 4;
				tstates += BIT(code);
			}

			CLR_PREFIX();
		}
		else {
			tstates += optstates[code];
			tstates += z80ops[code](code);

			if(IS_PREFIX && !((code == 0xcb) | (code == 0xdd) | (code == 0xfd) | (code == 0xed)))
				CLR_PREFIX();
		}
	}

	if(state.nmi_req > tstates)
		state.nmi_req -= tstates;
	else
		state.nmi_req = 0;

	if(state.int_req > tstates)
		state.int_req -= tstates;
	else
		state.int_req = 0;

	return tstates;
}

