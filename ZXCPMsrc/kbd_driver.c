/*
 * kbd_driver.c
 *
 *  Created on: Oct 14, 2019
 *      Author: artur
 */

#include "kbd_driver.h"

static char prvchar;

static const char keytable[8][5] = {
		{'\0','z','x','c','v'},
		{'a','s','d','f','g'},
		{'q','w','e','r','t'},
		{'1','2','3','4','5'},
		{'0','9','8','7','6'},
		{'p','o','i','u','y'},
		{'\r','l','k','j','h'},
		{' ','\0','m','n','b'}
};
static const char skeytable[8][5] = {
		{'\0',':','`','?','/'},
		{'~','|','\\','{','}'},
		{'\0','\0','\0','<','>'},
		{'!','@','#','$','%'},
		{'_',')','(','\'','&'},
		{'"',';','i',']','['},
		{'\n','=','+','-','^'},
		{' ','\0','.',',','*'}
};

#ifdef __SIMULATION
uint8_t ext_kbdscans[8];
#endif

uint32_t zxkbd_scan(register uint32_t addr) {
	register uint32_t scan = 0x1f;

#ifndef __SIMULATION
	register uint32_t pA, pB;

	pB = LL_GPIO_ReadOutputPort(KBDA_8_GPIO_Port);
	LL_GPIO_WriteOutputPort(KBDA_8_GPIO_Port, (pB & 0xfffff807) | ((uint32_t)addr)<<3);//PB3-PB10
	pA = LL_GPIO_ReadInputPort(KBD_2_GPIO_Port);
	pB = LL_GPIO_ReadInputPort(KBD_0_GPIO_Port);
	scan = 0x1f & (pB | 0xfffffffc) & ((pB | 0xfffff7ff) >> 7) & (((pA | 0xfffffffe) << 2) | 0xfffffffb) & (((pA | 0xfffffffb) << 1) | 0xfffffff7);
	pB = LL_GPIO_ReadOutputPort(KBDA_8_GPIO_Port);
	LL_GPIO_WriteOutputPort(KBDA_8_GPIO_Port, (pB & 0xfffff807) | (0x000000ff<<3));
#else
	for(register uint32_t i=0;i<8;i++) {
		if(~addr & (0x01 << i))
			return ~ext_kbdscans[i];
	}
#endif

	return scan;
}

char cpmkbd_read() {
	register char res, sres;
	register uint32_t kbdscan, is_cs, is_ss;
	res=sres='\0';
	is_cs = is_ss = 0;

	for(register uint32_t i=0; i<8; i++) {
		kbdscan = ~zxkbd_scan(~(0x01<<i));
		for(register uint32_t j=0; j<5; j++) {
			if(kbdscan & (0x01<<j)) {
				if(i == 0 && j == 0)
					is_cs = 1;
				else if(i == 7 && j == 1)
					is_ss = 1;
				else {
					res = keytable[i][j];
					sres = skeytable[i][j];
				}
			}
		}
	}

	if(res) {
		if(is_cs) {
			if(is_ss) {
				res = res - 0x60; //CTRL codes
			}
			else {
				if(res == ' ')
					res = '\e';//Escape
				else if(res == '0')
					res = '\b';//Backspace
				else
					res = res - 0x20; //Upper case
			}
		}
		else {
			if(is_ss) {
				res = sres;
			}
		}
	}

	if(res == prvchar)
		res = '\0';
	else
		prvchar = res;

	return res;
}


