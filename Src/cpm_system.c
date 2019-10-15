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

	uint32_t nxttact = SysTick->VAL / 18;
	uint32_t ticks = SysTick->VAL;

	for(int i=0;i<1000000;i++) {
		if((SysTick->VAL / 18) >= nxttact) {
			nxttact = SysTick->VAL / 18 + Z80_Step();
		}
	}

	ticks = SysTick->VAL - ticks;

	char buf[20];
	cpmdisp_puts("tacts ");
	cpmdisp_puts(utoa(nxttact, buf, 10));
	cpmdisp_puts(" ticks ");
	cpmdisp_puts(utoa(ticks, buf, 10));
	cpmdisp_putc('\n');
}
