/*
 * ext_memory.c
 *
 *  Created on: Oct 8, 2019
 *      Author: artur
 */

#include "ext_memory.h"

static uint16_t bl[4], br[4], bt[4], bb[4]; // 0-top bar, 1 - left bar, 2 - right bar, 3 - bottom bar
static uint16_t bl_straddr[4], bl_endaddr[4];
static uint32_t straddr;
static uint8_t cache[CACHE_SIZE], writed;

static uint16_t calc_cache_block(uint16_t addr, uint16_t *x1, uint16_t *y1, uint16_t *x2, uint16_t *y2) {
	uint16_t straddr = addr & ~((uint16_t)CACHE_SIZE-1);

	uint8_t blnum = 0;
	while((straddr < bl_straddr[blnum]) || (straddr > bl_endaddr[blnum])) blnum++;

	uint16_t pixnum = (addr - bl_straddr[blnum]) / 2;
	*x1 = bl[blnum] + pixnum % (br[blnum] - bl[blnum] + 1);
	*y1 = bt[blnum] + pixnum / (br[blnum] - bl[blnum] + 1);

	if(straddr/2 < ILI9341_PWIDTH*(ILI9341_PHEIGHT-192)) {
		*x2 = *x1 + CACHE_SIZE/2 - 1; *y2 = *y1;
	}
	else {
		*x2 = *x1 + 7; *y2 = *y1 + CACHE_SIZE/16 - 1;
	}

	return straddr;
}

static void update_cache(uint16_t addr) {//return index of appropriate cache block, update if need
	if((addr >= straddr) && (addr < straddr+CACHE_SIZE))
		return;

	uint16_t x1, y1, x2, y2;

	if(writed) {
		calc_cache_block(straddr, &x1, &y1, &x2, &y2);
		ILI9341_sendBuf(x1, y1, x2, y2, (uint16_t*)cache, CACHE_SIZE/2);
	}

	straddr = calc_cache_block(addr, &x1, &y1, &x2, &y2);
	writed = 0;
	ILI9341_readBuf(x1, y1, x2, y2, (uint16_t*)cache, CACHE_SIZE/2);
}

uint8_t extmem_read(uint16_t addr) {
	update_cache(addr);

	return cache[addr - straddr];

}

void extmem_write(uint16_t addr, uint8_t data) {
	update_cache(addr);
	writed = 1;

	cache[addr - straddr] = data;
}

void extmem_Init() {

	bl[0] = 0;   br[0] = ILI9341_PWIDTH-1; bt[0] = 0;   bb[0] = 23;
	bl[1] = 0;   br[1] = ILI9341_PWIDTH-1; bt[1] = 216; bb[1] = ILI9341_PHEIGHT-1;
	bl[2] = 0;   br[2] = 7;                bt[2] = 24;  bb[2] = 215;
	bl[3] = 312; br[3] = ILI9341_PWIDTH-1; bt[3] = 24;  bb[3] = 215;
	bl_straddr[0] = 0;
	bl_endaddr[0]=(br[0]-bl[0]+1)*(bb[0]-bt[0]+1)*2-1;
	bl_straddr[1] = bl_endaddr[0]+1;
	bl_endaddr[1]=bl_straddr[1]+(br[1]-bl[1]+1)*(bb[1]-bt[1]+1)*2-1;
	bl_straddr[2] = bl_endaddr[1]+1;
	bl_endaddr[2]=bl_straddr[2]+(br[2]-bl[2]+1)*(bb[2]-bt[2]+1)*2-1;
	bl_straddr[3] = bl_endaddr[2]+1;
	bl_endaddr[3]=bl_straddr[3]+(br[3]-bl[3]+1)*(bb[3]-bt[3]+1)*2-1;
	straddr = 0xffff;
	writed = 0;
	ILI9341_readBuf(0, 0, 0, 0, (uint16_t*)cache, CACHE_SIZE/2);
}

void extmem_deInit() {
}
