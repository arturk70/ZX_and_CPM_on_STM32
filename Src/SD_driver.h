/*
 * SD_driver.h
 *
 *  Created on: Nov 8, 2019
 *      Author: Artur
 */

#ifndef SD_DRIVER_H_
#define SD_DRIVER_H_

#include "main.h"

#ifndef __SIMULATION
#define SD_CS_SET		LL_GPIO_SetOutputPin(SD_CS_GPIO_Port, SD_CS_Pin)
#define SD_CS_RESET		LL_GPIO_ResetOutputPin(SD_CS_GPIO_Port, SD_CS_Pin)

#define SD_SPI				SPI2
#define SD_DMA				DMA1
#define SD_DMA_RX_CH		LL_DMA_CHANNEL_4
#define SD_DMA_TX_CH		LL_DMA_CHANNEL_5
#endif

#define CMD0	(0)			/* GO_IDLE_STATE */
#define CMD1	(1)			/* SEND_OP_COND (MMC) */
#define	ACMD41	(0x80+41)	/* SEND_OP_COND (SDC) */
#define CMD8	(8)			/* SEND_IF_COND */
#define CMD9	(9)			/* SEND_CSD */
#define CMD10	(10)		/* SEND_CID */
#define CMD12	(12)		/* STOP_TRANSMISSION */
#define ACMD13	(0x80+13)	/* SD_STATUS (SDC) */
#define CMD16	(16)		/* SET_BLOCKLEN */
#define CMD17	(17)		/* READ_SINGLE_BLOCK */
#define CMD18	(18)		/* READ_MULTIPLE_BLOCK */
#define CMD23	(23)		/* SET_BLOCK_COUNT (MMC) */
#define	ACMD23	(0x80+23)	/* SET_WR_BLK_ERASE_COUNT (SDC) */
#define CMD24	(24)		/* WRITE_BLOCK */
#define CMD25	(25)		/* WRITE_MULTIPLE_BLOCK */
#define CMD32	(32)		/* ERASE_ER_BLK_START */
#define CMD33	(33)		/* ERASE_ER_BLK_END */
#define CMD38	(38)		/* ERASE */
#define CMD55	(55)		/* APP_CMD */
#define CMD58	(58)		/* READ_OCR */

typedef enum {
	SD_UNKNOWN,
	SD_SDv1,
	SD_MMC,
	SD_SDSC,
	SD_SDHC
} sd_type;

typedef struct {
	sd_type type;
	uint8_t ocr[4];
#ifdef _SD_FULL_DRIVER
	uint8_t mid;
	char oid[3];
	char pnm[6];
	uint8_t prv;
	uint32_t psn;
	uint16_t myr;
	uint8_t mmn;
#endif
	uint32_t size; //in kb
} SD_info;

extern SD_info sd_info;

uint8_t SD_init();
uint8_t SD_readblock(uint32_t bnum, uint8_t *buf);
uint8_t SD_writeblock(uint32_t bnum, const uint8_t *buf);

#endif /* SD_DRIVER_H_ */
