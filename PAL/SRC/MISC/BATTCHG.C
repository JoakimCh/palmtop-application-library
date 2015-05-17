/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  BATTCHG.C
   Author:  Harry Konstas / Andreas Garzotto.
   Started: Aug. 27 1995
   Subject: Battery charging control
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
    ChargeMode:  Enable/disable main battery charging, or read charging
                 status. Use CHG_ENA to start battery charging, CHG_DIS
                 to stop. Use CHG_STAT to get status: TRUE if charging
                 is enabled, FALSE otherwise.
   -------------------------------------------------------------------- */

int ChargeMode(int mode)
{
   union REGS regs;
   struct SREGS segregs;

   regs.x.ax = 0x6004;
   regs.x.bx = mode;

   if(mode==CHG_STAT) regs.x.ax = 0x6002; /* get batt status */
   int86x(0x15, &regs, &regs, &segregs);

   AsmSTI();
   return !!(regs.x.ax & 8);
}


