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
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//'!'
	{0x00,0x04,0x04,0x00,0x00,0x00,0x00,0x00},//'"'
	{0x00,0x04,0x0e,0x04,0x04,0x0e,0x04,0x00},//'#'
	{0x00,0x08,0x0e,0x08,0x0e,0x0a,0x0e,0x08},//'$'
	{0x00,0x02,0x04,0x08,0x00,0x06,0x06,0x00},//'%'
	{0x00,0x00,0x08,0x00,0x0a,0x04,0x0a,0x00},//'&'
	{0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00},//'''
	{0x00,0x04,0x08,0x08,0x08,0x08,0x04,0x00},//'('
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//')'
	{0x00,0x00,0x04,0x08,0x0e,0x08,0x04,0x00},//'*'
	{0x00,0x00,0x08,0x08,0x0e,0x08,0x08,0x00},//'+'
	{0x00,0x00,0x00,0x00,0x00,0x08,0x08,0x00},//','
	{0x00,0x00,0x00,0x00,0x0e,0x00,0x00,0x00},//'-'
	{0x00,0x00,0x00,0x00,0x00,0x08,0x08,0x00},//'.'
	{0x00,0x00,0x02,0x04,0x08,0x00,0x00,0x00},//'/'
	{0x00,0x0c,0x06,0x0a,0x02,0x02,0x0c,0x00},//'0'
	{0x00,0x08,0x08,0x08,0x08,0x08,0x0e,0x00},//'1'
	{0x00,0x0c,0x02,0x02,0x0c,0x00,0x0e,0x00},//'2'
	{0x00,0x0c,0x02,0x0c,0x02,0x02,0x0c,0x00},//'3'
	{0x00,0x08,0x08,0x08,0x08,0x0e,0x08,0x00},//'4'
	{0x00,0x0e,0x00,0x0c,0x02,0x02,0x0c,0x00},//'5'
	{0x00,0x0c,0x00,0x0c,0x02,0x02,0x0c,0x00},//'6'
	{0x00,0x0e,0x02,0x04,0x08,0x00,0x00,0x00},//'7'
	{0x00,0x0c,0x02,0x0c,0x02,0x02,0x0c,0x00},//'8'
	{0x00,0x0c,0x02,0x02,0x0e,0x02,0x0c,0x00},//'9'
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//':'
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//';'
	{0x00,0x00,0x04,0x08,0x00,0x08,0x04,0x00},//'<'
	{0x00,0x00,0x00,0x0e,0x00,0x0e,0x00,0x00},//'='
	{0x00,0x00,0x00,0x08,0x04,0x08,0x00,0x00},//'>'
	{0x00,0x0c,0x02,0x04,0x08,0x00,0x08,0x00},//'?'
	{0x00,0x0c,0x0a,0x06,0x0e,0x00,0x0c,0x00},//'@'
	{0x00,0x0c,0x02,0x02,0x0e,0x02,0x02,0x00},//'A'
	{0x00,0x0c,0x02,0x0c,0x02,0x02,0x0c,0x00},//'B'
	{0x00,0x0c,0x02,0x00,0x00,0x02,0x0c,0x00},//'C'
	{0x00,0x08,0x04,0x02,0x02,0x04,0x08,0x00},//'D'
	{0x00,0x0e,0x00,0x0c,0x00,0x00,0x0e,0x00},//'E'
	{0x00,0x0e,0x00,0x0c,0x00,0x00,0x00,0x00},//'F'
	{0x00,0x0c,0x02,0x00,0x0e,0x02,0x0c,0x00},//'G'
	{0x00,0x02,0x02,0x0e,0x02,0x02,0x02,0x00},//'H'
	{0x00,0x0e,0x08,0x08,0x08,0x08,0x0e,0x00},//'I'
	{0x00,0x02,0x02,0x02,0x02,0x02,0x0c,0x00},//'J'
	{0x00,0x04,0x08,0x00,0x08,0x04,0x02,0x00},//'K'
	{0x00,0x00,0x00,0x00,0x00,0x00,0x0e,0x00},//'L'
	{0x00,0x02,0x06,0x0a,0x02,0x02,0x02,0x00},//'M'
	{0x00,0x02,0x02,0x02,0x0a,0x06,0x02,0x00},//'N'
	{0x00,0x0c,0x02,0x02,0x02,0x02,0x0c,0x00},//'O'
	{0x00,0x0c,0x02,0x02,0x0c,0x00,0x00,0x00},//'P'
	{0x00,0x0c,0x02,0x02,0x02,0x0a,0x0c,0x00},//'Q'
	{0x00,0x0c,0x02,0x02,0x0c,0x04,0x02,0x00},//'R'
	{0x00,0x0c,0x00,0x0c,0x02,0x02,0x0c,0x00},//'S'
	{0x00,0x0e,0x00,0x00,0x00,0x00,0x00,0x00},//'T'
	{0x00,0x02,0x02,0x02,0x02,0x02,0x0c,0x00},//'U'
	{0x00,0x02,0x02,0x02,0x02,0x04,0x08,0x00},//'V'
	{0x00,0x02,0x02,0x02,0x02,0x0a,0x04,0x00},//'W'
	{0x00,0x02,0x04,0x08,0x08,0x04,0x02,0x00},//'X'
	{0x00,0x02,0x04,0x08,0x00,0x00,0x00,0x00},//'Y'
	{0x00,0x0e,0x04,0x08,0x00,0x00,0x0e,0x00},//'Z'
	{0x00,0x0e,0x08,0x08,0x08,0x08,0x0e,0x00},//'['
	{0x00,0x00,0x00,0x00,0x00,0x08,0x04,0x00},//'\'
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},//']'
	{0x00,0x00,0x08,0x04,0x00,0x00,0x00,0x00},//'^'
	{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0f},//'_'
	{0x00,0x0c,0x02,0x08,0x00,0x00,0x0e,0x00},//'`'
	{0x00,0x00,0x08,0x04,0x0c,0x04,0x0c,0x00},//'a'
	{0x00,0x00,0x00,0x0c,0x02,0x02,0x0c,0x00},//'b'
	{0x00,0x00,0x0c,0x00,0x00,0x00,0x0c,0x00},//'c'
	{0x00,0x04,0x04,0x0c,0x04,0x04,0x0c,0x00},//'d'
	{0x00,0x00,0x08,0x04,0x08,0x00,0x0c,0x00},//'e'
	{0x00,0x0c,0x00,0x08,0x00,0x00,0x00,0x00},//'f'
	{0x00,0x00,0x0c,0x04,0x04,0x0c,0x04,0x08},//'g'
	{0x00,0x00,0x00,0x08,0x04,0x04,0x04,0x00},//'h'
	{0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00},//'i'
	{0x00,0x04,0x00,0x04,0x04,0x04,0x04,0x08},//'j'
	{0x00,0x00,0x08,0x00,0x00,0x08,0x04,0x00},//'k'
	{0x00,0x00,0x00,0x00,0x00,0x00,0x0c,0x00},//'l'
	{0x00,0x00,0x08,0x04,0x04,0x04,0x04,0x00},//'m'
	{0x00,0x00,0x08,0x04,0x04,0x04,0x04,0x00},//'n'
	{0x00,0x00,0x08,0x04,0x04,0x04,0x08,0x00},//'o'
	{0x00,0x00,0x08,0x04,0x04,0x08,0x00,0x00},//'p'
	{0x00,0x00,0x0c,0x04,0x04,0x0c,0x04,0x06},//'q'
	{0x00,0x00,0x0c,0x00,0x00,0x00,0x00,0x00},//'r'
	{0x00,0x00,0x08,0x00,0x08,0x04,0x08,0x00},//'s'
	{0x00,0x00,0x08,0x00,0x00,0x00,0x0c,0x00},//'t'
	{0x00,0x00,0x04,0x04,0x04,0x04,0x08,0x00},//'u'
	{0x00,0x00,0x04,0x04,0x08,0x08,0x00,0x00},//'v'
	{0x00,0x00,0x04,0x04,0x04,0x04,0x08,0x00},//'w'
	{0x00,0x00,0x04,0x08,0x00,0x08,0x04,0x00},//'x'
	{0x00,0x00,0x04,0x04,0x04,0x0c,0x04,0x08},//'y'
	{0x00,0x00,0x0c,0x08,0x00,0x00,0x0c,0x00},//'z'
	{0x00,0x0e,0x08,0x00,0x08,0x08,0x0e,0x00},//'{'
	{0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x00},//'|'
	{0x00,0x00,0x00,0x0c,0x00,0x00,0x00,0x00},//'}'
	{0x00,0x04,0x08,0x00,0x00,0x00,0x00,0x00},//'~'
	{0x0c,0x02,0x09,0x01,0x01,0x09,0x02,0x0c}//''
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

