/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  SETMSK.C
   Author:  Harry Konstas/Gilles Kohl
   Started: 10. Nov. 94
   Subject: Set fill pattern
   Modified:21 May 1995 by HK
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

void SetMask(char far *Mask)
{
   union REGS regs;
   struct SREGS segregs;
   int i;

   for(i = 0; i < 8; i++) PalState.sh_mask[i] = Mask[i]; /* save current mask */
   segread(&segregs);
   regs.x.di   = FP_OFF(Mask);
   segregs.es  = FP_SEG(Mask);
   regs.h.ah   = 1;
   int86x(0x5f, &regs, &regs, &segregs);
}


