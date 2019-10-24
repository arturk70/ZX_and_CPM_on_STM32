/*
 * z80.c
 *
 *  Created on: Oct 14, 2019
 *      Author: artur
 */

#include "z80_ops.h"
#include "z80.h"

z80_registers regs;
z80_state state;

void (*port_out)(uint16_t addr, uint8_t data);
uint8_t (*port_in)(uint16_t addr);

void z80_Init(void (*outfn)(uint16_t addr, uint8_t data), uint8_t (*infn)(uint16_t addr)) {
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
//	state.iff2_read = 0;
	state.prefix = 0;

}

uint8_t z80_interrupt() {
	uint8_t tstates = 13;

	if( IFF1) {

//		if ( state.iff2_read) {
//			/* We just executed LD A,I or LD A,R, causing IFF2 to be copied to the
//		 parity flag.  This occured whilst accepting an interrupt.
//		 We cannot do this when emulating LD itself as we cannot
//		 tell whether the next instruction will be interrupted. */
//			F &= ~FLAG_P;
//		}

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
			uint16_t inttemp=(0x100*I)+0xff;
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
void req_int(uint32_t tstates) {if(state.int_req >0) state.int_req += tstates; else state.int_req = tstates;}
void req_nmi(uint32_t tstates) {if(state.nmi_req >0) state.nmi_req += tstates; else state.nmi_req = tstates;}

uint8_t z80_step() {
	uint8_t tstates;

	if((state.nmi_req>0) && !state.int_blocked) {
#ifdef __SIMULATION
		printf("Process NMI at PC=0x%04x\n", PC);
#endif
		tstates = z80_nmi();
	}
	else if((state.int_req>0) && IFF1 && !state.int_blocked) {
#ifdef __SIMULATION
		printf("Process INT at PC=0x%04x, IM=0x%02x\n", PC, IM);
#endif
		tstates = z80_interrupt();
	}
	else if(state.halted) {
		tstates = 4;
		INC_R();
	}
	else {
#ifdef __SIMULATION
		uint16_t prvPC = PC;
#endif
		state.int_blocked = 0;
		uint8_t code = mem_read(PC++);
		INC_R();

#ifdef __SIMULATION
//		printf("Exec 0x%04x: (0x%04x)0x%02x\n", PC-1, state.prefix, code);
#endif

		if(IS_DD_PREFIX)
			regs.hlixiyptr = &(regs.ix);
		if(IS_FD_PREFIX)
			regs.hlixiyptr = &(regs.iy);

		if(IS_ED_PREFIX) {
			if((code < 0x40) || (code > 0xbf) || (code > 0x7f && code < 0xa0))
				tstates = NONI(code); //incorrect op NONI
			else {
				if(code < 0x80)
					tstates = z80edops[code-0x40](code);
				else
					tstates = z80edops[code-0x60](code);
			}
		}
		else if(IS_CB_PREFIX) {
			if(IS_DDFD_PREFIX) {
				regs.ixiyshift = code;
				code = mem_read(PC++);
			}
			if(code < 0x40)
				tstates = CBSFT(code);
			else
				tstates = BIT(code);
		}
		else
			tstates = z80ops[code](code);

#ifdef __SIMULATION
		if(prvPC < 0x4000 && PC >= 0x4000)
			printf("Out of ROM by cmd 0x%04x: (0x%04x)0x%02x regs: 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%04x 0x%02x 0x%02x 0x%02x 0x%02x\n",
					prvPC, state.prefix, code, BC, DE, HL, AF, IX, IY, SP, PC, I, IFF1, IFF2, IM);
#endif

		if(IS_DDFD_PREFIX && (code != 0xcb))
					regs.hlixiyptr = &(regs.hl);

		if(IS_PREFIX && !((code == 0xcb) | (code == 0xdd) | (code == 0xfd) | (code == 0xed)))
			CLR_PREFIX();
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

