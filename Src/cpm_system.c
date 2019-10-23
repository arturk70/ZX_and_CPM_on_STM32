/*
 * cpm_system.c
 *
 *  Created on: Oct 14, 2019
 *      Author: Artur
 */

#include "stdlib.h"
#include "cpm_system.h"

void CPMsys_Run() {
	cpmdisp_Init();
	mem_Init(MEMTYPE_CPM);

	char buf[10];
	uint8_t reg[2];
	*((uint16_t*)reg) = 0x3311;
	cpmdisp_puts("Test: ");
	cpmdisp_puts(utoa(*((uint16_t*)reg), buf, 16));
	cpmdisp_putc(' ');
	cpmdisp_puts(utoa(reg[0], buf, 16));
	cpmdisp_putc(' ');
	cpmdisp_puts(utoa(reg[1], buf, 16));
	cpmdisp_putc('\n');

}
