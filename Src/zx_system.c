/*
 * zx_system.c
 *
 *  Created on: Oct 14, 2019
 *      Author: artur
 */

#include "zx_system.h"

uint8_t is_zx_running = 0;

void ZXsys_Run() {
	LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH1);
	LL_TIM_EnableCounter(TIM3);

	mem_Init(MEMTYPE_ZX);
	ZXdisp_Init();
	Z80_Init();

	is_zx_running = 1;

	uint32_t nxttact = SysTick->VAL / 37;

	while(1) {
		if((SysTick->VAL / 37) >= nxttact) {
			nxttact += Z80_Step();
		}
	}
}

