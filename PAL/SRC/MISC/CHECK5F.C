/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  CHECK5F.C
   Author:  Gilles Kohl
   Started: 10. Nov. 94
   Subject: Check for presence of INT 5F services.
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
   Check5f:
   Checks for the presence of the INT5F service (Either running on
   palmtop or on PC with emulator). Returns TRUE if service present.
   -------------------------------------------------------------------- */

int Check5f(void)
{
   union  REGS regs;

   CallInt(0x5f, 0x1000, 0, 0x0a0b, 0, &regs);
   return regs.x.ax == 0x0008;
}
