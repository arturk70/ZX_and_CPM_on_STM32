/*
 * Z80.h
 *
 *  Created on: Oct 14, 2019
 *      Author: artur
 */

#ifndef Z80_H_
#define Z80_H_

#include "main.h"

/* Macros used for accessing the registers */
#define A   regs.af.b.h
#define F   regs.af.b.l
#define AF  regs.af.w

#define B   regs.bc.b.h
#define C   regs.bc.b.l
#define BC  regs.bc.w

#define D   regs.de.b.h
#define E   regs.de.b.l
#define DE  regs.de.w

#define H   regs.hl.b.h
#define L   regs.hl.b.l
#define HL  regs.hl.w

#define A_  regs.af_.b.h
#define F_  regs.af_.b.l
#define AF_ regs.af_.w

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

#define IR ( ( regs.i ) << 8 | ( regs.r7 & 0x80 ) | ( regs.r & 0x7f ) )

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


typedef union {
  struct { uint8_t h,l; } b;
  uint16_t w;
} regpair;

typedef struct {
  regpair af,bc,de,hl;
  regpair af_,bc_,de_,hl_;
  regpair ix,iy;
  uint8_t i;
  uint16_t r;	/* The low seven bits of the R register. 16 bits long
			   so it can also act as an RZX instruction counter */
  uint8_t r7;	/* The high bit of the R register */
  regpair sp,pc;
  regpair memptr;	/* The hidden register */
  uint8_t iff1, iff2, im;
} z80_registers;

typedef struct {
	uint8_t iff2_read;
	uint8_t halted;
	int z80_interrupt_event;
	int z80_nmi_event;
	int z80_nmos_iff2_event;
  /* Interrupts were enabled at this time; do not accept any interrupts
     until tstates > this value */
	int32_t interrupts_enabled_at;

} z80_state;

void Z80_Init(void (*outfn)(uint16_t addr, uint8_t data), uint8_t (*infn)(uint16_t addr));
void z80_interrupt();
void z80_nmi();
uint8_t Z80_Step();


#endif /* Z80_H_ */
