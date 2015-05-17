/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  EMSFREE.C
   Author:  Gilles Kohl
   Started: 07. Oct. 95
   Subject: Free an allocated EMS block
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
   EmsFree:
   Subject: Free an allocated EMS block. Returns TRUE on success
   -------------------------------------------------------------------- */

int EmsFree(int Handle)
{
   union REGS regs;

   regs.h.ah = 0x45;
   regs.x.dx = Handle;
   int86(EMS_INT, &regs, &regs); /* call EMM */
   return !(EmsErr = regs.h.ah);
}

