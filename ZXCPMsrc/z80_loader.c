/*
 * z80_loader.c
 *
 *  Created on: Nov 12, 2019
 *      Author: artur
 */
#include "stdlib.h"
#include "fatfs.h"
#include "z80_loader.h"

#define BUFSIZE	128

static uint8_t z80_loadfile(const char *fname) {
	retUSER = f_open(&USERFile, fname, FA_READ);
	if(retUSER != FR_OK) {
		return retUSER;
	}
	else {
		uint8_t buf[BUFSIZE], b12;
		UINT size = BUFSIZE;

		retUSER = f_read(&USERFile, buf, 30, &size);
		if(retUSER != FR_OK) {
			return retUSER;
		}
		else {
			A = buf[0];
			F = buf[1];
			C = buf[2];
			B = buf[3];
			L = buf[4];
			H = buf[5];
			PCL = buf[6];
			PCH = buf[7];
			SPL = buf[8];
			SPH = buf[9];
			I = buf[10];
			RR = buf[11] & 0x7f;
			b12 = buf[12];
			E = buf[13];
			D = buf[14];
			C_ = buf[15];
			B_ = buf[16];
			E_ = buf[17];
			D_ = buf[18];
			L_ = buf[19];
			H_ = buf[20];
			A_ = buf[21];
			F_ = buf[22];
			IYL = buf[23];
			IYH = buf[24];
			IXL = buf[25];
			IXH = buf[26];
			IFF1 = buf[27];
			IFF2 = buf[28];
			IM = buf[29] & 0x03;

			R8 = b12 << 7;
			zx_border_color = (((b12 << 9) & 0x0800) | ((b12 << 3) & 0x0040) | ((b12 >> 1) & 0x0001)) * 0x18;
			// (b12 & 0x20) --- RLE compression used

			register uint16_t addr = 0x4000;
			register uint8_t num = 0, edflag = 0, rle = 0;
			while(f_read(&USERFile, buf, BUFSIZE, &size) == FR_OK) {
				for(register uint16_t i=0;i<size;i++) {
					if(buf[i] == 0x00 && buf[i+1] == 0xed && buf[i+2] == 0xed && buf[i+3] == 0x00) {//end of Z80 marker found
						break;
					}

					if(addr == 0) { //end of memory reached
						return 0xff;
					}

					if(rle) {
						if(num) {
							rle = buf[i];
							for(register uint8_t j=0; j<num; j++)
								mem_write(addr++, rle);

							num = 0;
							rle = 0;
						}
						else {
							num = buf[i];
							if(num < 0x05 && !(num == 0x02 && buf[i+1] == 0xed)) {
								return 0xfe;
							}
						}

					}
					else {
						if(edflag) {
							if(buf[i] == 0xed) {//rle block found
								rle = 1;
							}
							else {
								mem_write(addr++, 0xed);
								mem_write(addr++, buf[i]);
							}

							edflag = 0;
						}
						else {
							if((b12 & 0x20) && (buf[i] == 0xed)) {
								edflag = 1;
							}
							else
								mem_write(addr++, buf[i]);
						}
					}
				}

				if(size < BUFSIZE) break;
			}
		}
	}
	f_close(&USERFile);

	return 0;
}

void z80_menu() {
	register char sym = '\0';
	char fname[22] = "0:/ZX/Z80";

	zxdisp_deinit();
	cpmcons_init();

	while(1) {
		while(cpmkbd_read() != '\0');

		cpmcons_puts("\n[d]ir, [l]oad, [r]eturn, [e]xit\n>");
		sym = cpmcons_getc();
		cpmcons_putc(sym);
		cpmcons_putc('\n');

		if(sym == 'e') {//exit
			zxsys_isrun = 0;
			break;
		}
		else if(sym == 'r') {//return
			cpmcons_deinit();
			zxdisp_init();
			break;
		}
		else if(sym == 'd') {//dir files from 0:/ZX/Z80/
#ifdef __SIMULATION
			ffDIR dir;
#else
			DIR dir;
#endif
			FILINFO fi;

			retUSER = f_opendir(&dir, fname);
			if(retUSER != FR_OK)
				cpmcons_errmsg(retUSER, "open dir");
			else {
				register uint8_t pos = 0;
				do {
					retUSER = f_readdir(&dir, &fi);
					if(fi.fname[0] == '\0')
						break;

					if(!(fi.fattrib & (AM_DIR | AM_HID | AM_SYS))) {
						cpmcons_puts(fi.fname);
						if(++pos > 2) {
							cpmcons_putc('\n');
							pos = 0;
						}
						else
							cpmcons_putc('\t');
					}
				} while(retUSER == FR_OK);

				if(retUSER != FR_OK)
					cpmcons_errmsg(retUSER, "read dir");
			}

			f_closedir(&dir);

			cpmcons_putc('\n');
		}
		else if(sym == 'l') {//load file
			cpmcons_puts("Enter file name w/o ext>");

			char strbuf[9];
			register uint8_t inpptr = 0, fnptr = 9;
			cpmcons_gets(strbuf, 8);
			cpmcons_putc('\n');
			fname[fnptr++] = '/';
			while(strbuf[inpptr] != '\0') {
				fname[fnptr++] = strbuf[inpptr++];
			}
			fname[fnptr++] = '.';
			fname[fnptr++] = 'Z';
			fname[fnptr++] = '8';
			fname[fnptr++] = '0';
			fname[fnptr] = '\0';

			if((retUSER = z80_loadfile(fname)) == 0) {
				cpmcons_deinit();
				zxdisp_init();
				break;
			}
			else
				cpmcons_errmsg(retUSER, "load .z80 file");
		}
	}
}
