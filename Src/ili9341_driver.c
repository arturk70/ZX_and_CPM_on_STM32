/*
 * ili9341_driver.c
 *
 *  Created on: Oct 4, 2019
 *      Author: Artur
 */

#include "ili9341_driver.h"

uint8_t ILI9341_DMA_busy = 0;
static uint16_t fcolor;

inline static void ILI9341_sendCommand(uint8_t com) {
//  ILI9341_CS_RESET;
	ILI9341_DC_RESET;
	LL_SPI_TransmitData8(ILI9341_SPI, com);
	ILI9341_DC_SET;
//  ILI9341_CS_SET;
}

inline static void ILI9341_sendData(uint8_t data) {
//  ILI9341_DC_SET;
//  ILI9341_CS_RESET;
  LL_SPI_TransmitData8(ILI9341_SPI, data);
//  ILI9341_CS_SET;
}

void ILI9341_Init() {
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
	while(ILI9341_DMA_busy);

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
//	ILI9341_sendCommand(ILI9341_GRAM);

	ILI9341_fillArea(0,0,ILI9341_PWIDTH-1,ILI9341_PHEIGHT/2-1,BLACK);
	ILI9341_fillArea(0,ILI9341_PHEIGHT/2,ILI9341_PWIDTH-1,ILI9341_PHEIGHT-1,BLACK);
}

void ILI9341_setFrame(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	while(ILI9341_DMA_busy);
	ILI9341_DC_RESET;
	ILI9341_sendCommand(ILI9341_COLUMN_ADDR);
	ILI9341_DC_SET;
//	ILI9341_CS_RESET;
	LL_SPI_TransmitData8(ILI9341_SPI, x1 >> 8);
	LL_SPI_TransmitData8(ILI9341_SPI, x1 & 0xFF);
	LL_SPI_TransmitData8(ILI9341_SPI, x2 >> 8);
	LL_SPI_TransmitData8(ILI9341_SPI, x2 & 0xFF);
//	ILI9341_CS_SET;

	ILI9341_DC_RESET;
	ILI9341_sendCommand(ILI9341_PAGE_ADDR);
	ILI9341_DC_SET;
//	ILI9341_CS_RESET;
	LL_SPI_TransmitData8(ILI9341_SPI, y1 >> 8);
	LL_SPI_TransmitData8(ILI9341_SPI, y1 & 0xFF);
	LL_SPI_TransmitData8(ILI9341_SPI, y2 >> 8);
	LL_SPI_TransmitData8(ILI9341_SPI, y2 & 0xFF);
//	ILI9341_CS_SET;
}

//note: (x2-x1+1)*(y2-y1+1) must be less then 65536
void ILI9341_sendBuf(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t *data) {
	while(ILI9341_DMA_busy);

	uint16_t len = (x2-x1+1)*(y2-y1+1);

	ILI9341_setFrame(x1, y1, x2, y2);
//	ILI9341_CS_RESET;
	ILI9341_DC_RESET;
	LL_SPI_TransmitData8(ILI9341_SPI, ILI9341_GRAM);
	ILI9341_DC_SET;

	LL_DMA_DisableChannel(ILI9341_DMA, ILI9341_DMA_TX_CH);
	while(LL_DMA_IsEnabledChannel(ILI9341_DMA, ILI9341_DMA_TX_CH));
	LL_SPI_SetDataWidth(ILI9341_SPI, LL_SPI_DATAWIDTH_16BIT);
	LL_DMA_SetMemoryAddress(ILI9341_DMA, ILI9341_DMA_TX_CH, (uint32_t)data);
	LL_DMA_SetDataLength(ILI9341_DMA, ILI9341_DMA_TX_CH, len);
	LL_DMA_SetMemoryIncMode(ILI9341_DMA, ILI9341_DMA_TX_CH, LL_DMA_MEMORY_INCREMENT);
	LL_DMA_DisableIT_HT(ILI9341_DMA, ILI9341_DMA_TX_CH);
	LL_DMA_EnableIT_TC(ILI9341_DMA, ILI9341_DMA_TX_CH);
	LL_DMA_EnableIT_TE(ILI9341_DMA, ILI9341_DMA_TX_CH);
	LL_SPI_EnableDMAReq_TX(ILI9341_SPI);
	LL_DMA_EnableChannel(ILI9341_DMA, ILI9341_DMA_TX_CH);

	ILI9341_DMA_busy = 1;
}

//note: (x2-x1+1)*(y2-y1+1) must be less then 65536
void ILI9341_readBuf(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t *buf) {
	while(ILI9341_DMA_busy);

	uint16_t len = (x2-x1+1)*(y2-y1+1);

	ILI9341_setFrame(x1, y1, x2, y2);
//	ILI9341_CS_RESET;
	ILI9341_DC_RESET;
	LL_SPI_TransmitData8(ILI9341_SPI, ILI9341_RAMRD);
	ILI9341_DC_SET;

	uint8_t r,g,b;
	LL_SPI_TransmitData8(ILI9341_SPI, 0xaa);
	r=LL_SPI_ReceiveData8(ILI9341_SPI);//read dummy byte


	for(uint16_t i=0; i<len;i++) {
		LL_SPI_TransmitData8(ILI9341_SPI, 0xaa);
		r=LL_SPI_ReceiveData8(ILI9341_SPI);
		LL_SPI_TransmitData8(ILI9341_SPI, 0xaa);
		g=LL_SPI_ReceiveData8(ILI9341_SPI);
		LL_SPI_TransmitData8(ILI9341_SPI, 0xaa);
		b=LL_SPI_ReceiveData8(ILI9341_SPI);
		buf[i]=(((r & 0xF8) << 8u) | ((g & 0xFC) << 3u) | (b >> 3u))&0xffff;//RGB565 to uint16
		LL_GPIO_ResetOutputPin(LED_GPIO_Port, LED_Pin);
		LL_mDelay(100);
		LL_GPIO_SetOutputPin(LED_GPIO_Port, LED_Pin);
		LL_mDelay(100);
	}
//	while(LL_SPI_IsActiveFlag_BSY(ILI9341_SPI));
	ILI9341_CS_SET;
	ILI9341_CS_RESET;

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
}

//note: (x2-x1+1)*(y2-y1+1) must be less then 65536
void ILI9341_fillArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
	while(ILI9341_DMA_busy);
	uint16_t len = (x2-x1+1)*(y2-y1+1);
	fcolor = color;

	ILI9341_setFrame(x1, y1, x2, y2);
//	ILI9341_CS_RESET;
	ILI9341_DC_RESET;
	LL_SPI_TransmitData8(ILI9341_SPI, ILI9341_GRAM);
	ILI9341_DC_SET;

	LL_DMA_DisableChannel(ILI9341_DMA, ILI9341_DMA_TX_CH);
	while(LL_DMA_IsEnabledChannel(ILI9341_DMA, ILI9341_DMA_TX_CH));
	LL_SPI_SetDataWidth(ILI9341_SPI, LL_SPI_DATAWIDTH_16BIT);
	LL_DMA_SetMemoryAddress(ILI9341_DMA, ILI9341_DMA_TX_CH, (uint32_t)&fcolor);
	LL_DMA_SetDataLength(ILI9341_DMA, ILI9341_DMA_TX_CH, len);
	LL_DMA_SetMemoryIncMode(ILI9341_DMA, ILI9341_DMA_TX_CH, LL_DMA_MEMORY_NOINCREMENT);
	LL_DMA_DisableIT_HT(ILI9341_DMA, ILI9341_DMA_TX_CH);
	LL_DMA_EnableIT_TC(ILI9341_DMA, ILI9341_DMA_TX_CH);
	LL_DMA_EnableIT_TE(ILI9341_DMA, ILI9341_DMA_TX_CH);
	LL_SPI_EnableDMAReq_TX(ILI9341_SPI);
	LL_DMA_EnableChannel(ILI9341_DMA, ILI9341_DMA_TX_CH);

	ILI9341_DMA_busy = 1;
}
