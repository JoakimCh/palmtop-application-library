/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  WRTEXT.C
   Author:  Harry Konstas/Gilles Kohl
   Started: 10. Nov. 94
   Subject: Output a text string to the screen
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
   WriteText:
   Write a text string to the screen (using current color and style)
   at position (x,y)
   -------------------------------------------------------------------- */

void WriteText(int x, int y, char far *Str)
{
   union REGS regs;
   struct SREGS segregs;

   segread(&segregs);       /* read segment registers */
   regs.x.cx   = x;
   regs.x.dx   = y;
   regs.x.di   = FP_OFF(Str);
   segregs.es  = FP_SEG(Str);
   regs.x.ax   = 0x0f00;
   int86x(0x5f, &regs, &regs, &segregs);
}

