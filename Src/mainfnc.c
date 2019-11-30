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
	cpmcons_puts("\nPress <6> for ZX Spectrum\n" \
			"Press <7> for CP/M\n" \
			"Press <0> for brightness\n>");

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
//	else if(sym == '9') {
//  			//TODO for test only - remove
//  			cpmcons_puts("\nEnter dir:>");
//
//  			sym = '\0';
//  			char dpath[100];
//  			dpath[0] = '0';
//  			dpath[1] = ':';
//  			dpath[2] = '\0';
//  			uint8_t dpptr = 2;
//  			cpmcons_gets(&dpath[2], 97);
//
//  			DIR dir;
//  			FILINFO fi;
//
//  			retUSER = f_opendir(&dir, dpath);
//  			if(retUSER != FR_OK) {
//  				cpmcons_errmsg(retUSER, "open dir");
//  			}
//
//  			do {
//  				retUSER = f_readdir(&dir, &fi);
//  				if(fi.fname[0] == '\0')
//  					break;
//
//  				if(fi.fattrib & AM_DIR)
//  					cpmcons_putc('/');
//  				cpmcons_puts(fi.fname);
//  				cpmcons_putc(' ');
//  				if(fi.fattrib & AM_DIR)
//  					cpmcons_putc('\n');
//  				else {
//  					cpmcons_puts(utoa(fi.fsize, buf, 10));
//  					cpmcons_puts("b\n");
//  				}
//  			} while(retUSER == FR_OK);
//
//  			if(retUSER != FR_OK)
//  				cpmcons_errmsg(retUSER, "read dir");
//
//  			f_closedir(&dir);
//	}
	else if(sym == '0') {
		cpmcons_puts("\n\nEnter brightness[1-0]=[10%-100%]>");
		sym = cpmcons_getc();
		cpmcons_putc(sym);
		if('0' == sym)
			ILI9341_setLEDpwm(1000);
		else if((sym >= '1') &(sym <= '9'))
			ILI9341_setLEDpwm((sym-'1')*100+100);
	}

	cpmcons_puts("\n\n");
}
