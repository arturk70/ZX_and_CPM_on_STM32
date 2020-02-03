/*
 * mainfnc.c
 *
 *  Created on: Nov 19, 2019
 *      Author: artur
 */

#include "mainfnc.h"
#include "fatfs.h"

#include <stdlib.h>
#include "cpm_system.h"
#include "zx_system.h"
#include "memory.h"
#include "kbd_driver.h"

void main_init() {
#ifndef __SIMULATION
	LL_TIM_EnableUpdateEvent(TIM3);
	LL_TIM_EnableCounter(TIM3);
	LL_TIM_EnableIT_UPDATE(TIM3);
#endif
	ILI9341_Init();
	cpmcons_clear();

	retUSER = f_mount(&USERFatFS, "0", 1);
	if(retUSER != FR_OK) {
		cpmcons_errmsg(retUSER, "mount drive");
	}
}

void main_loop() {
	register char sym = '\0';
	cpmcons_puts("\n<6> for ZX Spectrum\n" \
			"<7> for CP/M\n" \
			"<0> for brightness\n>");

	sym = cpmcons_getc();
	cpmcons_putc(sym);
	if(sym == '6') {
		zxsys_Run();

////memory test
//		extmem_Init();
//
//		uint16_t addr[200];
//		register uint16_t gaddr;
//		register uint8_t rd;
//		char buf[10];
//
//		cpmcons_putc('\n');
//		for(register uint8_t i=0;i<=200;i++) {
//			register uint8_t j;
//			do {
//				gaddr = (uint16_t)rand() & 0x3fff;
//				for(j=0;j<i;j++) {
//					if(addr[j] == gaddr)
//						break;
//				}
//			} while(j != i);
//
//			addr[i] = gaddr;
//			cpmcons_puts(utoa(gaddr, buf, 16));
//			cpmcons_putc(' ');
//		}
//		cpmcons_puts("\nPrepared\n");
//		for(register uint8_t i=0;i<=200;i++) {
//			extmem_write(addr[i], i);
//		}
//		cpmcons_puts("Writed\n");
//		for(register uint8_t i=0;i<=200;i++) {
//			rd = extmem_read(addr[i]);
//			if(rd != i) {
//				cpmcons_puts("Error ");
//				cpmcons_puts(utoa(addr[i], buf, 16));
//				cpmcons_putc(' ');
//				cpmcons_puts(utoa(i, buf, 16));
//				cpmcons_putc(' ');
//				cpmcons_puts(utoa(rd, buf, 16));
//				cpmcons_putc('\n');
//			}
//		}
//		cpmcons_puts("Readed\n");
	}
	else if(sym == '7') {
		cpmsys_Run();
	}
	else if(sym == '0') {
		cpmcons_puts("\n\nBrightness[1-0]=[10%-100%]>");
		sym = cpmcons_getc();
		cpmcons_putc(sym);
		sym -= '0';
		if(sym >=0 && sym <=9) {
			if(0 == sym)
				sym = 10;
			ILI9341_setLEDpwm(sym*100);
		}
	}

	cpmcons_clear();
}
