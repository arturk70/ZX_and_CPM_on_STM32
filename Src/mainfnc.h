/*
 * mainfnc.h
 *
 *  Created on: Nov 19, 2019
 *      Author: artur
 */

#ifndef MAINFNC_H_
#define MAINFNC_H_

#include "main.h"
#include "zx_display.h"
#include "ext_memory.h"

#ifdef __SIMULATION
#define LL_TIM_IsActiveFlag_UPDATE(timer) (0xff)
#define LL_TIM_ClearFlag_UPDATE(timer) {}
#endif

#define TIM3_IRQ_HANDLER()	{ \
	if(LL_TIM_IsActiveFlag_UPDATE(TIM3)) \
		{ \
			LL_TIM_ClearFlag_UPDATE(TIM3); \
			ZX_NEWLINE_SET; \
		} \
	}

void main_init();
void main_loop();

#endif /* MAINFNC_H_ */
