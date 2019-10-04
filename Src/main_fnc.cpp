/*
 * main_fnc.cpp
 *
 *  Created on: Oct 3, 2019
 *      Author: artur
 */
#include "main.h"
#include "CPMDisplay.h"

static const uint8_t font[96][8] = {
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//' '
		{0x00,0x10,0x10,0x10,0x10,0x00,0x10,0x00},//'!'
		{0x00,0x24,0x24,0x00,0x00,0x00,0x00,0x00},//'"'
		{0x00,0x24,0x7e,0x24,0x24,0x7e,0x24,0x00},//'#'
		{0x00,0x08,0x3e,0x28,0x3e,0x0a,0x3e,0x08},//'$'
		{0x00,0x62,0x64,0x08,0x10,0x26,0x46,0x00},//'%'
		{0x00,0x10,0x28,0x10,0x2a,0x44,0x3a,0x00},//'&'
		{0x00,0x08,0x10,0x00,0x00,0x00,0x00,0x00},//'''
		{0x00,0x04,0x08,0x08,0x08,0x08,0x04,0x00},//'('
		{0x00,0x20,0x10,0x10,0x10,0x10,0x20,0x00},//')'
		{0x00,0x00,0x14,0x08,0x3e,0x08,0x14,0x00},//'*'
		{0x00,0x00,0x08,0x08,0x3e,0x08,0x08,0x00},//'+'
		{0x00,0x00,0x00,0x00,0x00,0x08,0x08,0x10},//','
		{0x00,0x00,0x00,0x00,0x3e,0x00,0x00,0x00},//'-'
		{0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00},//'.'
		{0x00,0x00,0x02,0x04,0x08,0x10,0x20,0x00},//'/'
		{0x00,0x3c,0x46,0x4a,0x52,0x62,0x3c,0x00},//'0'
		{0x00,0x18,0x28,0x08,0x08,0x08,0x3e,0x00},//'1'
		{0x00,0x3c,0x42,0x02,0x3c,0x40,0x7e,0x00},//'2'
		{0x00,0x3c,0x42,0x0c,0x02,0x42,0x3c,0x00},//'3'
		{0x00,0x08,0x18,0x28,0x48,0x7e,0x08,0x00},//'4'
		{0x00,0x7e,0x40,0x7c,0x02,0x42,0x3c,0x00},//'5'
		{0x00,0x3c,0x40,0x7c,0x42,0x42,0x3c,0x00},//'6'
		{0x00,0x7e,0x02,0x04,0x08,0x10,0x10,0x00},//'7'
		{0x00,0x3c,0x42,0x3c,0x42,0x42,0x3c,0x00},//'8'
		{0x00,0x3c,0x42,0x42,0x3e,0x02,0x3c,0x00},//'9'
		{0x00,0x00,0x00,0x10,0x00,0x00,0x10,0x00},//':'
		{0x00,0x00,0x10,0x00,0x00,0x10,0x10,0x20},//';'
		{0x00,0x00,0x04,0x08,0x10,0x08,0x04,0x00},//'<'
		{0x00,0x00,0x00,0x3e,0x00,0x3e,0x00,0x00},//'='
		{0x00,0x00,0x10,0x08,0x04,0x08,0x10,0x00},//'>'
		{0x00,0x3c,0x42,0x04,0x08,0x00,0x08,0x00},//'?'
		{0x00,0x3c,0x4a,0x56,0x5e,0x40,0x3c,0x00},//'@'
		{0x00,0x3c,0x42,0x42,0x7e,0x42,0x42,0x00},//'A'
		{0x00,0x7c,0x42,0x7c,0x42,0x42,0x7c,0x00},//'B'
		{0x00,0x3c,0x42,0x40,0x40,0x42,0x3c,0x00},//'C'
		{0x00,0x78,0x44,0x42,0x42,0x44,0x78,0x00},//'D'
		{0x00,0x7e,0x40,0x7c,0x40,0x40,0x7e,0x00},//'E'
		{0x00,0x7e,0x40,0x7c,0x40,0x40,0x40,0x00},//'F'
		{0x00,0x3c,0x42,0x40,0x4e,0x42,0x3c,0x00},//'G'
		{0x00,0x42,0x42,0x7e,0x42,0x42,0x42,0x00},//'H'
		{0x00,0x3e,0x08,0x08,0x08,0x08,0x3e,0x00},//'I'
		{0x00,0x02,0x02,0x02,0x42,0x42,0x3c,0x00},//'J'
		{0x00,0x44,0x48,0x70,0x48,0x44,0x42,0x00},//'K'
		{0x00,0x40,0x40,0x40,0x40,0x40,0x7e,0x00},//'L'
		{0x00,0x42,0x66,0x5a,0x42,0x42,0x42,0x00},//'M'
		{0x00,0x42,0x62,0x52,0x4a,0x46,0x42,0x00},//'N'
		{0x00,0x3c,0x42,0x42,0x42,0x42,0x3c,0x00},//'O'
		{0x00,0x7c,0x42,0x42,0x7c,0x40,0x40,0x00},//'P'
		{0x00,0x3c,0x42,0x42,0x52,0x4a,0x3c,0x00},//'Q'
		{0x00,0x7c,0x42,0x42,0x7c,0x44,0x42,0x00},//'R'
		{0x00,0x3c,0x40,0x3c,0x02,0x42,0x3c,0x00},//'S'
		{0x00,0xfe,0x10,0x10,0x10,0x10,0x10,0x00},//'T'
		{0x00,0x42,0x42,0x42,0x42,0x42,0x3c,0x00},//'U'
		{0x00,0x42,0x42,0x42,0x42,0x24,0x18,0x00},//'V'
		{0x00,0x42,0x42,0x42,0x42,0x5a,0x24,0x00},//'W'
		{0x00,0x42,0x24,0x18,0x18,0x24,0x42,0x00},//'X'
		{0x00,0x82,0x44,0x28,0x10,0x10,0x10,0x00},//'Y'
		{0x00,0x7e,0x04,0x08,0x10,0x20,0x7e,0x00},//'Z'
		{0x00,0x0e,0x08,0x08,0x08,0x08,0x0e,0x00},//'['
		{0x00,0x00,0x40,0x20,0x10,0x08,0x04,0x00},//'\'
		{0x00,0x70,0x10,0x10,0x10,0x10,0x70,0x00},//']'
		{0x00,0x10,0x38,0x54,0x10,0x10,0x10,0x00},//'^'
		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xff},//'_'
		{0x00,0x1c,0x22,0x78,0x20,0x20,0x7e,0x00},//'`'
		{0x00,0x00,0x38,0x04,0x3c,0x44,0x3c,0x00},//'a'
		{0x00,0x20,0x20,0x3c,0x22,0x22,0x3c,0x00},//'b'
		{0x00,0x00,0x1c,0x20,0x20,0x20,0x1c,0x00},//'c'
		{0x00,0x04,0x04,0x3c,0x44,0x44,0x3c,0x00},//'d'
		{0x00,0x00,0x38,0x44,0x78,0x40,0x3c,0x00},//'e'
		{0x00,0x0c,0x10,0x18,0x10,0x10,0x10,0x00},//'f'
		{0x00,0x00,0x3c,0x44,0x44,0x3c,0x04,0x38},//'g'
		{0x00,0x40,0x40,0x78,0x44,0x44,0x44,0x00},//'h'
		{0x00,0x10,0x00,0x30,0x10,0x10,0x38,0x00},//'i'
		{0x00,0x04,0x00,0x04,0x04,0x04,0x24,0x18},//'j'
		{0x00,0x20,0x28,0x30,0x30,0x28,0x24,0x00},//'k'
		{0x00,0x10,0x10,0x10,0x10,0x10,0x0c,0x00},//'l'
		{0x00,0x00,0x68,0x54,0x54,0x54,0x54,0x00},//'m'
		{0x00,0x00,0x78,0x44,0x44,0x44,0x44,0x00},//'n'
		{0x00,0x00,0x38,0x44,0x44,0x44,0x38,0x00},//'o'
		{0x00,0x00,0x78,0x44,0x44,0x78,0x40,0x40},//'p'
		{0x00,0x00,0x3c,0x44,0x44,0x3c,0x04,0x06},//'q'
		{0x00,0x00,0x1c,0x20,0x20,0x20,0x20,0x00},//'r'
		{0x00,0x00,0x38,0x40,0x38,0x04,0x78,0x00},//'s'
		{0x00,0x10,0x38,0x10,0x10,0x10,0x0c,0x00},//'t'
		{0x00,0x00,0x44,0x44,0x44,0x44,0x38,0x00},//'u'
		{0x00,0x00,0x44,0x44,0x28,0x28,0x10,0x00},//'v'
		{0x00,0x00,0x44,0x54,0x54,0x54,0x28,0x00},//'w'
		{0x00,0x00,0x44,0x28,0x10,0x28,0x44,0x00},//'x'
		{0x00,0x00,0x44,0x44,0x44,0x3c,0x04,0x38},//'y'
		{0x00,0x00,0x7c,0x08,0x10,0x20,0x7c,0x00},//'z'
		{0x00,0x0e,0x08,0x30,0x08,0x08,0x0e,0x00},//'{'
		{0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x00},//'|'
		{0x00,0x70,0x10,0x0c,0x10,0x10,0x70,0x00},//'}'
		{0x00,0x14,0x28,0x00,0x00,0x00,0x00,0x00},//'~'
		{0x3c,0x42,0x99,0xa1,0xa1,0x99,0x42,0x3c}//''
	};

CPM_Display disp(&font[0][0]);

void main_init() {

	disp.Init();
	//LL_GPIO_ResetOutputPin(LED_GPIO_Port, LED_Pin);


}

void main_loop() {
//	const char* test = "Hello, World! ";
//	for(uint8_t i; test[i] != '\0';i++)
//		disp.putc(test[i]);
	LL_mDelay(20);
}

