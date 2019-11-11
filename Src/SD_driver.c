/*
 * SD_driver.c
 *
 *  Created on: Nov 8, 2019
 *      Author: Artur
 */
#include "SD_driver.h"

SD_info sd_info;

#ifdef __SIMULATION
#include <stdio.h>
FILE* imgf;
#endif

#ifndef __SIMULATION
static uint8_t SD_rw(uint8_t data) {
	while(LL_SPI_IsActiveFlag_TXE(SD_SPI) == 0);
	LL_SPI_TransmitData8(SD_SPI, data);
	while(LL_SPI_IsActiveFlag_RXNE(SD_SPI) == 0);
	return LL_SPI_ReceiveData8(SD_SPI);
}

static uint8_t SD_cmd(uint8_t cmd, uint32_t arg) {
	register uint8_t n, res;

	if (cmd & 0x80) {
		cmd &= 0x7F;
		res = SD_cmd(CMD55, 0);
		if (res > 0x01) return res;
	}

	//for old SDv1 cards wait
	while(SD_rw(0xff) != 0xff);

	SD_rw(0x40 | cmd); // Start + Command index
	SD_rw((uint8_t)(arg >> 24)); // Argument[31..24]
	SD_rw((uint8_t)(arg >> 16)); // Argument[23..16]
	SD_rw((uint8_t)(arg >> 8)); // Argument[15..8]
	SD_rw((uint8_t)arg); // Argument[7..0]

	n = 0x01; // Dummy CRC + Stop
	if (cmd == CMD0) {n = 0x95;} // Valid CRC for CMD0(0)
	if (cmd == CMD8) {n = 0x87;} // Valid CRC for CMD8(0x1AA)
	SD_rw(n);

	// Receive a command response
	if(cmd == CMD12) SD_rw(0xff);
	n = 10; // Wait for a valid response in timeout of 10 attempts
	do {
		res = SD_rw(0xff);
	} while ((res & 0x80) && --n);

	return res;
}
#endif

#ifdef _SD_FULL_DRIVER
static uint8_t SD_readCID() {
	register uint8_t error = 0;

	if(SD_cmd(CMD10, 0))//read cid
		error = 0xf2;
	else {
		while((error = SD_rw(0xff)) == 0xff);//read token
		if(error == 0xfe) {//data token
			error = 0;

			sd_info.mid = SD_rw(0xff);
			sd_info.oid[0] = SD_rw(0xff);
			sd_info.oid[1] = SD_rw(0xff);
			sd_info.oid[2] = '\0';
			sd_info.pnm[0] = SD_rw(0xff);
			sd_info.pnm[1] = SD_rw(0xff);
			sd_info.pnm[2] = SD_rw(0xff);
			sd_info.pnm[3] = SD_rw(0xff);
			sd_info.pnm[4] = SD_rw(0xff);
			sd_info.pnm[5] = '\0';
			sd_info.prv = SD_rw(0xff);
			sd_info.psn = SD_rw(0xff);
			sd_info.psn = (sd_info.psn << 8) | SD_rw(0xff);
			sd_info.psn = (sd_info.psn << 8) | SD_rw(0xff);
			sd_info.psn = (sd_info.psn << 8) | SD_rw(0xff);
			sd_info.myr = SD_rw(0xff);
			sd_info.mmn = SD_rw(0xff);
			sd_info.myr = ((sd_info.myr & 0x0f) << 4) + ((sd_info.mmn >> 4) & 0x0f) + 2000;
			sd_info.mmn = sd_info.mmn & 0x0f;
			SD_rw(0xff);//read CID CRC

			SD_rw(0xff);//read 16-bit CRC
			SD_rw(0xff);
		}
		else
			error |=0xc0;
	}

	return error;
}
#endif

#ifndef __SIMULATION
static uint8_t SD_readCSD() {
	register uint8_t type, error = 0;

	if(SD_cmd(CMD9, 0))//read csd
		error = 0xf1;
	else {
		while((error = SD_rw(0xff)) == 0xff);//read token
		if(error == 0xfe) {//data token
			error = 0;

			type = SD_rw(0xff);//byte 15
			SD_rw(0xff);//byte 14
			SD_rw(0xff);//byte 13
			SD_rw(0xff);//byte 12
			SD_rw(0xff);//byte 11
			uint8_t csd_10_5[6];
			csd_10_5[0] = SD_rw(0xff);//byte 10
			csd_10_5[1] = SD_rw(0xff);//byte 9
			csd_10_5[2] = SD_rw(0xff);//byte 8
			csd_10_5[3] = SD_rw(0xff);//byte 7
			csd_10_5[4] = SD_rw(0xff);//byte 6
			csd_10_5[5] = SD_rw(0xff);//byte 5
			SD_rw(0xff);//byte 4
			SD_rw(0xff);//byte 3
			SD_rw(0xff);//byte 2
			SD_rw(0xff);//byte 1
			SD_rw(0xff);//byte 0

			SD_rw(0xff);//read 16-bit CRC
			SD_rw(0xff);

			if(type == 0x40) {//SDv2
				sd_info.size = ((csd_10_5[4] | (csd_10_5[3] << 8) | ((csd_10_5[2] & 0x0f) << 16)) + 1) * 512;
			}
			else {
				uint8_t c_size_mult;
				sd_info.size = (((csd_10_5[3] >> 6) & 0x03) | (csd_10_5[2] << 2) | ((csd_10_5[1] & 0x03) << 10)) + 1;
				c_size_mult = (((csd_10_5[5] >> 7) & 0x01) | ((csd_10_5[4] & 0x03) << 1)) + 2;
				sd_info.size *= (1U << c_size_mult) * (1U << (csd_10_5[0] & 0x0f));
				sd_info.size = sd_info.size / 1024;
			}
		}
		else
			error |=0x80;
	}

	return error;
}
#endif

uint8_t SD_init() {
#ifndef __SIMULATION
	register uint16_t tmr;
	register uint8_t i, error = 0;
	sd_info.type = SD_UNKNOWN;
#ifdef _SD_FULL_DRIVER
	for (i = 0; i < 4; i++)
		sd_info.ocr[i] = 0x00;
#endif

	//LL_DMA_SetPeriphAddress(SD_DMA, SD_DMA_TX_CH, LL_SPI_DMA_GetRegAddr(SD_SPI));
	LL_SPI_Enable(SD_SPI);

	//set SD SPI mode
	SD_CS_SET;
	LL_SPI_SetBaudRatePrescaler(SD_SPI, LL_SPI_BAUDRATEPRESCALER_DIV256);
	LL_mDelay(10);
	for(i=0; i<10; i++)
		SD_rw(0xff);
	SD_CS_RESET;
	for(i=0; i<10; i++)
		SD_rw(0xff);

	if (SD_cmd(CMD0, 0) == 0x01) {// Enter Idle state
		if (SD_cmd(CMD8, 0x1AA) == 0x01) {// SDv2
			sd_info.type = SD_SDSC;

			for (i = 0; i < 4; i++)
				sd_info.ocr[i] = SD_rw(0xff);

			if (sd_info.ocr[2] == 0x01 && sd_info.ocr[3] == 0xAA) {// The card can work at vdd range of 2.7-3.6V
				for (tmr = 5000; tmr && SD_cmd(ACMD41, 1UL << 30); tmr--); // Wait for leaving idle state (ACMD41 with HCS bit)
				if(!tmr) {
					sd_info.type = SD_UNKNOWN;
					error = 2;
				}
			}
			else {
				sd_info.type = SD_UNKNOWN;
				error = 3;
			}
		}
		else {//SDv1 or MMCv3
			register uint8_t cmd;
			if (SD_cmd(ACMD41, 0) <= 0x01) {
				sd_info.type = SD_SDv1;
				cmd = ACMD41;
			}
			else {
				sd_info.type = SD_MMC;
				cmd = CMD1;
			}
			for (tmr = 5000; tmr && SD_cmd(cmd, 0); tmr--) ; // Wait for leaving idle state
			if(!tmr) {
				sd_info.type = SD_UNKNOWN;
				error = 4;
			}
		}
	}
	else
		error = 1;

	if((sd_info.type != SD_UNKNOWN) && SD_cmd(CMD58, 0) == 0) { //read OCR
		for (i = 0; i < 4; i++)
			sd_info.ocr[i] = SD_rw(0xff);

		if((sd_info.type == SD_SDSC) && (sd_info.ocr[0] & 0x40)) // Check CCS bit in the OCR
			sd_info.type = SD_SDHC;
	}

	if(SD_cmd(CMD16, 512) != 0) {// Set R/W block length to 512 for not SDHC card
		sd_info.type = SD_UNKNOWN;
		error = 5;
	}

	if(sd_info.type != SD_UNKNOWN) {
		LL_SPI_SetBaudRatePrescaler(SD_SPI, LL_SPI_BAUDRATEPRESCALER_DIV4);

		error = SD_readCSD();
#ifdef _SD_FULL_DRIVER
		if(!error)//read cid
			error = SD_readCID();
#endif
	}

	SD_CS_SET;

	return error;
#else
	sd_info.type = SD_SDv1;
	sd_info.size = 247040;
	imgf = fopen("/home/artur/tmp/SD256.img", "r+b");
	if(imgf == NULL) {
		printf("Error while open image file\n");
		return 1;
	}
	return 0;
#endif
}

uint8_t SD_readblock(uint32_t bnum, uint8_t *buf) {
#ifndef __SIMULATION
	register uint16_t i;
	register uint8_t error = 0;

	if(sd_info.type != SD_SDHC) bnum <<= 9;

	SD_CS_RESET;

	if(SD_cmd(CMD17, bnum))
		error = 0xff;
	else {
		while((error = SD_rw(0xff)) == 0xff);//read token
		if(error == 0xfe) {//data token
			error = 0;
			for(i=0; i<512; i++)
				buf[i] = SD_rw(0xff);

			SD_rw(0xff);//read 16-bit CRC
			SD_rw(0xff);
		}
	}

	SD_CS_SET;

	return error;
#else
	fseek(imgf, bnum*512, 0);
	fread(buf, sizeof(uint8_t), 512, imgf);
	return 0;
#endif
}

uint8_t SD_writeblock(uint32_t bnum, const uint8_t *buf) {
#ifndef __SIMULATION
	register uint16_t i;
	register uint8_t error = 0;

	if(sd_info.type != SD_SDHC) bnum <<= 9;

	SD_CS_RESET;

	if(SD_cmd(CMD24, bnum))
		error = 0xff;
	else {
		SD_rw(0xff); //pause before send datablock
		SD_rw(0xfe); //data tocken
		for(i=0; i<512; i++)
			SD_rw(buf[i]);

		SD_rw(0xff);//send fake 16-bit CRC
		SD_rw(0xff);

		error = SD_rw(0xff) & 0x1f;//read data response
		if(error == 0x05)
			error = 0;
	}

	SD_CS_SET;

	return error;
#else
	fseek(imgf, bnum*512, 0);
	fwrite(buf, sizeof(uint8_t), 512, imgf);
	fflush(imgf);
	return 0;
#endif
}
