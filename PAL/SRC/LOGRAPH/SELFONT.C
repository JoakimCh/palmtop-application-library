/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  SELFONT.C
   Author:  Harry Konstas/Gilles Kohl
   Started: 10. Nov. 94
   Subject: Select a font as the current font
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


void SelectFont(int Size)
{
   union REGS regs;
   struct SREGS segregs;

   PalState.sh_font = Size;   /* save current state */

   CallInt(0x5f, 0x1000, 0, Size, 0, &regs);
   segregs.es  = regs.x.dx;
   regs.x.di   = regs.x.ax;
   regs.h.ah   = 0x11;
   int86x(0x5f, &regs, &regs, &segregs);
}

