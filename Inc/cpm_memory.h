/*
 * cpm_memory.h
 *
 *  Created on: Oct 8, 2019
 *      Author: artur
 */

#ifndef CPM_MEMORY_H_
#define CPM_MEMORY_H_

uint8_t cpmmem_read(uint16_t addr);
void cpmmem_write(uint16_t addr, uint8_t data);
void cpmmem_Init();

#endif /* CPM_MEMORY_H_ */
