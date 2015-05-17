/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  EMSALLOC.C
   Author:  Gilles Kohl
   Started: 07. Oct. 95
   Subject: Allocate EMS pages, return handle
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
   EmsAlloc:
   Allocate EMS pages, return a handle or EMS_ERR on error
   -------------------------------------------------------------------- */

int EmsAlloc(int Pages)
{
   union REGS regs;

   regs.h.ah = 0x43;
   regs.x.bx = Pages;
   int86(EMS_INT, &regs, &regs); /* call EMM */
   EmsErr = regs.h.ah;
   return EmsErr ? EMS_ERR : regs.x.dx;
}

