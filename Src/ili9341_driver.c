/*
 * ili9341_driver.c
 *
 *  Created on: Oct 4, 2019
 *      Author: Artur
 */

#include "ili9341_driver.h"

#ifndef __SIMULATION
uint8_t ILI9341_DMA_busy = 0;
#endif
static uint8_t is_inited = 0;
static uint16_t fcolor;

void ILI9341_setLEDpwm(uint16_t val) {
#ifndef __SIMULATION
	if(val>999) val=999;
	ILI9341_SETLED_PWM(val);
#endif
}

void ILI9341_sendCommand(uint8_t com) {
#ifndef __SIMULATION
	while(LL_SPI_IsActiveFlag_BSY(ILI9341_SPI) != 0);
	ILI9341_DC_RESET;
	LL_SPI_TransmitData8(ILI9341_SPI, com);
	while(LL_SPI_IsActiveFlag_BSY(ILI9341_SPI) != 0);
	ILI9341_DC_SET;
#endif
}

void ILI9341_sendData(uint8_t data) {
#ifndef __SIMULATION
	while(LL_SPI_IsActiveFlag_TXE(ILI9341_SPI) == 0);
	LL_SPI_TransmitData8(ILI9341_SPI, data);
#endif
}

void ILI9341_Init() {
	if(is_inited) return;

#ifndef __SIMULATION
	//turn on periphery used by driver
	LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH2);
	LL_TIM_EnableCounter(TIM2);
	LL_DMA_SetPeriphAddress(ILI9341_DMA, ILI9341_DMA_TX_CH, LL_SPI_DMA_GetRegAddr(ILI9341_SPI));
	LL_SPI_Enable(ILI9341_SPI);

	//set brightness of backlight
	ILI9341_setLEDpwm(200);

	//set CS pin
	ILI9341_CS_SET;
	ILI9341_DC_SET;

	// сброс дисплея
	LL_mDelay(100);
	ILI9341_WAIT_DMA();

	ILI9341_CS_RESET;
	/// настраиваем дисплей
	ILI9341_sendCommand(ILI9341_POWERA);
	ILI9341_sendData(0x39);
	ILI9341_sendData(0x2C);
	ILI9341_sendData(0x00);
	ILI9341_sendData(0x34);
	ILI9341_sendData(0x02);
	ILI9341_sendCommand(ILI9341_POWERB);
	ILI9341_sendData(0x00);
	ILI9341_sendData(0xC1);
	ILI9341_sendData(0x30);
	ILI9341_sendCommand(ILI9341_DTCA);
	ILI9341_sendData(0x85);
	ILI9341_sendData(0x00);
	ILI9341_sendData(0x78);
	ILI9341_sendCommand(ILI9341_DTCB);
	ILI9341_sendData(0x00);
	ILI9341_sendData(0x00);
	ILI9341_sendCommand(ILI9341_POWER_SEQ);
	ILI9341_sendData(0x64);
	ILI9341_sendData(0x03);
	ILI9341_sendData(0x12);
	ILI9341_sendData(0x81);
	ILI9341_sendCommand(ILI9341_PRC);
	ILI9341_sendData(0x20);
	ILI9341_sendCommand(ILI9341_POWER1);
	ILI9341_sendData(0x23);
	ILI9341_sendCommand(ILI9341_POWER2);
	ILI9341_sendData(0x10);
	ILI9341_sendCommand(ILI9341_VCOM1);
	ILI9341_sendData(0x3E);
	ILI9341_sendData(0x28);
	ILI9341_sendCommand(ILI9341_VCOM2);
	ILI9341_sendData(0x86);
	ILI9341_sendCommand(ILI9341_MAC);//set orientation
	ILI9341_sendData(ORIENTATION_LANDSCAPE_MIRROR);
	ILI9341_sendCommand(ILI9341_PIXEL_FORMAT);
	ILI9341_sendData(0x55);//RGB 565
	ILI9341_sendCommand(ILI9341_FRMCTR1);
	ILI9341_sendData(0x00);
	ILI9341_sendData(0x18);
	ILI9341_sendCommand(ILI9341_DFC);
	ILI9341_sendData(0x08);
	ILI9341_sendData(0x82);
	ILI9341_sendData(0x27);
	ILI9341_sendCommand(ILI9341_3GAMMA_EN);
	ILI9341_sendData(0x00);
	ILI9341_sendCommand(ILI9341_GAMMA);
	ILI9341_sendData(0x01);
	ILI9341_sendCommand(ILI9341_PGAMMA);
	ILI9341_sendData(0x0F);
	ILI9341_sendData(0x31);
	ILI9341_sendData(0x2B);
	ILI9341_sendData(0x0C);
	ILI9341_sendData(0x0E);
	ILI9341_sendData(0x08);
	ILI9341_sendData(0x4E);
	ILI9341_sendData(0xF1);
	ILI9341_sendData(0x37);
	ILI9341_sendData(0x07);
	ILI9341_sendData(0x10);
	ILI9341_sendData(0x03);
	ILI9341_sendData(0x0E);
	ILI9341_sendData(0x09);
	ILI9341_sendData(0x00);
	ILI9341_sendCommand(ILI9341_NGAMMA);
	ILI9341_sendData(0x00);
	ILI9341_sendData(0x0E);
	ILI9341_sendData(0x14);
	ILI9341_sendData(0x03);
	ILI9341_sendData(0x11);
	ILI9341_sendData(0x07);
	ILI9341_sendData(0x31);
	ILI9341_sendData(0xC1);
	ILI9341_sendData(0x48);
	ILI9341_sendData(0x08);
	ILI9341_sendData(0x0F);
	ILI9341_sendData(0x0C);
	ILI9341_sendData(0x31);
	ILI9341_sendData(0x36);
	ILI9341_sendData(0x0F);
	ILI9341_sendCommand(ILI9341_SLEEP_OUT);

	LL_mDelay(100);
	ILI9341_sendCommand(ILI9341_DISPLAY_ON);
#endif

	ILI9341_fillArea(0,0,ILI9341_PWIDTH-1,ILI9341_PHEIGHT/2-1,BLACK);
	ILI9341_fillArea(0,ILI9341_PHEIGHT/2,ILI9341_PWIDTH-1,ILI9341_PHEIGHT-1,BLACK);

	is_inited = 1;
}

void ILI9341_setFrame(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
#ifndef __SIMULATION
	ILI9341_WAIT_DMA();
	ILI9341_sendCommand(ILI9341_COLUMN_ADDR);
	ILI9341_sendData(x1 >> 8);
	ILI9341_sendData(x1 & 0xFF);
	ILI9341_sendData(x2 >> 8);
	ILI9341_sendData(x2 & 0xFF);

	ILI9341_sendCommand(ILI9341_PAGE_ADDR);
	ILI9341_sendData(y1 >> 8);
	ILI9341_sendData(y1 & 0xFF);
	ILI9341_sendData(y2 >> 8);
	ILI9341_sendData(y2 & 0xFF);
#else
#endif
}

//note: (x2-x1+1)*(y2-y1+1) must be less then 65536
void ILI9341_sendBuf(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t *data) {
#ifndef __SIMULATION
	ILI9341_WAIT_DMA();
#endif

	uint16_t len = (x2-x1+1)*(y2-y1+1);

	ILI9341_setFrame(x1, y1, x2, y2);
#ifndef __SIMULATION
	ILI9341_sendCommand(ILI9341_GRAM);

	LL_SPI_DisableDMAReq_TX(ILI9341_SPI);
	LL_DMA_DisableChannel(ILI9341_DMA, ILI9341_DMA_TX_CH);
	while(LL_DMA_IsEnabledChannel(ILI9341_DMA, ILI9341_DMA_TX_CH));
	LL_SPI_SetDataWidth(ILI9341_SPI, LL_SPI_DATAWIDTH_16BIT);
	LL_DMA_SetMemoryAddress(ILI9341_DMA, ILI9341_DMA_TX_CH, (uint32_t)data);
	LL_DMA_SetDataLength(ILI9341_DMA, ILI9341_DMA_TX_CH, len);
	LL_DMA_SetMemoryIncMode(ILI9341_DMA, ILI9341_DMA_TX_CH, LL_DMA_MEMORY_INCREMENT);
	LL_DMA_DisableIT_HT(ILI9341_DMA, ILI9341_DMA_TX_CH);
	LL_DMA_EnableIT_TC(ILI9341_DMA, ILI9341_DMA_TX_CH);
	LL_DMA_EnableIT_TE(ILI9341_DMA, ILI9341_DMA_TX_CH);
	LL_DMA_EnableChannel(ILI9341_DMA, ILI9341_DMA_TX_CH);
	LL_SPI_EnableDMAReq_TX(ILI9341_SPI);

	ILI9341_DMA_busy = 1;
#else
#endif
}

//note: (x2-x1+1)*(y2-y1+1) must be less then 65536
void ILI9341_readBuf(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t *buf) {
#ifndef __SIMULATION
	ILI9341_WAIT_DMA();
#endif

	uint16_t len = (x2-x1+1)*(y2-y1+1);

	ILI9341_setFrame(x1, y1, x2, y2);

#ifndef __SIMULATION
	ILI9341_sendCommand(ILI9341_RAMRD);

	LL_SPI_SetBaudRatePrescaler(ILI9341_SPI, LL_SPI_BAUDRATEPRESCALER_DIV8);

	uint8_t r,g,b;
	while(LL_SPI_IsActiveFlag_TXE(ILI9341_SPI) == 0);
	LL_SPI_TransmitData8(ILI9341_SPI, 0xaa);
	while(LL_SPI_IsActiveFlag_RXNE(ILI9341_SPI) == 0);
	r=LL_SPI_ReceiveData8(ILI9341_SPI);//read dummy byte


	for(uint16_t i=0; i<len;i++) {
		while(LL_SPI_IsActiveFlag_TXE(ILI9341_SPI) == 0);
		LL_SPI_TransmitData8(ILI9341_SPI, 0xaa);
		while(LL_SPI_IsActiveFlag_RXNE(ILI9341_SPI) == 0);
		r=LL_SPI_ReceiveData8(ILI9341_SPI);
		while(LL_SPI_IsActiveFlag_TXE(ILI9341_SPI) == 0);
		LL_SPI_TransmitData8(ILI9341_SPI, 0xaa);
		while(LL_SPI_IsActiveFlag_RXNE(ILI9341_SPI) == 0);
		g=LL_SPI_ReceiveData8(ILI9341_SPI);
		while(LL_SPI_IsActiveFlag_TXE(ILI9341_SPI) == 0);
		LL_SPI_TransmitData8(ILI9341_SPI, 0xaa);
		while(LL_SPI_IsActiveFlag_RXNE(ILI9341_SPI) == 0);
		b=LL_SPI_ReceiveData8(ILI9341_SPI);
		buf[i]=(((r & 0xF8) << 8u) | ((g & 0xFC) << 3u) | (b >> 3u));//RGB565 to uint16
	}
	while(LL_SPI_IsActiveFlag_BSY(ILI9341_SPI) != 0);
	ILI9341_CS_SET;
	ILI9341_CS_RESET;

	LL_SPI_SetBaudRatePrescaler(ILI9341_SPI, LL_SPI_BAUDRATEPRESCALER_DIV2);

	//	LL_DMA_DisableChannel(ILI9341_DMA, ILI9341_DMA_RX_CH);
	//	while(LL_DMA_IsEnabledChannel(ILI9341_DMA, ILI9341_DMA_RX_CH));
	//	LL_SPI_SetDataWidth(ILI9341_SPI, LL_SPI_DATAWIDTH_16BIT);
	//	LL_DMA_SetMemoryAddress(ILI9341_DMA, ILI9341_DMA_RX_CH, (uint32_t)buf);
	//	LL_DMA_SetDataLength(ILI9341_DMA, ILI9341_DMA_RX_CH, len+1);
	//	LL_DMA_SetMemoryIncMode(ILI9341_DMA, ILI9341_DMA_RX_CH, LL_DMA_MEMORY_INCREMENT);
	//	LL_DMA_DisableIT_HT(ILI9341_DMA, ILI9341_DMA_RX_CH);
	//	LL_DMA_EnableIT_TC(ILI9341_DMA, ILI9341_DMA_RX_CH);
	//	LL_DMA_EnableIT_TE(ILI9341_DMA, ILI9341_DMA_RX_CH);
	//	LL_SPI_EnableDMAReq_RX(ILI9341_SPI);
	//	LL_DMA_EnableChannel(ILI9341_DMA, ILI9341_DMA_RX_CH);

	//	fcolor=0x0000;
	//	LL_DMA_DisableChannel(ILI9341_DMA, ILI9341_DMA_TX_CH);
	//	while(LL_DMA_IsEnabledChannel(ILI9341_DMA, ILI9341_DMA_TX_CH));
	//	LL_DMA_SetMemoryAddress(ILI9341_DMA, ILI9341_DMA_TX_CH, (uint32_t)&fcolor);
	//	LL_DMA_SetDataLength(ILI9341_DMA, ILI9341_DMA_TX_CH, len+1);
	//	LL_DMA_SetMemoryIncMode(ILI9341_DMA, ILI9341_DMA_TX_CH, LL_DMA_MEMORY_NOINCREMENT);
	//	LL_DMA_DisableIT_HT(ILI9341_DMA, ILI9341_DMA_TX_CH);
	//	LL_DMA_EnableIT_TC(ILI9341_DMA, ILI9341_DMA_TX_CH);
	//	LL_DMA_EnableIT_TE(ILI9341_DMA, ILI9341_DMA_TX_CH);
	//	LL_SPI_EnableDMAReq_TX(ILI9341_SPI);
	//	LL_DMA_EnableChannel(ILI9341_DMA, ILI9341_DMA_TX_CH);

	//	ILI9341_DMA_busy = 1;
#else
#endif
}

//note: (x2-x1+1)*(y2-y1+1) must be less then 65536
void ILI9341_fillArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
#ifndef __SIMULATION
	ILI9341_WAIT_DMA();
#endif
	uint16_t len = (x2-x1+1)*(y2-y1+1);
	fcolor = color;

	ILI9341_setFrame(x1, y1, x2, y2);

#ifndef __SIMULATION
	ILI9341_sendCommand(ILI9341_GRAM);

	LL_SPI_DisableDMAReq_TX(ILI9341_SPI);
	LL_DMA_DisableChannel(ILI9341_DMA, ILI9341_DMA_TX_CH);
	while(LL_DMA_IsEnabledChannel(ILI9341_DMA, ILI9341_DMA_TX_CH));
	LL_SPI_SetDataWidth(ILI9341_SPI, LL_SPI_DATAWIDTH_16BIT);
	LL_DMA_SetMemoryAddress(ILI9341_DMA, ILI9341_DMA_TX_CH, (uint32_t)&fcolor);
	LL_DMA_SetDataLength(ILI9341_DMA, ILI9341_DMA_TX_CH, len);
	LL_DMA_SetMemoryIncMode(ILI9341_DMA, ILI9341_DMA_TX_CH, LL_DMA_MEMORY_NOINCREMENT);
	LL_DMA_DisableIT_HT(ILI9341_DMA, ILI9341_DMA_TX_CH);
	LL_DMA_EnableIT_TC(ILI9341_DMA, ILI9341_DMA_TX_CH);
	LL_DMA_EnableIT_TE(ILI9341_DMA, ILI9341_DMA_TX_CH);
	LL_DMA_EnableChannel(ILI9341_DMA, ILI9341_DMA_TX_CH);
	LL_SPI_EnableDMAReq_TX(ILI9341_SPI);

	ILI9341_DMA_busy = 1;
#else
#endif
}

#ifdef __SIMULATION
void ILI9341_readPix(uint16_t x, uint16_t y, uint8_t *r, uint8_t *g, uint8_t *b) {

}

void ILI9341_writePix(uint16_t x, uint16_t y, uint16_t color) {

}
#endif
