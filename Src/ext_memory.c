/*
 * ext_memory.c
 *
 *  Created on: Oct 8, 2019
 *      Author: artur
 */

#include "ext_memory.h"

static uint16_t bl[4], br[4], bt[4], bb[4]; // 0-top bar, 1 - left bar, 2 - right bar, 3 - bottom bar
static uint16_t bl_straddr[4], bl_endaddr[4];
static cache_t cache[CACHE_BLOCKS_NUM];

static void calc_pix(uint16_t addr, uint16_t *x, uint16_t *y) {
	uint8_t blnum = 0;
	while((addr < bl_straddr[blnum]) || (addr > bl_endaddr[blnum])) blnum++;

	uint16_t pixnum = (addr-bl_straddr[blnum]) / 2;
	*x = bl[blnum] + pixnum % (br[blnum] - bl[blnum] + 1);
	*y = bt[blnum] + pixnum / (br[blnum] - bl[blnum] + 1);
}

static uint16_t calc_cache_block(uint16_t addr, uint16_t *x1, uint16_t *y1, uint16_t *x2, uint16_t *y2) {
	uint16_t straddr = addr & ~((uint16_t)CACHE_BLOCK_SIZE-1);

	calc_pix(straddr, x1, y1);
	if(straddr/2 < ILI9341_PWIDTH*(ILI9341_PHEIGHT-192)) {
		*x2 = *x1 + CACHE_BLOCK_SIZE/2 - 1; *y2 = *y1;
	}
	else {
		*x2 = *x1 + 7; *y2 = *y1 + CACHE_BLOCK_SIZE/16 - 1;
	}

	return straddr;
}

static uint8_t read_cache(uint16_t addr, uint8_t *data) {
	for(uint8_t i=0; i<CACHE_BLOCKS_NUM;i++) {
		if(addr>=cache[i].straddr && addr <= (cache[i].straddr+CACHE_BLOCK_SIZE-1)) {
			*data = cache[i].data[addr - cache[i].straddr];
			cache[i].usage++;
			//cache[i].usage = SysTick->VAL;
			return 1;
		}
	}

	return 0;
}

static uint8_t write_cache(uint16_t addr, uint8_t data) {
	for(uint8_t i=0; i<CACHE_BLOCKS_NUM;i++) {
		if(addr>=cache[i].straddr && addr <= (cache[i].straddr+CACHE_BLOCK_SIZE-1)) {
			cache[i].data[addr - cache[i].straddr] = data;
			cache[i].usage++;
			//cache[i].usage = SysTick->VAL;
			return 1;
		}
	}

	return 0;
}

static void update_cache(uint16_t addr) {
	uint8_t lfu_num = 0;
	for(uint8_t i=1; i<CACHE_BLOCKS_NUM;i++) {
		if(cache[i].usage < cache[lfu_num].usage) lfu_num = i;
	}

	uint16_t x1, y1, x2, y2;

	if(cache[lfu_num].usage != 0) {
		calc_cache_block(cache[lfu_num].straddr, &x1, &y1, &x2, &y2);
		ILI9341_sendBuf(x1, y1, x2, y2, (uint16_t*)cache[lfu_num].data);
	}

	cache[lfu_num].straddr = calc_cache_block(addr, &x1, &y1, &x2, &y2);
	cache[lfu_num].usage = 0;
	ILI9341_readBuf(x1, y1, x2, y2, (uint16_t*)cache[lfu_num].data);
}

uint8_t extmem_read(uint16_t addr) {
	uint8_t res;

	if(read_cache(addr, &res)) return res;

	uint16_t x, y;
	uint8_t r,g,b;
	calc_pix(addr, &x, &y);

#ifndef __SIMULATION
	ILI9341_WAIT_DMA();

	ILI9341_setFrame(x, y, x, y);
	ILI9341_sendCommand(ILI9341_RAMRD);

	LL_SPI_SetBaudRatePrescaler(ILI9341_SPI, LL_SPI_BAUDRATEPRESCALER_DIV8);

	while(LL_SPI_IsActiveFlag_TXE(ILI9341_SPI) == 0);
	LL_SPI_TransmitData8(ILI9341_SPI, 0xaa);
	while(LL_SPI_IsActiveFlag_RXNE(ILI9341_SPI) == 0);
	r=LL_SPI_ReceiveData8(ILI9341_SPI);//read dummy byte
	while(LL_SPI_IsActiveFlag_TXE(ILI9341_SPI) == 0);
	LL_SPI_TransmitData8(ILI9341_SPI, 0xaa);
	while(LL_SPI_IsActiveFlag_RXNE(ILI9341_SPI) == 0);
	r=LL_SPI_ReceiveData8(ILI9341_SPI);
	while(LL_SPI_IsActiveFlag_TXE(ILI9341_SPI) == 0);
	LL_SPI_TransmitData8(ILI9341_SPI, 0xaa);
	while(LL_SPI_IsActiveFlag_RXNE(ILI9341_SPI) == 0);
	g=LL_SPI_ReceiveData8(ILI9341_SPI);
	while(LL_SPI_IsActiveFlag_TXE(ILI9341_SPI) == 0);
	LL_SPI_TransmitData8(ILI9341_SPI, 0xaa);
	while(LL_SPI_IsActiveFlag_RXNE(ILI9341_SPI) == 0);
	b=LL_SPI_ReceiveData8(ILI9341_SPI);
	while(LL_SPI_IsActiveFlag_BSY(ILI9341_SPI) != 0);
	ILI9341_CS_SET;
	ILI9341_CS_RESET;

	LL_SPI_SetBaudRatePrescaler(ILI9341_SPI, LL_SPI_BAUDRATEPRESCALER_DIV2);
#else
	ILI9341_readPix(x, y, &r, &g, &b);
#endif

	if(0 == (addr % 2))
		res=(((g & 0xfc) << 3u) | (b >> 3u)); //get L byte
	else
		res=((r & 0xF8) | (g >> 5u)); //get H byte

	update_cache(addr);

	return res;

}

void extmem_write(uint16_t addr, uint8_t data) {

	if(write_cache(addr, data)) return;

	uint16_t x, y;
	uint8_t r,g,b;
	calc_pix(addr, &x, &y);

#ifndef __SIMULATION
	ILI9341_WAIT_DMA();

	ILI9341_setFrame(x, y, x, y);
	ILI9341_sendCommand(ILI9341_RAMRD);

	LL_SPI_SetBaudRatePrescaler(ILI9341_SPI, LL_SPI_BAUDRATEPRESCALER_DIV8);

	while(LL_SPI_IsActiveFlag_TXE(ILI9341_SPI) == 0);
	LL_SPI_TransmitData8(ILI9341_SPI, 0xaa);
	while(LL_SPI_IsActiveFlag_RXNE(ILI9341_SPI) == 0);
	r=LL_SPI_ReceiveData8(ILI9341_SPI);//read dummy byte
	while(LL_SPI_IsActiveFlag_TXE(ILI9341_SPI) == 0);
	LL_SPI_TransmitData8(ILI9341_SPI, 0xaa);
	while(LL_SPI_IsActiveFlag_RXNE(ILI9341_SPI) == 0);
	r=LL_SPI_ReceiveData8(ILI9341_SPI);
	while(LL_SPI_IsActiveFlag_TXE(ILI9341_SPI) == 0);
	LL_SPI_TransmitData8(ILI9341_SPI, 0xaa);
	while(LL_SPI_IsActiveFlag_RXNE(ILI9341_SPI) == 0);
	g=LL_SPI_ReceiveData8(ILI9341_SPI);
	while(LL_SPI_IsActiveFlag_TXE(ILI9341_SPI) == 0);
	LL_SPI_TransmitData8(ILI9341_SPI, 0xaa);
	while(LL_SPI_IsActiveFlag_RXNE(ILI9341_SPI) == 0);
	b=LL_SPI_ReceiveData8(ILI9341_SPI);
	while(LL_SPI_IsActiveFlag_BSY(ILI9341_SPI) != 0);
	ILI9341_CS_SET;
	ILI9341_CS_RESET;

	LL_SPI_SetBaudRatePrescaler(ILI9341_SPI, LL_SPI_BAUDRATEPRESCALER_DIV2);
#else
	ILI9341_readPix(x, y, &r, &g, &b);
#endif

	uint8_t pixh, pixl;
	pixh = (r & 0xF8) | (g >> 5u);
	pixl = ((g & 0xfc) << 3u) | (b >> 3u);
	if(0 == (addr % 2))
		pixl = data; //to L byte
	else
		pixh = data; //to H byte

#ifndef __SIMULATION
	ILI9341_sendCommand(ILI9341_GRAM);
	ILI9341_sendData(pixh);
	ILI9341_sendData(pixl);
#else
	ILI9341_writePix(x, y, (pixh << 8) | pixl);

	update_cache(addr);
#endif
}

void extmem_Init(uint16_t b0l, uint16_t b0r, uint16_t b0t, uint16_t b0b,
		uint16_t b1l, uint16_t b1r, uint16_t b1t, uint16_t b1b,
		uint16_t b2l, uint16_t b2r, uint16_t b2t, uint16_t b2b,
		uint16_t b3l, uint16_t b3r, uint16_t b3t, uint16_t b3b) {
	bl[0] = b0l; br[0] = b0r; bt[0] = b0t; bb[0] = b0b;
	bl[1] = b1l; br[1] = b1r; bt[1] = b1t; bb[1] = b1b;
	bl[2] = b2l; br[2] = b2r; bt[2] = b2t; bb[2] = b2b;
	bl[3] = b3l; br[3] = b3r; bt[3] = b3t; bb[3] = b3b;
	bl_straddr[0] = 0;
	bl_endaddr[0]=(br[0]-bl[0]+1)*(bb[0]-bt[0]+1)*2-1;
	bl_straddr[1] = bl_endaddr[0]+1;
	bl_endaddr[1]=bl_straddr[1]+(br[1]-bl[1]+1)*(bb[1]-bt[1]+1)*2-1;
	bl_straddr[2] = bl_endaddr[1]+1;
	bl_endaddr[2]=bl_straddr[2]+(br[2]-bl[2]+1)*(bb[2]-bt[2]+1)*2-1;
	bl_straddr[3] = bl_endaddr[2]+1;
	bl_endaddr[3]=bl_straddr[3]+(br[3]-bl[3]+1)*(bb[3]-bt[3]+1)*2-1;
	for(uint8_t i=1; i<CACHE_BLOCKS_NUM;i++)
			cache[i].straddr = 0xffff;
}
