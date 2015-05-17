/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  EMSFREEC.C
   Author:  Gilles Kohl
   Started: 07. Oct. 95
   Subject: Return number of free EMS pages available
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
   EmsFreeCnt:
   Return the number of free EMS pages available
   -------------------------------------------------------------------- */
int EmsFreeCnt(void)
{
   union REGS regs;

   regs.h.ah = 0x42;
   int86(EMS_INT, &regs, &regs); /* call EMM */
   EmsErr = regs.h.ah;
   return EmsErr ? EMS_ERR : regs.x.bx;
}

