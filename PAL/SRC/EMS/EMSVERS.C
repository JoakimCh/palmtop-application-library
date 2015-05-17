/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  EMSVERS.C
   Author:  Gilles Kohl
   Started: 08. Oct. 95
   Subject: Return version of EMS driver
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
   EmsVersion:
   Return version of EMS driver
   N.B.: Version is returned as 320 for V3.2, for example
   -------------------------------------------------------------------- */

int EmsVersion(void)
{
   union REGS   regs;

   regs.h.ah = 0x46;
   int86(EMS_INT, &regs, &regs); /* call EMM */
   EmsErr = regs.h.ah;
   return EmsErr ? EMS_ERR :
                   (regs.h.al & 15)*10 + (regs.h.al >> 4)*100;
}

