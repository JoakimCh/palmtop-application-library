/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  BATTFAST.C
   Author:  Andreas Garzotto
   Started: Aug. 27 1995
   Subject: Detect battery fast/trickle  charging.
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                          standard includes
   -------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdio.h>

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
    BattFast: return true if in fast charging mode, FALSE if trickle.
   -------------------------------------------------------------------- */

int BattFast(void)
{
   union REGS regs;

   regs.x.ax = 0x6002;
   int86(0x15, &regs, &regs);

   return !!(regs.x.ax & 32);
}
