/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  EMSPAGEC.C
   Author:  Gilles Kohl
   Started: 07. Oct. 95
   Subject: Return the total number of EMS pages available
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
   EmsPageCnt:
   Return the total number of EMS pages available
   -------------------------------------------------------------------- */

int EmsPageCnt(void)
{
   union REGS regs;

   regs.h.ah = 0x42;
   int86(EMS_INT, &regs, &regs); /* call EMM */
   EmsErr = regs.h.ah;
   return EmsErr ? EMS_ERR : regs.x.dx;
}

