/*
 * z80_loader.c
 *
 *  Created on: Nov 12, 2019
 *      Author: artur
 */
#include "stdlib.h"
#include "fatfs.h"
#include "z80_loader.h"

void z80_load() {
	register char sym = '\0';
	char strbuf[20];
	register uint8_t inpptr, fnptr = 9;
	char fname[22] = "0:/ZX/Z80";

	cpmdisp_Init();

	while(1) {
		while(cpmkbd_read() != '\0');

		cpmdisp_puts("\n[d] for list, [l <name>] for load\n");
		cpmdisp_putc('>');
		inpptr = 0;
		while(1) {
			do { sym = cpmkbd_read(); } while('\0' == sym);

			if('\n' == sym)
				break;
			else {
				if(inpptr < 20) {
					strbuf[inpptr++] = sym;
					strbuf[inpptr] = '\0';
					cpmdisp_putc(sym);
				}
			}
		}
		cpmdisp_putc('\n');

		if(strbuf[0] == 'd' && strbuf[1] == '\0') {//dir files from 0:/ZX/Z80/
			DIR dir;
			FILINFO fi;
			register FRESULT res;

			res = f_opendir(&dir, fname);
			if(res != FR_OK) {
				cpmdisp_puts("Error open dir \"");
				cpmdisp_puts(fname);
				cpmdisp_puts("\": ");
				cpmdisp_puts(utoa(res, strbuf, 10));
				cpmdisp_putc('\n');
			}
			else {
				do {
					f_readdir(&dir, &fi);
					if(fi.fname[0] == '\0')
						break;

					if(!(fi.fattrib & (AM_DIR | AM_HID | AM_SYS))) {
						cpmdisp_puts(fi.fname);
						cpmdisp_putc('\n');
					}
				} while(res == FR_OK);
			}

			f_closedir(&dir);
		}
		else if(strbuf[0] == 'l' && strbuf[1] == ' ') {//load file
			fname[fnptr++] = '/';
			inpptr = 2;
			while(strbuf[inpptr] != '\0') {
				fname[fnptr++] = strbuf[inpptr++];
			}
			fname[fnptr++] = '.';
			fname[fnptr++] = 'Z';
			fname[fnptr++] = '8';
			fname[fnptr++] = '0';
			fname[fnptr] = '\0';

			cpmdisp_puts("Loading file ");
			cpmdisp_puts(fname);
			cpmdisp_putc('\n');

			register FRESULT res;
			FIL z80f;
			res = f_open(&z80f, fname, FA_READ);
			if(res != FR_OK) {
				cpmdisp_puts("Error load file: ");
				cpmdisp_puts(utoa(res, strbuf, 10));
				cpmdisp_putc('\n');
			}
			else {

#define BUFSIZE	512
				uint8_t buf[BUFSIZE], b12;
				UINT size = BUFSIZE;

				res = f_read(&z80f, buf, 30, &size);
				if(res != FR_OK) {
					cpmdisp_puts("Error read file: ");
					cpmdisp_puts(utoa(res, strbuf, 10));
					cpmdisp_putc('\n');
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
					R = buf[11];
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

					if(b12 == 0xff)
						b12 = 0x01;
					R = (R & 0x7f) | (b12 << 7);
					zx_border_color = (((b12 << 9) & 0x0800) | ((b12 << 3) & 0x0040) | ((b12 >> 1) & 0x0001)) * 0x18;
					// (b12 & 0x20) --- RLE compression used

					register uint16_t addr = 0x4000;
					while(f_read(&z80f, buf, BUFSIZE, &size) == FR_OK) {
						for(uint16_t i=0;i<size;i++) {
							//TODO RLE decompression
							mem_write(addr++, buf[i]);
						}

						if(size < BUFSIZE) break;
					}
				}
			}
			f_close(&z80f);
//			sleep(2);
			break;
		}
	}

	ILI9341_clear(BG_COLOR);
}
