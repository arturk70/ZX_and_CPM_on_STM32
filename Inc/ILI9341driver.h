/*
 * ILI9341driver.h
 *
 *  Created on: Oct 3, 2019
 *      Author: artur
 */
#ifndef ILI9341DRIVER_H_
#define ILI9341DRIVER_H_

#include "main.h"
#include "ILI9341_commands.h"

//display RESET pin connect to STM32 reset pin

#define LCD_DC_SET		LL_GPIO_SetOutputPin(ILI9341_DC_GPIO_Port, ILI9341_DC_Pin)
#define LCD_DC_RESET	LL_GPIO_ResetOutputPin(ILI9341_DC_GPIO_Port, ILI9341_DC_Pin)
#define LCD_CS_SET		LL_GPIO_SetOutputPin(ILI9341_CS_GPIO_Port, ILI9341_CS_Pin)
#define LCD_CS_RESET	LL_GPIO_ResetOutputPin(ILI9341_CS_GPIO_Port, ILI9341_CS_Pin)

#define ILI9341_SPI			SPI1
#define ILI9341_DMA			DMA1
#define ILI9341_DMA_RX_CH	LL_DMA_CHANNEL_2
#define ILI9341_DMA_TX_CH	LL_DMA_CHANNEL_3
#define ILI9341_LED_PWM_TIM	TIM2
#define ILI9341_LED_PWM_CH	LL_TIM_CHANNEL_CH3

class ILI9341driver {
public:
	ILI9341driver();
	virtual ~ILI9341driver();
	void Init();
	inline bool isInitiated() {return _isinit;}
	inline bool isReady() {return LL_GPIO_IsOutputPinSet(ILI9341_CS_GPIO_Port, ILI9341_CS_Pin);}
	inline void setLEDpwm(uint8_t val) {LL_TIM_OC_SetCompareCH3(ILI9341_LED_PWM_TIM, val);}
	inline void Send(uint8_t data) {LL_SPI_TransmitData8(ILI9341_SPI, data);}
	inline void SendCmd(uint8_t data) {LCD_DC_RESET; LL_SPI_TransmitData8(ILI9341_SPI, data); LCD_DC_SET;}
	void setFrame(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
	void SendBuf(uint8_t *buf, uint32_t len);
	void ReadBuf(uint8_t *buf, uint32_t len);
	void fillArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

private:
	bool _isinit = false;
};

#endif /* ILI9341DRIVER_H_ */
