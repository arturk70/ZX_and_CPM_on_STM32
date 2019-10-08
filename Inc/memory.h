/*
 * memory.h
 *
 *  Created on: Oct 8, 2019
 *      Author: artur
 */

#ifndef MEMORY_H_
#define MEMORY_H_

#include "main.h"

static uint8_t (*memread)(uint16_t addr);
static void (*memwrite)(uint16_t addr, uint8_t data);

#endif /* MEMORY_H_ */
