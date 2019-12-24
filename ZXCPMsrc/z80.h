/*
 * Z80.h
 *
 *  Created on: Oct 14, 2019
 *      Author: artur
 */

#ifndef Z80_H_
#define Z80_H_

#include "main.h"
#include "memory.h"

/* Macros used for accessing the registers */
#define B   regs[1]
#define C   regs[0]
#define BC  (*(uint16_t*)&regs[0])

#define D   regs[3]
#define E   regs[2]
#define DE  (*(uint16_t*)&regs[2])

#define H   regs[5]
#define L   regs[4]
#define HL  (*(uint16_t*)&regs[4])

#define A   regs[7]
#define F   regs[6]
#define AF  (*(uint16_t*)&regs[6])

#define B_   regs[9]
#define C_   regs[8]
#define BC_  (*(uint16_t*)&regs[8])

#define D_   regs[11]
#define E_   regs[10]
#define DE_  (*(uint16_t*)&regs[10])

#define H_   regs[13]
#define L_   regs[12]
#define HL_  (*(uint16_t*)&regs[12])

#define A_   regs[15]
#define F_   regs[14]
#define AF_  (*(uint16_t*)&regs[14])

#define IXH   regs[17]
#define IXL   regs[16]
#define IX  (*(uint16_t*)&regs[16])

#define IYH   regs[19]
#define IYL   regs[18]
#define IY  (*(uint16_t*)&regs[18])

#define SPH   regs[21]
#define SPL   regs[20]
#define SP  (*(uint16_t*)&regs[20])

#define PCH   regs[23]
#define PCL   regs[22]
#define PC  (*(uint16_t*)&regs[22])

#define I  regs[24]
#define RR  regs[25]
#define R8 regs[26]

#define IFF1 regs[27]
#define IFF2 regs[28]
#define IM   regs[29]

/* The flags */
#define FLAG_C	0x01
#define FLAG_N	0x02
#define FLAG_P	0x04
#define FLAG_V	FLAG_P
#define FLAG_3	0x08
#define FLAG_H	0x10
#define FLAG_5	0x20
#define FLAG_Z	0x40
#define FLAG_S	0x80

#define HLIXIY_REG (*hlixiyptr)
#define HLIXIY_REGH (*((uint8_t*)hlixiyptr+1))
#define HLIXIY_REGL (*((uint8_t*)hlixiyptr))

#define IS_PREFIX (state.prefix)
#define IS_DD_PREFIX ((state.prefix & 0xff00) == 0xdd00)
#define IS_FD_PREFIX ((state.prefix & 0xff00) == 0xfd00)
#define IS_ED_PREFIX ((state.prefix & 0x00ff) == 0x00ed)
#define IS_CB_PREFIX ((state.prefix & 0x00ff) == 0x00cb)
#define CLR_PREFIX() { state.prefix = 0; }

#define INT_REQ	1
#define NMI_REQ	2

typedef struct {
	uint8_t halted;
	uint16_t prefix;
	int8_t int_req;//= INT_REQ, NMI_REQ
	uint8_t int_blocked;//interrupt can be blocked for next command by prefixes or EI

} z80_state_t;

extern uint8_t regs[30];
extern uint16_t* hlixiyptr;
extern int8_t gixiyshift;
extern z80_state_t state;
extern int z80_tstates;

extern void (*port_out)(register uint32_t addr, register uint32_t data);
extern uint8_t (*port_in)(register uint32_t addr);

void z80_Init(void (*outfn)(register uint32_t addr, register uint32_t data), uint8_t (*infn)(register uint32_t addr));
void z80_reset();
void z80_interrupt();
void z80_nmi();
void req_int(register uint32_t type);
void z80_step();


#endif /* Z80_H_ */
