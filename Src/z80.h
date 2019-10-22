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
#define A   regs.fa.b.h
#define F   regs.fa.b.l
#define FA  regs.fa.w

#define B   regs.bc.b.h
#define C   regs.bc.b.l
#define BC  regs.bc.w

#define D   regs.de.b.h
#define E   regs.de.b.l
#define DE  regs.de.w

#define H   regs.hl.b.h
#define L   regs.hl.b.l
#define HL  regs.hl.w

#define A_  regs.fa_.b.h
#define F_  regs.fa_.b.l
#define FA_ regs.fa_.w

#define B_  regs.bc_.b.h
#define C_  regs.bc_.b.l
#define BC_ regs.bc_.w

#define D_  regs.de_.b.h
#define E_  regs.de_.b.l
#define DE_ regs.de_.w

#define H_  regs.hl_.b.h
#define L_  regs.hl_.b.l
#define HL_ regs.hl_.w

#define IXH regs.ix.b.h
#define IXL regs.ix.b.l
#define IX  regs.ix.w

#define IYH regs.iy.b.h
#define IYL regs.iy.b.l
#define IY  regs.iy.w

#define SPH regs.sp.b.h
#define SPL regs.sp.b.l
#define SP  regs.sp.w

#define PCH regs.pc.b.h
#define PCL regs.pc.b.l
#define PC  regs.pc.w

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

#define HLIXIY_REG ((*(regs.hlixiyptr)).w)
#define HLIXIY_REGH ((*(regs.hlixiyptr)).b.h)
#define HLIXIY_REGL ((*(regs.hlixiyptr)).b.l)

#define IS_PREFIX (state.prefix)
#define IS_DDFD_PREFIX (state.prefix & 0xff00)
#define IS_DD_PREFIX ((state.prefix & 0xff00) == 0xdd00)
#define IS_FD_PREFIX ((state.prefix & 0xff00) == 0xfd00)
#define IS_ED_PREFIX ((state.prefix & 0x00ff) == 0x00ed)
#define IS_CB_PREFIX ((state.prefix & 0x00ff) == 0x00cb)
#define CLR_PREFIX() { state.prefix = 0; }

typedef union {
#ifdef __SIMULATION
  struct { uint8_t l,h; } b;
#else
  struct { uint8_t l,h; } b;
#endif
  uint16_t w;
} regpair;

typedef struct {
  regpair bc,de,hl,fa;
  regpair bc_,de_,hl_,fa_;
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
