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
#define HBYTE	1
#define LBYTE	0

#define A   regs.af[HBYTE]
#define F   regs.af[LBYTE]
#define AF  (*(uint16_t*)regs.af)

#define B   regs.bc[HBYTE]
#define C   regs.bc[LBYTE]
#define BC  (*(uint16_t*)regs.bc)

#define D   regs.de[HBYTE]
#define E   regs.de[LBYTE]
#define DE  (*(uint16_t*)regs.de)

#define H   regs.hl[HBYTE]
#define L   regs.hl[LBYTE]
#define HL  (*(uint16_t*)regs.hl)

#define A_   regs.af_[HBYTE]
#define F_   regs.af_[LBYTE]
#define AF_  (*(uint16_t*)regs.af_)

#define B_   regs.bc_[HBYTE]
#define C_   regs.bc_[LBYTE]
#define BC_  (*(uint16_t*)regs.bc_)

#define D_   regs.de_[HBYTE]
#define E_   regs.de_[LBYTE]
#define DE_  (*(uint16_t*)regs.de_)

#define H_   regs.hl_[HBYTE]
#define L_   regs.hl_[LBYTE]
#define HL_  (*(uint16_t*)regs.hl_)

#define IXH   regs.ix[HBYTE]
#define IXL   regs.ix[LBYTE]
#define IX  (*(uint16_t*)regs.ix)

#define IYH   regs.iy[HBYTE]
#define IYL   regs.iy[LBYTE]
#define IY  (*(uint16_t*)regs.iy)

#define SPH   regs.sp[HBYTE]
#define SPL   regs.sp[LBYTE]
#define SP  (*(uint16_t*)regs.sp)

#define PCH   regs.pc[HBYTE]
#define PCL   regs.pc[LBYTE]
#define PC  (*(uint16_t*)regs.pc)

#define I  regs.i
#define R  regs.r

#define IFF1 regs.iff1
#define IFF2 regs.iff2
#define IM   regs.im

#define INC_R()	{regs.r = (regs.r & 0x80) | ((regs.r+1) & 0x7f);}

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

#define HLIXIY_REG (*((uint16_t*)regs.hlixiyptr))
#define HLIXIY_REGH ((*(regs.hlixiyptr))[HBYTE])
#define HLIXIY_REGL ((*(regs.hlixiyptr))[LBYTE])

#define IS_PREFIX (state.prefix)
#define IS_DDFD_PREFIX (state.prefix & 0xff00)
#define IS_DDFDCB_PREFIX ((state.prefix == 0xddcb) || (state.prefix == 0xfdcb))
#define IS_DD_PREFIX ((state.prefix & 0xff00) == 0xdd00)
#define IS_FD_PREFIX ((state.prefix & 0xff00) == 0xfd00)
#define IS_ED_PREFIX ((state.prefix & 0x00ff) == 0x00ed)
#define IS_CB_PREFIX ((state.prefix & 0x00ff) == 0x00cb)
#define CLR_PREFIX() { state.prefix = 0; }

typedef uint8_t regpair[2];

typedef struct {
  regpair bc,de,hl,af;
  regpair bc_,de_,hl_,af_;
  regpair ix,iy;
  uint8_t i;
  uint8_t r;
  regpair sp,pc;
  uint8_t iff1, iff2, im;
  regpair* hlixiyptr; //pointer to HL or IX or IY register for DD/FD prefixes
  int8_t ixiyshift;
} z80_registers_t;

typedef struct {
	uint8_t halted;
	uint16_t prefix;
	int16_t int_req;
	int16_t nmi_req;
	uint8_t int_blocked;//interrupt can be blocked for next command by prefixes or EI

} z80_state_t;

extern z80_registers_t regs;
extern z80_state_t state;
extern int z80_tstates;

extern void (*port_out)(register uint16_t addr, register uint8_t data);
extern uint8_t (*port_in)(register uint16_t addr);

void z80_Init(void (*outfn)(register uint16_t addr, register uint8_t data), uint8_t (*infn)(register uint16_t addr));
void z80_reset();
void z80_interrupt();
void z80_nmi();
void req_int(register uint32_t tstates);
void req_nmi(register uint32_t tstates);
void z80_step();


#endif /* Z80_H_ */
