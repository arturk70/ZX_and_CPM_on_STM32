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

void ILI9341driver::Init() {
	if(_isinit) return;

	//enable periphery used by driver
	LL_DMA_DisableChannel(ILI9341_DMA, ILI9341_DMA_RX_CH);
	LL_DMA_DisableChannel(ILI9341_DMA, ILI9341_DMA_TX_CH);
	LL_DMA_ClearFlag_TC2(ILI9341_DMA);
	LL_DMA_ClearFlag_TE2(ILI9341_DMA);
	LL_DMA_ClearFlag_TC3(ILI9341_DMA);
	LL_DMA_ClearFlag_TE3(ILI9341_DMA);
	LL_SPI_EnableDMAReq_TX(ILI9341_SPI);
	LL_SPI_EnableDMAReq_RX(ILI9341_SPI);
	LL_DMA_EnableIT_TC(ILI9341_DMA, ILI9341_DMA_RX_CH);
	LL_DMA_EnableIT_TE(ILI9341_DMA, ILI9341_DMA_RX_CH);
	LL_DMA_EnableIT_TC(ILI9341_DMA, ILI9341_DMA_TX_CH);
	LL_DMA_EnableIT_TE(ILI9341_DMA, ILI9341_DMA_TX_CH);
	LL_SPI_Enable(ILI9341_SPI);
	LL_DMA_DisableChannel(ILI9341_DMA, ILI9341_DMA_RX_CH);
	LL_DMA_DisableChannel(ILI9341_DMA, ILI9341_DMA_TX_CH);
	LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH3);

	//init ILI9341
	LL_mDelay(50);
	LCD_CS_RESET;
	SendCmd(LCD_SLEEP_OUT);
	LL_mDelay(150);
	SendCmd(LCD_DISPLAY_ON);

	// Power control A
	SendCmd(LCD_POWERA);
	Send(0x39);Send(0x2C);Send(0x00);Send(0x34);Send(0x02);
	// Power control B
	SendCmd(LCD_POWERB);
	Send(0x00);Send(0xC1);Send(0x30);
	// Driver timing control A
	SendCmd(LCD_DTCA);
	Send(0x85);Send(0x00);Send(0x78);
	// Driver timing control B
	SendCmd(LCD_DTCB);
	Send(0x00);Send(0x00);
	// Power on sequence control
	SendCmd(LCD_POWER_SEQ);
	Send(0x64);Send(0x03);Send(0x12);Send(0x81);
	// Pump ratio control
	SendCmd(LCD_PRC);
	Send(0x20);
	// Power control 1
	SendCmd(LCD_POWER1);
	Send(0x23);
	// Power control 2
	SendCmd(LCD_POWER2);
	Send(0x10);
	// VCOM control 1
	SendCmd(LCD_VCOM1);
	Send(0x3E);Send(0x28);
	// VCOM control 2
	SendCmd(LCD_VCOM2);
	Send(0x86);
	// Memory access control
	SendCmd(LCD_MAC);
	Send(0x48);
	// Pixel format set
	SendCmd(LCD_PIXEL_FORMAT);
	Send(0x55);
	// Frame rate control
	SendCmd(LCD_FRMCTR1);
	Send(0x00);Send(0x18);
	// Display function control
	SendCmd(LCD_DFC);
	Send(0x08);Send(0x82);Send(0x27);
	// 3Gamma function disable
	SendCmd(LCD_3GAMMA_EN);
	Send(0x00);
	// Gamma curve selected
	SendCmd(LCD_GAMMA);
	Send(0x01);
	// Set positive gamma
	SendCmd(LCD_PGAMMA);
	Send(0x0F);Send(0x31);Send(0x2B);Send(0x0C);Send(0x0E);Send(0x08);Send(0x4E);
	Send(0xF1);Send(0x37);Send(0x07);Send(0x10);Send(0x03);Send(0x0E);Send(0x09);Send(0x00);
	SendCmd(LCD_NGAMMA);
	Send(0x00);Send(0x0E);Send(0x14);Send(0x03);Send(0x11);Send(0x07);Send(0x31);
	Send(0xC1);Send(0x48);Send(0x08);Send(0x0F);Send(0x0C);Send(0x31);Send(0x36);Send(0x0F);

	//Set orientation
	SendCmd(LCD_MAC);
	Send(ORIENTATION_LANDSCAPE_MIRROR);

	LCD_CS_SET;

	//set brightness of backlight
	setLEDpwm(100);

	_isinit=true;
}

void ILI9341driver::setFrame(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	LCD_CS_RESET;

	SendCmd(LCD_COLUMN_ADDR);
	Send(x1 >> 8);
	Send(x2 & 0xFF);

	SendCmd(LCD_PAGE_ADDR);
	Send(y1 >> 8);
	Send(y2 & 0xFF);

	LCD_CS_SET;
}

void ILI9341driver::SendBuf(uint8_t *buf, uint32_t len) {
	LCD_CS_RESET;
	LL_DMA_SetDataLength(ILI9341_DMA, ILI9341_DMA_TX_CH, len);
	LL_DMA_ConfigAddresses(ILI9341_DMA, ILI9341_DMA_TX_CH,
			(uint32_t)buf, LL_SPI_DMA_GetRegAddr(ILI9341_SPI),
			LL_DMA_GetDataTransferDirection(ILI9341_DMA, ILI9341_DMA_TX_CH));

	SendCmd(LCD_GRAM);
	LL_DMA_EnableChannel(ILI9341_DMA, ILI9341_DMA_TX_CH);
}

void ILI9341driver::ReadBuf(uint8_t *buf, uint32_t len) {
	LCD_CS_RESET;
	LL_DMA_SetDataLength(ILI9341_DMA, ILI9341_DMA_RX_CH, len);
	LL_DMA_ConfigAddresses(ILI9341_DMA, ILI9341_DMA_RX_CH,
			LL_SPI_DMA_GetRegAddr(ILI9341_SPI), (uint32_t)buf,
			LL_DMA_GetDataTransferDirection(ILI9341_DMA, ILI9341_DMA_RX_CH));

	SendCmd(LCD_RAMRD);
	LL_DMA_EnableChannel(ILI9341_DMA, ILI9341_DMA_RX_CH);
}
