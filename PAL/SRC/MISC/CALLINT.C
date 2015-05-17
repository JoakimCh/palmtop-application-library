/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  CALLINT.C
   Author:  Gilles Kohl
   Started: 10. Nov. 94
   Subject: Utility function: Call an interrupt with registers
   Modif:   GK941116: Added <dos.h> include for Turbo C compatibility
  -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                            msdos includes
   -------------------------------------------------------------------- */
#include <dos.h>

#ifndef LSI_C
/* LSI_C doesn't have BIOS.H, it is part of DOS.H there */
#include <bios.h>
#endif

/* --------------------------------------------------------------------
                           local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"

/* --------------------------------------------------------------------
                           Functions
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
   CallInt:
   utility function - will call a given interrupt and pass the
   most common registers. If a pointer to a REGS structure is
   provided, this structure will be used an return values can
   be found in it. If NULL is passed instead, an internal
   REGS structure is used.
   -------------------------------------------------------------------- */

void CallInt(int IntNo, WORD ax, WORD bx, WORD cx, WORD dx, union REGS *pr)
{
   union REGS r;

   if(!pr) pr = &r; /* If no structure passed, use our own */

   /* copy the register values ... */
   pr->x.ax = ax;
   pr->x.bx = bx;
   pr->x.cx = cx;
   pr->x.dx = dx;

   /* ... and do the call. */
   int86(IntNo, pr, pr);
}


