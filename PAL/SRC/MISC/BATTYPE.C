/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  BATTYPE.C
   Author:  Harry Konstas
   Started: Aug. 3 1995
   Subject: Battery type management
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
      BattType: read/set main battery type to NICD_BATT or ALK_BATT.
                Use BATT_STAT to read status only: returns NICD_BATT
                or ALK_BATT.
   -------------------------------------------------------------------- */

int BattType(int type)
{
   union REGS regs;
   struct SREGS segregs;

   regs.x.ax = 0x6003;                     /* set batt type   */
   regs.x.bx = type;

   if(type==BATT_STAT) regs.x.ax = 0x6002; /* get batt status */
   int86x(0x15, &regs, &regs, &segregs);

   return !!(regs.x.ax & 128);
}


