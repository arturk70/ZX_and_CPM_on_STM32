/*
 * zx_system.c
 *
 *  Created on: Oct 14, 2019
 *      Author: artur
 */

#include "zx_system.h"

uint8_t is_zx_running = 0;

void zxsys_Run() {
	LL_TIM_EnableUpdateEvent(TIM3);
	LL_TIM_EnableCounter(TIM3);
	LL_TIM_EnableIT_UPDATE(TIM3);

	mem_Init(MEMTYPE_ZX);
	ZXdisp_Init();
	Z80_Init(zxports_out, zxports_in);

	is_zx_running = 1;

	uint32_t nxttact = SysTick->VAL / 37;

	while(1) {
		if((SysTick->VAL / 37) >= nxttact) {
			nxttact += Z80_Step();
		}
	}
}

void zxports_out(uint16_t addr, uint8_t data) {

}

uint8_t zxports_in(uint16_t addr) {
	return 0xff;
}

