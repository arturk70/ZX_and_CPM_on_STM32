/*
 * zx_system.c
 *
 *  Created on: Oct 14, 2019
 *      Author: artur
 */

#include "stdlib.h"
#include "zx_system.h"

void zxsys_Run() {
	mem_Init(MEMTYPE_ZX);
	z80_Init(zxports_out, zxports_in);
	ZXdisp_Init();

//prepare test screen
	for(uint16_t i = 0x4000; i<0x5800; i++) {
		mem_write(i, i);
	}
	for(uint16_t i = 0x5800; i<0x5B00; i++) {
		mem_write(i, i);
	}
	for(uint8_t i=0; i<3; i++)
		for(uint8_t j=0; j<32; j++)
			for(uint8_t l=0; l<8; l++) {
				uint16_t la = (i*32+j)*8+l;
				uint16_t sa = (i<<5) | (l<<8) | j;
				uint8_t b = mem_read(0x3d00+la);
				mem_write(0x4000+sa, b);

			}
//end test screen


	while(1) {
#ifndef __SIMULATION
		LL_GPIO_ResetOutputPin(LED_GPIO_Port, LED_Pin);
#endif
		while(!zx_newline_flag || ILI9341_DMA_busy) {
			z80_step();
		}
#ifndef __SIMULATION
		LL_GPIO_SetOutputPin(LED_GPIO_Port, LED_Pin);
#endif

		if(ZXdisp_drawnextline() == 0)
			req_int(4);
	}
}

void zxports_out(uint16_t addr, uint8_t data) {

}

uint8_t zxports_in(uint16_t addr) {
	if((addr & 0x00ff) == 0x00fe) {
		return zxkbd_scan(addr>>8);
	}
	else
		return 0xff;
}

