/*
 * z80.c
 *
 *  Created on: Oct 14, 2019
 *      Author: artur
 */

#include <stdio.h>
#include "z80_ops.h"
#include "z80.h"
#include "memory.h"

z80_registers regs;
z80_state state;

void (*port_out)(uint16_t addr, uint8_t data);
uint8_t (*port_in)(uint16_t addr);

void z80_Init(void (*outfn)(uint16_t addr, uint8_t data), uint8_t (*infn)(uint16_t addr)) {
	port_out = outfn;
	port_in = infn;

	BC = DE = HL = 0;
	BC_= DE_= HL_= 0;
	IX = IY = 0;
	regs.memptr.w = 0;	/* TODO: confirm if this happens on soft reset */
	z80_reset();
}

void z80_reset() {
	AF = AF_ = 0xffff;
	I= R = R7 = 0;
	PC = 0;
	SP = 0xffff;
	IFF1 = IFF2 = IM = 0;
	state.halted = 0;
	state.iff2_read = 0;

	state.interrupts_enabled_at = -1;
}

uint8_t z80_interrupt() {
	uint8_t tstates = 0;
	/* An interrupt will occur if IFF1 is set and the /INT line hasn't
	     gone high again. On a Timex machine, we also need the SCLD's
	     INTDISABLE to be clear */
	if( IFF1 /*&& tstates < machine_current->timings.interrupt_length*/) {

		if ( state.iff2_read) {
			/* We just executed LD A,I or LD A,R, causing IFF2 to be copied to the
		 parity flag.  This occured whilst accepting an interrupt.  For NMOS
		 Z80s only, clear the parity flag to reflect the fact that IFF2 would
		 have actually been cleared before its value was transferred by LD A,I
		 or LD A,R.  We cannot do this when emulating LD itself as we cannot
		 tell whether the next instruction will be interrupted. */
			F &= ~FLAG_P;
		}

		if( state.halted ) { PC++; state.halted = 0; }

		IFF1 = IFF2 = 0;
		R++;

		mem_write( --SP, PCH ); mem_write( --SP, PCL );

		switch(IM) {
		case 0:
			/* We assume 0xff (RST 38) is on the data bus, as the Spectrum leaves
		   it pulled high when the end-of-frame interrupt is delivered.  Only
		   the first byte is provided directly to the Z80: all remaining bytes
		   of the instruction are fetched from memory using PC, which is
		   incremented as normal.  As RST 38 takes a single byte, we do not
		   emulate fetching of additional bytes. */
			PC = 0x0038;
			tstates = 13;
			break;
		case 1:
			/* RST 38 */
			PC = 0x0038;
			tstates = 13;
			break;
		case 2:
			/* We assume 0xff is on the data bus, as the Spectrum leaves it pulled
		   high when the end-of-frame interrupt is delivered.  Our interrupt
		   vector is therefore 0xff. */
		{
			uint16_t inttemp=(0x100*I)+0xff;
			PCL = mem_read(inttemp++); PCH = mem_read(inttemp);
			tstates = 19;
			break;
		}
		}

		regs.memptr.w = PC;

		return tstates;
	}

	return 0;
}

uint8_t z80_nmi() {
	if( state.halted ) state.halted = 0;

	IFF1 = 0;
	R++;

	mem_write( --SP, PCH );
	mem_write( --SP, PCL );

	PC = 0x0066;

	return 11;
}

uint8_t Z80_Step() {
	uint8_t code = mem_read(PC);

#ifdef __SIMULATION
	printf("Exec 0x%04x: 0x%02x\n", PC, code);
#endif
	z80ops[code](code);

	return optstates[code];
}

