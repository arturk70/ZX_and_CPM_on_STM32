/*
 * dispmem_cashed.c
 *
 *  Created on: Oct 8, 2019
 *      Author: artur
 */

#include "extmem_cashed.h"
#include "ili9341_driver.h"

static uint8_t cache[CASH_BLOCK_COUNT][CASH_BLOCK_SIZE];
static uint16_t start_addr[CASH_BLOCK_COUNT];
static uint32_t start_time[CASH_BLOCK_COUNT];
static uint32_t agecounter = 0;
static uint16_t bl[4], br[4], bt[4], bb[4]; // 0-top bar, 1 - left bar, 2 - right bar, 3 - bottom bar
static uint16_t bl_topaddr[4];

void calcblock(uint16_t addr, uint16_t* blp, uint16_t* brp, uint16_t* btp, uint16_t* bbp) {
	uint8_t bnum=4;

	for(uint8_t i=0;i<4;i++)
		if(addr <= bl_topaddr[i]) {
			bnum = i;
			break;
		}
	uint16_t staddr = 0;
	if(bnum>0) staddr = bl_topaddr[bnum-1]+1;
	uint8_t pixshift = ((addr-staddr)/CASH_BLOCK_SIZE)*CASH_BLOCK_SIZE/2;
	*btp = pixshift/(br[bnum]-bl[bnum]+1);
	if(bnum == 0 || bnum == 3) {
		*blp = pixshift-(*btp*(br[bnum]-bl[bnum]+1));
		*btp += bt[bnum];
		*blp += bl[bnum];
		*bbp = *btp;
		*brp = *blp + CASH_BLOCK_SIZE/2 - 1;
	}
	else {
		*blp = bl[bnum];
		*brp = br[bnum];
		*btp += bt[bnum];
		*bbp = *btp+CASH_BLOCK_SIZE/2/(br[bnum]-bl[bnum]+1)-1;
	}
}

uint8_t updateCache(uint16_t addr) {
	uint8_t newcache = 0;

	for(uint8_t i=1;i<CASH_BLOCK_COUNT;i++)
		if(start_time[i]<start_time[newcache])
			newcache = i;
	uint16_t lbl, lbr, lbt, lbb;
	calcblock(start_addr[newcache], &lbl, &lbr, &lbt, &lbb);
	ILI9341_sendBuf(lbl, lbt, lbr, lbb, (uint16_t*)cache[newcache]);
	start_addr[newcache] = (addr/CASH_BLOCK_SIZE)*CASH_BLOCK_SIZE;
	calcblock(start_addr[newcache], &lbl, &lbr, &lbt, &lbb);
	ILI9341_readBuf(lbl, lbt, lbr, lbb, (uint16_t*)cache[newcache]);

	return newcache;
}

inline static void read_write(uint16_t addr, uint8_t *data, uint8_t op) {
	uint8_t cachenum = 0xff;

	agecounter++;

	for(uint8_t i=0;i<CASH_BLOCK_COUNT;i++)
		if(addr >= start_addr[i] && addr<start_addr[i]+CASH_BLOCK_SIZE) {
			cachenum = i;
			break;
		}
	if(cachenum == 0xff)
		cachenum = updateCache(addr);
	if(op == 0)
		*data = cache[cachenum][addr-start_addr[cachenum]];
	else
		cache[cachenum][addr-start_addr[cachenum]] = *data;
}

uint8_t extmem_read(uint16_t addr) {
	uint8_t res;
	read_write(addr, &res, 0);
	return res;

}

void extmem_write(uint16_t addr, uint8_t data) {
	read_write(addr, &data, 1);
}

void extmem_Init(uint16_t b0l, uint16_t b0r, uint16_t b0t, uint16_t b0b,
		uint16_t b1l, uint16_t b1r, uint16_t b1t, uint16_t b1b,
		uint16_t b2l, uint16_t b2r, uint16_t b2t, uint16_t b2b,
		uint16_t b3l, uint16_t b3r, uint16_t b3t, uint16_t b3b) {
	bl[0] = b0l; br[0] = b0r; bt[0] = b0t; bb[0] = b0b;
	bl[1] = b1l; br[1] = b1r; bt[1] = b1t; bb[1] = b1b;
	bl[2] = b2l; br[2] = b2r; bt[2] = b2t; bb[2] = b2b;
	bl[3] = b3l; br[3] = b3r; bt[3] = b3t; bb[3] = b3b;
	bl_topaddr[0]=(br[0]-bl[0]+1)*(bb[0]-bt[0]+1)*2-1;
	for(uint8_t i=1;i<4;i++) {
		bl_topaddr[i]=bl_topaddr[i-1]+(br[i]-bl[i]+1)*(bb[i]-bt[i]+1)*2;
	}


}
