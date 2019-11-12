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
	z80_Init(zxports_out, zxports_in);
	ZXdisp_Init();

	zxsys_isrun = 1;

	register uint16_t tstates = 0;
	register uint8_t lines = 0;
	while(zxsys_isrun) {

#ifndef __SIMULATION
//		LL_GPIO_ResetOutputPin(LED_GPIO_Port, LED_Pin);
#endif
		while(!zx_newline_flag || ILI9341_DMA_busy) {
			if(tstates < 1140) // 3500000/192/16 = 1140 - number of tstates for one line drawing
				tstates += z80_step();
		}
#ifndef __SIMULATION
//		LL_GPIO_SetOutputPin(LED_GPIO_Port, LED_Pin);
#endif

		ZXdisp_drawnextline();

		if(lines++ == 192*16/50) {//50Hz interrupt
			req_int(32);
			lines = 0;
		}

		if(tstates >= 1140)
			tstates -= 1140;
	}

	mem_deInit();
}

void zxports_out(uint16_t addr, uint8_t data) {
	if((addr & 0x00ff) == 0x00fe) {
		zx_border_color = (((data << 10) & 0x0800) | ((data << 4) & 0x0040) | (data & 0x0001)) * 0x18;
	}
	else if((addr == 0x00ff) & (data == 0x00)) {
		//exit from ZX spectrum emulator
		zxsys_isrun = 0;
	}
	else if((addr == 0x00ff) & (data == 0x01)) {
		//load Z80 snapshot
		z80_load();
	}
}

uint8_t zxports_in(uint16_t addr) {
	register uint8_t res = 0xff;
	if((addr & 0x00ff) == 0x00fe) {
		res = zxkbd_scan(addr>>8);
#ifdef __SIMULATION
//		printf("Port read 0x%04x: 0x%02x\n", addr, res);
#endif
		return res;
	}
	else
		return 0xff;
}

