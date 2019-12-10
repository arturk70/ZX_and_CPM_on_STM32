/*
 * zx_system.c
 *
 *  Created on: Oct 14, 2019
 *      Author: artur
 */

#include <stdlib.h>
#include <stdio.h>
#include "zx_system.h"
#include "z80_loader.h"

uint8_t zxsys_isrun = 0;

void zxsys_Run() {
	mem_Init(MEMTYPE_ZX);
	zxdisp_init();
	z80_Init(zxports_out, zxports_in);

	zxsys_isrun = 1;

	while(zxsys_isrun) {

#ifndef __SIMULATION
//		LL_GPIO_ResetOutputPin(LED_GPIO_Port, LED_Pin);
#endif
		while(!zx_newline_flag/* || ILI9341_DMA_busy*/) {
			if(z80_tstates < 1140) // 3500000/192/16 = 1140 - number of tstates for one line drawing
				z80_step();
		}
#ifndef __SIMULATION
//		LL_GPIO_SetOutputPin(LED_GPIO_Port, LED_Pin);
#endif

		zxdisp_drawnextline();

		if(!(zxlnum & 0x3f))
			req_int(32);

		if(z80_tstates >= 1140)
			z80_tstates -= 1140;
	}
}

void zxports_out(register uint16_t addr, register uint8_t data) {
	if((addr & 0x00ff) == 0x00fe) {
		zx_border_color = (((data << 10) & 0x0800) | ((data << 4) & 0x0040) | (data & 0x0001)) * 0x18;
	}
}

uint8_t zxports_in(register uint16_t addr) {
	register uint8_t res = 0xff;
	if((addr & 0x00ff) == 0x00fe) {
		res = zxkbd_scan(addr>>8);
		if(((addr>>8) == 0x7f) && ((res & 0x1f) == 0x1c)) {//SS+Break
			z80_menu();
			return 0xff;
		}

		return res;
	}
	else
		return 0xff;
}

