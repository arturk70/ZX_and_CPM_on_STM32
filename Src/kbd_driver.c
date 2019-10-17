/*
 * kbd_driver.c
 *
 *  Created on: Oct 14, 2019
 *      Author: artur
 */

#include "kbd_driver.h"

static char prvchar;

#ifdef __SIMULATION
uint8_t ext_kbdscans[8];
#endif

uint8_t zxkbd_scan(uint8_t addr) {
	uint8_t scan = 0;

#ifndef __SIMULATION
	uint32_t pA, pB;

	pB = LL_GPIO_ReadOutputPort(KBDA_8_GPIO_Port);
	LL_GPIO_WriteOutputPort(KBDA_8_GPIO_Port, (pB & 0xfffff807) | ((uint32_t)addr)<<3);
	pA = LL_GPIO_ReadInputPort(KBD_2_GPIO_Port);
	pB = LL_GPIO_ReadInputPort(KBD_0_GPIO_Port);
	scan = 0x1f & (pB | 0xfffffffc) & ((pB | 0xfffff7ff) >> 7) & (((pA | 0xfffffffe) << 2) | 0xfffffffb) & (((pA | 0xfffffffb) << 1) | 0xfffffff7);
	pB = LL_GPIO_ReadOutputPort(KBDA_8_GPIO_Port);
	LL_GPIO_WriteOutputPort(KBDA_8_GPIO_Port, (pB & 0xfffff807) | (0x000000ff<<3));
#else
	for(uint8_t i=0;i<8;i++) {
		if(~addr & (0x01 << i))
			return ~ext_kbdscans[i];
	}
#endif

	return scan;
}

char cpmkbd_read() {
	char res='\0';
	uint8_t kbdscans[8];

	for(uint8_t i=0; i<8; i++)
		kbdscans[i] = ~zxkbd_scan(~(0x01<<i));

	if(kbdscans[6] & 0x01)
		res = '\n';//Enter
	else {
		if(kbdscans[7] & 0x01) {
			if(IS_CONTROL)
				res = (char)0x18;//Ctrl-C
			else
				res = ' ';
		}
		else {
			res = (kbdscans[3] & 0x01) ? '1' : ((kbdscans[3] & 0x02) ? '2' : ((kbdscans[3] & 0x04) ? '3' : ((kbdscans[3] & 0x08) ? '4' : ((kbdscans[3] & 0x10) ? '5' : res))));
			res = (kbdscans[4] & 0x01) ? '0' : ((kbdscans[4] & 0x02) ? '9' : ((kbdscans[4] & 0x04) ? '8' : ((kbdscans[4] & 0x08) ? '7' : ((kbdscans[4] & 0x10) ? '6' : res))));
			if(res != '\0') {
				if(IS_CONTROL) {
					if(IS_SHIFT) {
						if(res == '0')
							res = (char)0x08;//backspace
						else if(res == '1')
							res = (char)0x1b;//ESC
						else
							res = '\0';
					}
					else {
						switch (res) {
						case '1': res = '!'; break;
						case '2': res = '@'; break;
						case '3': res = '#'; break;
						case '4': res = '$'; break;
						case '5': res = '%'; break;
						case '6': res = '&'; break;
						case '7': res = '\''; break;
						case '8': res = '('; break;
						case '9': res = ')'; break;
						case '0': res = '_'; break;
						}
					}
				}
			}
			else {
				res =                           (kbdscans[0] & 0x02) ? 'z' : ((kbdscans[0] & 0x04) ? 'x' : ((kbdscans[0] & 0x08) ? 'c' : ((kbdscans[0] & 0x10) ? 'v' : res)));
				res = (kbdscans[1] & 0x01) ? 'a' : ((kbdscans[1] & 0x02) ? 's' : ((kbdscans[1] & 0x04) ? 'd' : ((kbdscans[1] & 0x08) ? 'f' : ((kbdscans[1] & 0x10) ? 'g' : res))));
				res = (kbdscans[2] & 0x01) ? 'q' : ((kbdscans[2] & 0x02) ? 'w' : ((kbdscans[2] & 0x04) ? 'e' : ((kbdscans[2] & 0x08) ? 'r' : ((kbdscans[2] & 0x10) ? 't' : res))));
				res = (kbdscans[5] & 0x01) ? 'p' : ((kbdscans[5] & 0x02) ? 'o' : ((kbdscans[5] & 0x04) ? 'i' : ((kbdscans[5] & 0x08) ? 'u' : ((kbdscans[5] & 0x10) ? 'y' : res))));
				res =                           (kbdscans[6] & 0x02) ? 'l' : ((kbdscans[6] & 0x04) ? 'k' : ((kbdscans[6] & 0x08) ? 'j' : ((kbdscans[6] & 0x10) ? 'h' : res)));
				res =                                                     (kbdscans[7] & 0x04) ? 'm' : ((kbdscans[7] & 0x08) ? 'n' : ((kbdscans[7] & 0x10) ? 'b' : res));
				if(res != '\0') {
					if(IS_SHIFT) {
						if(IS_CONTROL) {
							res = res - 0x60; //CTRL codes
						}
						else {
							res = res - 0x20; //Upper case
						}
					}
					else {
						if(IS_CONTROL) {
							switch (res) {
							case 'r': res = '<'; break;
							case 't': res = '>'; break;
							case 'y': res = '['; break;
							case 'u': res = ']'; break;
							case 'o': res = ';'; break;
							case 'p': res = '"'; break;

							case 'a': res = '~'; break;
							case 's': res = '|'; break;
							case 'd': res = '\\'; break;
							case 'f': res = '{'; break;
							case 'g': res = '}'; break;
							case 'h': res = '^'; break;
							case 'j': res = '-'; break;
							case 'k': res = '+'; break;
							case 'l': res = '='; break;

							case 'z': res = ':'; break;
							case 'x': res = '`'; break;
							case 'c': res = '?'; break;
							case 'v': res = '/'; break;
							case 'b': res = '*'; break;
							case 'n': res = ','; break;
							case 'm': res = '.'; break;
							default: res = '\0';
							}
						}
					}
				}
			}
		}
	}

	if(res == prvchar)
		res = '\0';
	else
		prvchar = res;

	return res;
}


