/*
 * cpm_system.c
 *
 *  Created on: Oct 14, 2019
 *      Author: Artur
 */

#include "stdlib.h"
#include "fatfs.h"
#include "cpm_system.h"

static uint8_t dskdsk = 0x00;
static uint16_t dsktrk = 0x0000;
static uint16_t dsksec = 0x0000;
static uint16_t dskdma = 0x0000;
static uint8_t dskst = 0x00;

static void cpmdsk_rwsec(uint8_t op) {// op=0 for read, op=1 for write
	uint8_t buf[128];
	UINT num;
	char fname[22] = "0:/CPM/DISK_";
	fname[12] = '0'+dskdsk;
	fname[13] = '\0';
	//utoa(dskdsk, &fname[12], 16);

	register uint8_t st = 0x00;

	retUSER = f_open(&USERFile, fname, op+1);
	if(retUSER != FR_OK) {
		st = 0x01;
	}
	else {
		retUSER = f_lseek(&USERFile, ((uint32_t)dsktrk*64+dsksec-1)*128);
		if(retUSER != FR_OK) {
			st = 0x01;
		}
		else {
			if(op) {//write
				for(register uint8_t i=0; i<128; i++)
					buf[i] = mem_read(dskdma+i);
				retUSER = f_write(&USERFile, buf, 128, &num);
			}
			else {//read
				retUSER = f_read(&USERFile, buf, 128, &num);
				for(register uint8_t i=0; i<128; i++) {
					mem_write(dskdma+i, buf[i]);
				}
			}

			if(retUSER != FR_OK || num != 128) {
				st = 0x01;
			}
		}
	}

	f_close(&USERFile);
	dskst = st;
}

static void cpmsys_load() {
	cpmcons_puts("Loading CP/M\n");

	dskdsk = 0x00;
	dsktrk = 0x0000;
	dsksec = 0x0002;
	dskdma = 0xa400;
	do {
		cpmdsk_rwsec(0);
		dsksec++;
		dskdma += 128;
		if(dskst) {
			cpmcons_errmsg(retUSER, "load CP/M");
			return;
		}
	} while(dsksec < 53);

	cpmcons_puts("CP/M Loaded\n");
}

void cpmsys_Run() {
	mem_Init(MEMTYPE_CPM);
	cpmcons_clear();
	z80_Init(cpmports_out, cpmports_in);

	cpmsys_load();
	PC = 0xba00;

//	char cbuf[10];
	while(1) {
//		cpmcons_puts(utoa(PC, cbuf, 16));
//		cpmcons_putc(' ');
		z80_step();
	}
}

void cpmports_out(uint16_t addr, uint8_t data) {
	if((addr & 0x00ff) == 0x0001) {//ciop
		cpmcons_putc(data);
	}
	else if((addr & 0x00ff) == 0x0003) {//siop
		LL_USART_TransmitData8(USART1, data);
	}
	else if((addr & 0x00ff) == 0x0008) {//ddskp
		dskdsk = data;
	}
	else if((addr & 0x00ff) == 0x0009) {//dtrkp
		if(dsktrk)
			dsktrk |= ((uint16_t)data) << 8;
		else
			dsktrk = data;
	}
	else if((addr & 0x00ff) == 0x000a) {//dsecp
		if(dsksec)
			dsksec |= ((uint16_t)data) << 8;
		else
			dsksec = data;
	}
	else if((addr & 0x00ff) == 0x000b) {//ddmap
		if(dskdma)
			dskdma |= ((uint16_t)data) << 8;
		else
			dskdma = data;

	}
	else if((addr & 0x00ff) == 0x000c) {//dcmdp
		if(data < 0x02)//read/write
			cpmdsk_rwsec(data);

		dsktrk = 0x0000;
		dsksec = 0x0000;
		dskdma = 0x0000;
	}
}

uint8_t cpmports_in(uint16_t addr) {
	if((addr & 0x00ff) == 0x0000) {//cstp
		return cpmconsst;
	}
	else if((addr & 0x00ff) == 0x0001) {//ciop
		return cpmconsch;
	}
	else if((addr & 0x00ff) == 0x0002) {//sstp
		register uint8_t status = 0x00;
		if(LL_USART_IsActiveFlag_RXNE(USART1))//ready for Rx
			status |= 0x02;
		if(LL_USART_IsActiveFlag_TXE(USART1))//ready for Tx
			status |= 0x01;

		return status;
	}
	else if((addr & 0x00ff) == 0x0003) {//siop
		return LL_USART_ReceiveData8(USART1);
	}
	else if((addr & 0x00ff) == 0x000d) {//dcmdp
		return dskst;
	}
	else
		return 0x00;
}
