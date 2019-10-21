/*
 * z80_ops.h
 *
 *  Created on: Oct 18, 2019
 *      Author: artur
 */

#ifndef SRC_Z80_OPS_H_
#define SRC_Z80_OPS_H_

#include "z80.h"

#ifndef __SIMULATION
#define ERROR(code) {LL_GPIO_ResetOutputPin(LED_GPIO_Port, LED_Pin); LL_GPIO_SetOutputPin(LED_GPIO_Port, LED_Pin);}
#else
#define ERROR(code) printf("Error executing code (0x%04x)0x%02x", state.prefix, code);
#endif

typedef uint8_t (*z80cmd_t)(uint8_t code);

uint8_t NONI(uint8_t code);
uint8_t CTR(uint8_t code);
uint8_t EDCT(uint8_t code);
uint8_t LD_(uint8_t code);
uint8_t EDLD(uint8_t code);
uint8_t INC(uint8_t code);
uint8_t DEC(uint8_t code);
uint8_t ALU(uint8_t code);
uint8_t EDAL(uint8_t code);
uint8_t JMP(uint8_t code);
uint8_t STK(uint8_t code);
uint8_t SFT(uint8_t code);
uint8_t EDSF(uint8_t code);
uint8_t BIT(uint8_t code);
uint8_t EX_(uint8_t code);
uint8_t IO_(uint8_t code);
uint8_t EDIO(uint8_t code);
uint8_t DAA(uint8_t code);

uint8_t PFX(uint8_t code);

//extern const uint8_t optstates[256];

extern const z80cmd_t z80ops[256];
extern const z80cmd_t z80edops[96];

#endif /* SRC_Z80_OPS_H_ */
