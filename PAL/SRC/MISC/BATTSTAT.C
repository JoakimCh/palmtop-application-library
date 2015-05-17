/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  BATTSTAT.C
   Author:  Harry Konstas
   Started: Aug. 3 1995
   Subject: Battery status
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
    BattStatus:  Get battery status. Specify battery, MAIN_BATT,
                 BACK_BATT, or CARD_BATT. Returns TRUE if battery
                 is LOW, FALSE otherwise. Returns (2) if Main VERY LOW.
   -------------------------------------------------------------------- */

int BattStatus(int battery)
{
   union REGS regs;
   struct SREGS segregs;

   regs.x.ax = 0x6002;
   int86x(0x15, &regs, &regs, &segregs);

   if(battery==MAIN_BATT) if(!!(regs.x.ax & 0x4000)) return 2;
   if(battery==MAIN_BATT) return !!(regs.x.ax & 1);
   if(battery==BACK_BATT) return !!(regs.x.ax & 2);
   if(battery==CARD_BATT) return !!(regs.x.ax & 4);
}

