/*
 * kbd_driver.h
 *
 *  Created on: Oct 13, 2019
 *      Author: Artur
 */

#ifndef KBD_DRIVER_H_
#define KBD_DRIVER_H_

#include "main.h"

#define IS_SHIFT	(scan[0] == 0x1e)
#define IS_CONTROL	(scan[7] == 0x1d)


uint8_t zxkbd_scan(uint8_t addr) {
	uint8_t scan = 0;

	if(!(addr & 0b00000001)) {
		LL_GPIO_ResetOutputPin(KBDA_8_GPIO_Port, KBDA_8_Pin);
		if(LL_GPIO_IsInputPinSet(KBD_4_GPIO_Port, KBD_4_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_3_GPIO_Port, KBD_3_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_2_GPIO_Port, KBD_2_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_1_GPIO_Port, KBD_1_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_0_GPIO_Port, KBD_0_Pin)) scan |= 0x01;
		LL_GPIO_SetOutputPin(KBDA_8_GPIO_Port, KBDA_8_Pin);
	}
	if(!(addr & 0b000000010)) {
		LL_GPIO_ResetOutputPin(KBDA_9_GPIO_Port, KBDA_9_Pin);
		if(LL_GPIO_IsInputPinSet(KBD_4_GPIO_Port, KBD_4_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_3_GPIO_Port, KBD_3_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_2_GPIO_Port, KBD_2_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_1_GPIO_Port, KBD_1_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_0_GPIO_Port, KBD_0_Pin)) scan |= 0x01;
		LL_GPIO_SetOutputPin(KBDA_9_GPIO_Port, KBDA_9_Pin);
	}
	if(!(addr & 0b00000100)) {
		LL_GPIO_ResetOutputPin(KBDA_10_GPIO_Port, KBDA_10_Pin);
		if(LL_GPIO_IsInputPinSet(KBD_4_GPIO_Port, KBD_4_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_3_GPIO_Port, KBD_3_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_2_GPIO_Port, KBD_2_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_1_GPIO_Port, KBD_1_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_0_GPIO_Port, KBD_0_Pin)) scan |= 0x01;
		LL_GPIO_SetOutputPin(KBDA_10_GPIO_Port, KBDA_10_Pin);
	}
	if(!(addr & 0b00001000)) {
		LL_GPIO_ResetOutputPin(KBDA_11_GPIO_Port, KBDA_11_Pin);
		if(LL_GPIO_IsInputPinSet(KBD_4_GPIO_Port, KBD_4_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_3_GPIO_Port, KBD_3_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_2_GPIO_Port, KBD_2_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_1_GPIO_Port, KBD_1_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_0_GPIO_Port, KBD_0_Pin)) scan |= 0x01;
		LL_GPIO_SetOutputPin(KBDA_11_GPIO_Port, KBDA_11_Pin);
	}
	if(!(addr & 0b00010000)) {
		LL_GPIO_ResetOutputPin(KBDA_12_GPIO_Port, KBDA_12_Pin);
		if(LL_GPIO_IsInputPinSet(KBD_4_GPIO_Port, KBD_4_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_3_GPIO_Port, KBD_3_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_2_GPIO_Port, KBD_2_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_1_GPIO_Port, KBD_1_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_0_GPIO_Port, KBD_0_Pin)) scan |= 0x01;
		LL_GPIO_SetOutputPin(KBDA_12_GPIO_Port, KBDA_12_Pin);
	}
	if(!(addr & 0b00100000)) {
		LL_GPIO_ResetOutputPin(KBDA_13_GPIO_Port, KBDA_13_Pin);
		if(LL_GPIO_IsInputPinSet(KBD_4_GPIO_Port, KBD_4_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_3_GPIO_Port, KBD_3_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_2_GPIO_Port, KBD_2_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_1_GPIO_Port, KBD_1_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_0_GPIO_Port, KBD_0_Pin)) scan |= 0x01;
		LL_GPIO_SetOutputPin(KBDA_13_GPIO_Port, KBDA_13_Pin);
	}
	if(!(addr & 0b01000000)) {
		LL_GPIO_ResetOutputPin(KBDA_14_GPIO_Port, KBDA_14_Pin);
		if(LL_GPIO_IsInputPinSet(KBD_4_GPIO_Port, KBD_4_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_3_GPIO_Port, KBD_3_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_2_GPIO_Port, KBD_2_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_1_GPIO_Port, KBD_1_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_0_GPIO_Port, KBD_0_Pin)) scan |= 0x01;
		LL_GPIO_SetOutputPin(KBDA_14_GPIO_Port, KBDA_14_Pin);
	}
	if(!(addr & 0b10000000)) {
		LL_GPIO_ResetOutputPin(KBDA_15_GPIO_Port, KBDA_15_Pin);
		if(LL_GPIO_IsInputPinSet(KBD_4_GPIO_Port, KBD_4_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_3_GPIO_Port, KBD_3_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_2_GPIO_Port, KBD_2_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_1_GPIO_Port, KBD_1_Pin)) scan |= 0x01;
		scan = scan << 1;
		if(LL_GPIO_IsInputPinSet(KBD_0_GPIO_Port, KBD_0_Pin)) scan |= 0x01;
		LL_GPIO_SetOutputPin(KBDA_15_GPIO_Port, KBDA_15_Pin);
	}

	return scan & 0x1f;
}

char cpmkbd_read() {
	char res='\0';
	uint8_t scan[8];

	for(uint8_t i=0; i<8; i++)
		scan[i] = zxkbd_scan(~(0x01<<i));

	if(scan[6] == 0x1e) return '\n';
	if(scan[7] == 0x1e) return ' ';

	res = (scan[0] == 0x1d) ? 'z' : (scan[0] == 0x1b) ? 'x' : (scan[0] == 0x17) ? 'c' : (scan[0] == 0x0f) ? 'v' : res;
	res = (scan[1] == 0x1e) ? 'a' : (scan[1] == 0x1d) ? 's' : (scan[1] == 0x1b) ? 'd' : (scan[1] == 0x17) ? 'f' : (scan[1] == 0x0f) ? 'g' : res;
	res = (scan[2] == 0x1e) ? 'q' : (scan[2] == 0x1d) ? 'w' : (scan[2] == 0x1b) ? 'e' : (scan[2] == 0x17) ? 'r' : (scan[2] == 0x0f) ? 't' : res;
	res = (scan[5] == 0x1e) ? 'p' : (scan[5] == 0x1d) ? 'o' : (scan[5] == 0x1b) ? 'i' : (scan[5] == 0x17) ? 'u' : (scan[5] == 0x0f) ? 'y' : res;
	res = (scan[6] == 0x1d) ? 'l' : (scan[6] == 0x1b) ? 'k' : (scan[6] == 0x17) ? 'j' : (scan[6] == 0x0f) ? 'h' : res;
	res = (scan[7] == 0x1b) ? 'm' : (scan[7] == 0x17) ? 'n' : (scan[7] == 0x0f) ? 'b' : res;
	if(IS_SHIFT && (res >= 'a') && (res<='z')) res -= 0x20;

	res = (scan[3] == 0x1e) ? '1' : (scan[3] == 0x1d) ? '2' : (scan[3] == 0x1b) ? '3' : (scan[3] == 0x17) ? '4' : (scan[3] == 0x0f) ? '5' : res;
	res = (scan[4] == 0x1e) ? '0' : (scan[4] == 0x1d) ? '9' : (scan[4] == 0x1b) ? '8' : (scan[4] == 0x17) ? '7' : (scan[4] == 0x0f) ? '6' : res;

	return res;
}

#endif /* KBD_DRIVER_H_ */
