/*
 * kbd_driver.c
 *
 *  Created on: Oct 14, 2019
 *      Author: artur
 */

#include "kbd_driver.h"

uint8_t zxkbd_scan(uint8_t addr) {
	uint8_t scan = 0;
	uint32_t pA, pB;

	pB = LL_GPIO_ReadOutputPort(KBDA_8_GPIO_Port);
	LL_GPIO_WriteOutputPort(KBDA_8_GPIO_Port, (pB & 0xfffff807) | ((uint32_t)addr)<<3);
	pA = LL_GPIO_ReadInputPort(KBD_2_GPIO_Port);
	pB = LL_GPIO_ReadInputPort(KBD_0_GPIO_Port);
	scan = 0x1f & (pB | 0xfffffffc) & ((pB | 0xfffff7ff) >> 7) & (((pA | 0xfffffffe) << 2) | 0xfffffffb) & (((pA | 0xfffffffb) << 1) | 0xfffffff7);
	pB = LL_GPIO_ReadOutputPort(KBDA_8_GPIO_Port);
	LL_GPIO_WriteOutputPort(KBDA_8_GPIO_Port, (pB & 0xfffff807) | (0x000000ff<<3));

	return scan;
}

char cpmkbd_read() {
	char res='\0';
	uint8_t scan[8];

	for(uint8_t i=0; i<8; i++)
		scan[i] = ~zxkbd_scan(~(0x01<<i));

	if(scan[6] & 0x01) return '\n';//Enter
	if(scan[7] & 0x01) {
		if(IS_CONTROL)
			return (char)0x18;//Ctrl-C
		else
			return ' ';
	}

	res = (scan[3] & 0x01) ? '1' : ((scan[3] & 0x02) ? '2' : ((scan[3] & 0x04) ? '3' : ((scan[3] & 0x08) ? '4' : ((scan[3] & 0x10) ? '5' : res))));
	res = (scan[4] & 0x01) ? '0' : ((scan[4] & 0x02) ? '9' : ((scan[4] & 0x04) ? '8' : ((scan[4] & 0x08) ? '7' : ((scan[4] & 0x10) ? '6' : res))));
	if(res != 0x00) {
		if(IS_SHIFT) {
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

			return res;
		}
		else {
			if(IS_CONTROL) {
				if(res == '0') return (char)0x08;//backspace
				if(res == '1') return (char)0x1b;//ESC
				return 0x00;
			}
			else
				return res;
		}
	}


	res =                           (scan[0] & 0x02) ? 'z' : ((scan[0] & 0x04) ? 'x' : ((scan[0] & 0x08) ? 'c' : ((scan[0] & 0x10) ? 'v' : res)));
	res = (scan[1] & 0x01) ? 'a' : ((scan[1] & 0x02) ? 's' : ((scan[1] & 0x04) ? 'd' : ((scan[1] & 0x08) ? 'f' : ((scan[1] & 0x10) ? 'g' : res))));
	res = (scan[2] & 0x01) ? 'q' : ((scan[2] & 0x02) ? 'w' : ((scan[2] & 0x04) ? 'e' : ((scan[2] & 0x08) ? 'r' : ((scan[2] & 0x10) ? 't' : res))));
	res = (scan[5] & 0x01) ? 'p' : ((scan[5] & 0x02) ? 'o' : ((scan[5] & 0x04) ? 'i' : ((scan[5] & 0x08) ? 'u' : ((scan[5] & 0x10) ? 'y' : res))));
	res =                           (scan[6] & 0x02) ? 'l' : ((scan[6] & 0x04) ? 'k' : ((scan[6] & 0x08) ? 'j' : ((scan[6] & 0x10) ? 'h' : res)));
	res =                                                     (scan[7] & 0x04) ? 'm' : ((scan[7] & 0x08) ? 'n' : ((scan[7] & 0x10) ? 'b' : res));
	if(res != 0x00) {
		if(IS_CONTROL) {
			if(IS_SHIFT) {
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
				}

				return res;
			}
			else {
				if((res >='a') && (res <='z'))
					return res - 0x60; //CTRL codes
				else {

				}
			}
		}
		else {
			if(IS_SHIFT)
				return res - 0x20; //Upper case
			else
				return res;
		}
	}

	return res;
}


