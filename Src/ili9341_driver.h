/*
 * ili9341_driver.h
 *
 *  Created on: Oct 4, 2019
 *      Author: Artur
 */

#ifndef ILI9341DRIVER_H_
#define ILI9341DRIVER_H_

#include "main.h"
#include "ili9341_commands.h"

#ifndef __SIMULATION
//display RESET pin connect to STM32 reset pin
#define ILI9341_DC_SET		LL_GPIO_SetOutputPin(ILI9341_DC_GPIO_Port, ILI9341_DC_Pin)
#define ILI9341_DC_RESET	LL_GPIO_ResetOutputPin(ILI9341_DC_GPIO_Port, ILI9341_DC_Pin)
#define ILI9341_CS_SET		LL_GPIO_SetOutputPin(ILI9341_CS_GPIO_Port, ILI9341_CS_Pin)
#define ILI9341_CS_RESET	LL_GPIO_ResetOutputPin(ILI9341_CS_GPIO_Port, ILI9341_CS_Pin)

#define ILI9341_SPI				SPI1
#define ILI9341_DMA				DMA1
#define ILI9341_DMA_RX_CH		LL_DMA_CHANNEL_2
#define ILI9341_DMA_TX_CH		LL_DMA_CHANNEL_3
#define ILI9341_LED_PWM_TIM		TIM2
#define ILI9341_LED_PWM_CH		LL_TIM_CHANNEL_CH2
#define ILI9341_SETLED_PWM(val)	LL_TIM_OC_SetCompareCH2(ILI9341_LED_PWM_TIM, val)
#define ILI9341_WAIT_DMA()		{ while(ILI9341_DMA_busy); }
#define TX_DMA_IRQ_HANDLER() 	{ LL_SPI_DisableDMAReq_TX(ILI9341_SPI); \
	    LL_DMA_DisableChannel(ILI9341_DMA, ILI9341_DMA_TX_CH); \
		while(LL_DMA_IsEnabledChannel(ILI9341_DMA, ILI9341_DMA_TX_CH)); \
	    if(LL_DMA_IsActiveFlag_TE3(ILI9341_DMA)) { \
	    	LL_DMA_ClearFlag_TE3(ILI9341_DMA);} \
		LL_DMA_ClearFlag_TC3(ILI9341_DMA); \
		while(LL_SPI_IsActiveFlag_BSY(ILI9341_SPI) != 0); \
		LL_SPI_SetDataWidth(ILI9341_SPI, LL_SPI_DATAWIDTH_8BIT); \
		ILI9341_DMA_busy = 0; }
#define RX_DMA_IRQ_HANDLER() 	{ LL_SPI_DisableDMAReq_RX(ILI9341_SPI); \
	    LL_DMA_DisableChannel(ILI9341_DMA, ILI9341_DMA_RX_CH); \
		while(LL_DMA_IsEnabledChannel(ILI9341_DMA, ILI9341_DMA_RX_CH)); \
	    if(LL_DMA_IsActiveFlag_TE2(ILI9341_DMA)) { \
	    	LL_DMA_ClearFlag_TE2(ILI9341_DMA);} \
		LL_DMA_ClearFlag_TC2(ILI9341_DMA); \
		while(LL_SPI_IsActiveFlag_BSY(ILI9341_SPI) != 0); \
		LL_SPI_SetDataWidth(ILI9341_SPI, LL_SPI_DATAWIDTH_8BIT); \
		ILI9341_DMA_busy = 0; }
#else
extern uint8_t ili9341_image[ILI9341_PWIDTH*ILI9341_PHEIGHT*3];
#endif

extern uint8_t ILI9341_DMA_busy;

void ILI9341_Init();
void ILI9341_sendCommand(uint8_t com);
void ILI9341_sendData(uint8_t data);
void ILI9341_setFrame(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void ILI9341_sendBuf(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t *buf, uint16_t len);
void ILI9341_readBuf(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t *buf, uint16_t len);
void ILI9341_fillArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

void ILI9341_setLEDpwm(uint16_t val);

#ifdef __SIMULATION
void ILI9341_readPix(uint16_t x, uint16_t y, uint8_t *r, uint8_t *g, uint8_t *b);
void ILI9341_writePix(uint16_t x, uint16_t y, uint16_t color);
#endif

#endif /* ILI9341DRIVER_H_ */
