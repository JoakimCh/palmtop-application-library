/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  BATTVOLT.C
   Author:  Harry Konstas
   Started: Aug. 3 1995
   Subject: Battery voltage readout
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
    BattVoltage: read battery voltage. Specify which battery
                 MAIN_BATT or BACK_BATT. Returns battery voltage.
                 Ex: 243 = 2.43 volts.
   -------------------------------------------------------------------- */

int BattVoltage(int battery)
{
   union REGS regs;
   struct SREGS segregs;

   regs.x.ax = 0x6000;  /* Main battery */
   if(battery==BACK_BATT) regs.x.ax = 0x6001;
   int86x(0x15, &regs, &regs, &segregs);

   return (27 * regs.x.ax)/10 + 157;
}

