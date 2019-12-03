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
	ILI9341_Init();
	cpmcons_init();

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
		cpmcons_deinit();
		zxsys_Run();
		cpmcons_init();
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

	cpmcons_puts("\n\n");
}
