/*
 * cpm_system.c
 *
 *  Created on: Oct 14, 2019
 *      Author: Artur
 */

#include "stdlib.h"
#include "fatfs.h"
#include "cpm_system.h"

#define	MEMSIZE		(48*1024)
#define CCPADDR		(MEMSIZE-7*1024)
#define BIOSADDR	(CCPADDR+0x1600)
#define DSKSECSIZE	128
#define	DSKSECTRK	256

static uint8_t cpmsys_isrun = 0;

static uint8_t dskdsk = 0x00;
static uint8_t dsktrk = 0x00;
static uint8_t dsksec = 0x00;
static uint16_t dskdma = 0x0000;
static uint8_t dskst = 0x00;

static void cpmdsk_rwsec(register uint8_t op) {// op=0 for read, op=1 for write
	uint8_t buf[DSKSECSIZE];
	UINT num;
	char fname[14] = "0:/CPM/DISK_";
	fname[12] = '0'+dskdsk;
	fname[13] = '\0';

	register uint8_t st = 0x00;

	retUSER = f_open(&USERFile, fname, op+1);
	if(retUSER != FR_OK) {
		st = 0x01;
	}
	else {
		retUSER = f_lseek(&USERFile, ((uint32_t)dsktrk*DSKSECTRK+dsksec)*DSKSECSIZE);
		if(retUSER != FR_OK) {
			st = 0x01;
		}
		else {
			if(op) {//write
				for(register uint8_t i=0; i<DSKSECSIZE; i++)
					buf[i] = mem_read(dskdma+i);
				retUSER = f_write(&USERFile, buf, DSKSECSIZE, &num);
			}
			else {//read
				retUSER = f_read(&USERFile, buf, DSKSECSIZE, &num);
				for(register uint8_t i=0; i<DSKSECSIZE; i++) {
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
//	cpmcons_puts("Loading CP/M\n");

	dskdsk = 0x00;
	dsktrk = 0x00;
	for(register uint8_t i=0; i<51; i++) {
		dsksec = 0x01 + i;
		dskdma = CCPADDR + i*DSKSECSIZE;
		cpmdsk_rwsec(0);
		if(dskst) {
			cpmcons_errmsg(retUSER, "load CP/M");
			return;
		}
	}

//	cpmcons_puts("CP/M Loaded\n");
}

void cpmsys_Run() {
	mem_Init(MEMTYPE_CPM);
	cpmcons_clear();
	z80_Init(cpmports_out, cpmports_in);

	cpmsys_load();
	PC = BIOSADDR;

	cpmsys_isrun = 1;

	while(cpmsys_isrun) {
		z80_step();
	}

	cpmcons_clear();
}

void cpmports_out(register uint32_t addr, register uint32_t data) {
	if((addr & 0x00ff) == 0x0001) {//ciop
		cpmcons_putc(data);
	}
	else if((addr & 0x00ff) == 0x0003) {//siop
#ifndef __SIMULATION
		LL_USART_TransmitData8(USART1, data);
#endif
	}
	else if((addr & 0x00ff) == 0x0008) {//ddskp
		dskdsk = data;
	}
	else if((addr & 0x00ff) == 0x0009) {//dtrkp
		dsktrk = data;
	}
	else if((addr & 0x00ff) == 0x000a) {//dsecp
		dsksec = data;
	}
	else if((addr & 0x00ff) == 0x000b) {//ddmalp
		dskdma = data;

	}
	else if((addr & 0x00ff) == 0x000c) {//ddmahp
		dskdma |= ((uint16_t)data) << 8;

	}
	else if((addr & 0x00ff) == 0x000d) {//dcmdp
		if(data < 0x02)//read/write
			cpmdsk_rwsec(data);

		dsktrk = 0x00;
		dsksec = 0x00;
		dskdma = 0x0000;
	}
	else if((addr & 0x00ff) == 0x000f) {//exit
		cpmsys_isrun = 0;
	}
}

uint8_t cpmports_in(register uint32_t addr) {
	if((addr & 0x00ff) == 0x0000) {//cstp
		cpmcons_getkey();

		return cpmconsst;
	}
	else if((addr & 0x00ff) == 0x0001) {//ciop
		cpmconsst = 0x00;

		return cpmconsch;
	}
	else if((addr & 0x00ff) == 0x0002) {//sstp
		register uint32_t status = 0x00;
#ifndef __SIMULATION
		if(LL_USART_IsActiveFlag_RXNE(USART1))//ready for Rx
			status |= 0x02;
		if(LL_USART_IsActiveFlag_TXE(USART1))//ready for Tx
			status |= 0x01;
#endif
		return status;
	}
	else if((addr & 0x00ff) == 0x0003) {//siop
#ifndef __SIMULATION
		return LL_USART_ReceiveData8(USART1);
#else
		return 0x00;
#endif
	}
	else if((addr & 0x00ff) == 0x000d) {//dcmdp
		return dskst;
	}
	else
		return 0x00;
}
