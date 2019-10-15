/*
 * z80.c
 *
 *  Created on: Oct 14, 2019
 *      Author: artur
 */

#include "z80.h"

z80_registers regs;
z80_state state;

void (*port_out)(uint16_t addr, uint8_t data);
uint8_t (*port_in)(uint16_t addr);

void Z80_Init(void (*outfn)(uint16_t addr, uint8_t data), uint8_t (*infn)(uint16_t addr)) {
	port_out = outfn;
	port_in = infn;
}

void z80_interrupt() {

}

void z80_nmi() {

}

uint8_t Z80_Step() {

	return 4;
}
