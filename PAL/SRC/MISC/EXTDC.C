/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  EXTDC.C
   Author:  Harry Konstas
   Started: Aug. 3 1995
   Subject: External DC status
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
     DCStatus: Get DC status: TRUE if palmtop connected to external
               DC adaptor, FALSE otherwise.
   -------------------------------------------------------------------- */

int DCStatus(void)
{
   union REGS regs;
   struct SREGS segregs;

   regs.x.ax = 0x6002;
   int86x(0x15, &regs, &regs, &segregs);

   return !(regs.x.ax & 64);
}


