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
#define F   regs.f
#define A   regs.a
#define FA  (*(uint16_t*)(&regs.f))

#define B   regs.bc[1]
#define C   regs.bc[0]
#define BC  (*(uint16_t*)regs.bc)

#define D   regs.de[1]
#define E   regs.de[0]
#define DE  (*(uint16_t*)regs.de)

#define H   regs.hl[1]
#define L   regs.hl[0]
#define HL  (*(uint16_t*)regs.hl)

#define F_   regs.f_
#define A_   regs.a_
#define FA_  (*(uint16_t*)(&regs.f_))

#define B_   regs.bc_[1]
#define C_   regs.bc_[0]
#define BC_  (*(uint16_t*)regs.bc_)

#define D_   regs.de_[1]
#define E_   regs.de_[0]
#define DE_  (*(uint16_t*)regs.de_)

#define H_   regs.hl_[1]
#define L_   regs.hl_[0]
#define HL_  (*(uint16_t*)regs.hl_)

#define IXH   regs.ix[1]
#define IXL   regs.ix[0]
#define IX  (*(uint16_t*)regs.ix)

#define IYH   regs.iy[1]
#define IYL   regs.iy[0]
#define IY  (*(uint16_t*)regs.iy)

#define SPH   regs.sp[1]
#define SPL   regs.sp[0]
#define SP  (*(uint16_t*)regs.sp)

#define PCH   regs.pc[1]
#define PCL   regs.pc[0]
#define PC  (*(uint16_t*)regs.pc)

#define I  regs.i
#define R  regs.r
#define R7 regs.r7

#define IFF1 regs.iff1
#define IFF2 regs.iff2
#define IM   regs.im

#define IR ((regs.i) << 8 | (regs.r7 & 0x80) | (regs.r & 0x7f))

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
#define HLIXIY_REGH ((*(regs.hlixiyptr))[1])
#define HLIXIY_REGL ((*(regs.hlixiyptr))[0])

#define IS_PREFIX (state.prefix)
#define IS_DDFD_PREFIX (state.prefix & 0xff00)
#define IS_DD_PREFIX ((state.prefix & 0xff00) == 0xdd00)
#define IS_FD_PREFIX ((state.prefix & 0xff00) == 0xfd00)
#define IS_ED_PREFIX ((state.prefix & 0x00ff) == 0x00ed)
#define IS_CB_PREFIX ((state.prefix & 0x00ff) == 0x00cb)
#define CLR_PREFIX() { state.prefix = 0; }

//typedef union {
//  struct { uint8_t l,h; } b;
//  uint16_t w;
//} regpair;
typedef uint8_t regpair[2];

typedef struct {
  regpair bc,de,hl;
  uint8_t f,a;
  regpair bc_,de_,hl_;
  uint8_t f_,a_;
  regpair ix,iy;
  uint8_t i;
  uint16_t r;	/* The low seven bits of the R register. 16 bits long
			   so it can also act as an RZX instruction counter */
  uint8_t r7;	/* The high bit of the R register */
  regpair sp,pc;
  regpair* hlixiyptr; //pointer to HL or IX or IY register for DD/FD prefixes
  uint8_t ixiyshift;
  uint8_t iff1, iff2, im;
} z80_registers;

typedef struct {
//	uint8_t iff2_read;
	uint8_t halted;
	uint16_t prefix;
	uint32_t int_req;
	uint32_t nmi_req;
	uint8_t int_blocked;//interrupt can be blocked for next command by prefixes or EI

} z80_state;

extern z80_registers regs;
extern z80_state state;

extern void (*port_out)(uint16_t addr, uint8_t data);
extern uint8_t (*port_in)(uint16_t addr);

void z80_Init(void (*outfn)(uint16_t addr, uint8_t data), uint8_t (*infn)(uint16_t addr));
void z80_reset();
uint8_t z80_interrupt();
uint8_t z80_nmi();
void req_int(uint32_t tstates);
void req_nmi(uint32_t tstates);
uint8_t z80_step();


#endif /* Z80_H_ */
