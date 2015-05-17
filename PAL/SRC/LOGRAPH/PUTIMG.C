/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  PUTIMG.C
   Author:  Harry Konstas/Gilles Kohl
   Started: 10. Nov. 94
   Subject: Output a bitmap to screen.
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                            msdos includes
   -------------------------------------------------------------------- */
#include <dos.h>

/* --------------------------------------------------------------------
                           local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"

/* --------------------------------------------------------------------
                           Functions
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
   PutImg:
   Output a bitmap at a given coordinate position, with a given output
   rule.
   -------------------------------------------------------------------- */

void PutImg(int x, int y, int Rule, void far *Bitmap)
{
   union  REGS regs;
   struct SREGS segregs;

   segread(&segregs);
   regs.x.cx  = x;
   regs.x.dx  = y;
   regs.x.di  = FP_OFF(Bitmap);
   segregs.es = FP_SEG(Bitmap);
   regs.h.ah  = 14;
   regs.h.al  = Rule;
   int86x(0x5f, &regs, &regs, &segregs);
}
