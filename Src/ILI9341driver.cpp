/*
 * ILI9341driver.cpp
 *
 *  Created on: Oct 3, 2019
 *      Author: artur
 */

#include "ILI9341driver.h"

ILI9341driver::ILI9341driver() {

}

ILI9341driver::~ILI9341driver() {

}

inline void ILI9341driver::SendCmd(uint8_t cmd) {
  LCD_DC_RESET;
  LCD_CS_RESET;
  LL_SPI_TransmitData8(ILI9341_SPI, cmd);
  LCD_CS_SET;
}

inline void ILI9341driver::Send(uint8_t data) {
  LCD_DC_SET;
  LCD_CS_RESET;
  LL_SPI_TransmitData8(ILI9341_SPI, data);
  LCD_CS_SET;
}

void ILI9341driver::Init() {
	if(_isinit) return;

	while(!isReady());

	//enable periphery used by driver
	LL_SPI_Enable(ILI9341_SPI);
	LL_TIM_CC_EnableChannel(ILI9341_LED_PWM_TIM, ILI9341_LED_PWM_CH);
	LL_TIM_EnableCounter(ILI9341_LED_PWM_TIM);

	//set brightness of backlight
	setLEDpwm(100);

	//init ILI9341
	// настраиваем ноги
	LCD_CS_SET;

	// сброс дисплея
	LL_mDelay(10);

	LL_mDelay(100);

	/// настраиваем дисплей
	SendCmd(ILI9341_POWERA);
	Send(0x39);
	Send(0x2C);
	Send(0x00);
	Send(0x34);
	Send(0x02);
	SendCmd(ILI9341_POWERB);
	Send(0x00);
	Send(0xC1);
	Send(0x30);
	SendCmd(ILI9341_DTCA);
	Send(0x85);
	Send(0x00);
	Send(0x78);
	SendCmd(ILI9341_DTCB);
	Send(0x00);
	Send(0x00);
	SendCmd(ILI9341_POWER_SEQ);
	Send(0x64);
	Send(0x03);
	Send(0x12);
	Send(0x81);
	SendCmd(ILI9341_PRC);
	Send(0x20);
	SendCmd(ILI9341_POWER1);
	Send(0x23);
	SendCmd(ILI9341_POWER2);
	Send(0x10);
	SendCmd(ILI9341_VCOM1);
	Send(0x3E);
	Send(0x28);
	SendCmd(ILI9341_VCOM2);
	Send(0x86);
	SendCmd(ILI9341_MAC);//set orientation
	Send(ORIENTATION_LANDSCAPE_MIRROR);
	SendCmd(ILI9341_PIXEL_FORMAT);
	Send(0x55);
	SendCmd(ILI9341_FRMCTR1);
	Send(0x00);
	Send(0x18);
	SendCmd(ILI9341_DFC);
	Send(0x08);
	Send(0x82);
	Send(0x27);
	SendCmd(ILI9341_3GAMMA_EN);
	Send(0x00);
	SendCmd(ILI9341_GAMMA);
	Send(0x01);
	SendCmd(ILI9341_PGAMMA);
	Send(0x0F);
	Send(0x31);
	Send(0x2B);
	Send(0x0C);
	Send(0x0E);
	Send(0x08);
	Send(0x4E);
	Send(0xF1);
	Send(0x37);
	Send(0x07);
	Send(0x10);
	Send(0x03);
	Send(0x0E);
	Send(0x09);
	Send(0x00);
	SendCmd(ILI9341_NGAMMA);
	Send(0x00);
	Send(0x0E);
	Send(0x14);
	Send(0x03);
	Send(0x11);
	Send(0x07);
	Send(0x31);
	Send(0xC1);
	Send(0x48);
	Send(0x08);
	Send(0x0F);
	Send(0x0C);
	Send(0x31);
	Send(0x36);
	Send(0x0F);
	SendCmd(ILI9341_SLEEP_OUT);

	LL_mDelay(100);
	SendCmd(ILI9341_DISPLAY_ON);
	SendCmd(ILI9341_GRAM);

	_isinit=true;
}

void ILI9341driver::setFrame(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	while(!isReady());

	SendCmd(ILI9341_COLUMN_ADDR);
	Send(x1 >> 8);
	Send(x1 & 0xFF);
	Send(x2 >> 8);
	Send(x2 & 0xFF);

	SendCmd(ILI9341_PAGE_ADDR);
	Send(y1 >> 8);
	Send(y1 & 0xFF);
	Send(y2 >> 8);
	Send(y2 & 0xFF);

	LL_GPIO_ResetOutputPin(LED_GPIO_Port, LED_Pin);
}

void ILI9341driver::SendBuf(uint16_t *buf, uint32_t len) {
	while(!isReady());
	LCD_CS_RESET;
	LL_DMA_SetDataLength(ILI9341_DMA, ILI9341_DMA_TX_CH, len);
	LL_DMA_SetMemoryAddress(ILI9341_DMA, ILI9341_DMA_TX_CH, (uint32_t)buf);

	SendCmd(ILI9341_GRAM);
	LL_DMA_EnableChannel(ILI9341_DMA, ILI9341_DMA_TX_CH);
}

void ILI9341driver::ReadBuf(uint16_t *buf, uint32_t len) {
	while(!isReady());
	LCD_CS_RESET;
	LL_DMA_SetDataLength(ILI9341_DMA, ILI9341_DMA_RX_CH, len);
	LL_DMA_SetMemoryAddress(ILI9341_DMA, ILI9341_DMA_TX_CH, (uint32_t)buf);

	SendCmd(ILI9341_RAMRD);
	LL_DMA_EnableChannel(ILI9341_DMA, ILI9341_DMA_RX_CH);
}

void ILI9341driver::fillArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
	while(!isReady());
	setFrame(x1, y1, x2, y2);

//	LL_DMA_SetMemorySize(ILI9341_DMA, ILI9341_DMA_TX_CH, LL_DMA_MDATAALIGN_HALFWORD);
//	LL_DMA_SetDataLength(ILI9341_DMA, ILI9341_DMA_TX_CH, (x2-x1+1)*(y2-y1+1));
//	LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_3, LL_DMA_MEMORY_NOINCREMENT);
//	LL_DMA_SetMemoryAddress(ILI9341_DMA, ILI9341_DMA_TX_CH, (uint32_t)&color);
//
//	SendCmd(LCD_GRAM);
//	LL_DMA_EnableChannel(ILI9341_DMA, ILI9341_DMA_TX_CH);

	for(uint32_t i;i<(x2-x1+1)*(y2-y1+1);i++) {
		SendCmd(ILI9341_GRAM);
		Send(RED>>8);
		Send(RED&0xFF);
	}

}
