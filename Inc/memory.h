/*
 * memory.h
 *
 *  Created on: Oct 8, 2019
 *      Author: artur
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#include "ext_memory.h"

#define MEMTYPE_ZX	0
#define MEMTYPE_CPM	1

uint8_t mem_read(uint16_t addr);
void mem_write(uint16_t addr, uint8_t data);
void mem_Init(uint8_t type);
void mem_clear();
uint16_t mem_test();
uint8_t *get_ZX_videomem();

#endif /* MEMORY_H_ */
