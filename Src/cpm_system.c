/*
 * cpm_system.c
 *
 *  Created on: Oct 14, 2019
 *      Author: Artur
 */

#include "stdlib.h"
#include "cpm_system.h"

static void cpmsys_load() {

}

void cpmsys_Run() {
	mem_Init(MEMTYPE_CPM);
	cpmcons_clear();
	z80_Init(cpmports_out, cpmports_in);

	cpmsys_load();

	while(1) {
		z80_step();
	}
}

void cpmports_out(uint16_t addr, uint8_t data) {

}

uint8_t cpmports_in(uint16_t addr) {
	return 0xff;
}
