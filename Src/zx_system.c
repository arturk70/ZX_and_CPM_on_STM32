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

		while(!zx_newline_flag) {
			if(z80_tstates < 0)
				z80_step();
		}

		zxdisp_drawnextline();

		if(!(zxlnum & 0x3f))
			req_int(INT_REQ);

		z80_tstates -= 1140; // 3500000/192/16 = 1140 - number of tstates for one line drawing
	}
}

void zxports_out(register uint32_t addr, register uint32_t data) {
	if((addr & 0x00ff) == 0x00fe) {
		zx_border_color = (((data << 10) & 0x0800) | ((data << 4) & 0x0040) | (data & 0x0001)) * 0x18;
	}
}

uint8_t zxports_in(register uint32_t addr) {
	register uint32_t res = 0xff;
	if((addr & 0x00ff) == 0x00fe) {
		res = zxkbd_scan(addr>>8);
		if(((addr>>8) == 0x007f) && ((res & 0x1f) == 0x1c)) {//SS+Break
			z80_menu();
			return 0xff;
		}

		return res;
	}
	else
		return 0xff;
}

