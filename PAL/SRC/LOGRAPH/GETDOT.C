/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  GETDOT.C
   Author:  Harry Konstas
   Started: Aug. 3 1995
   Subject: Read the color of a pixel on the screen
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
    GetDot: Reads the color of the pixel at the x:y coordinates.
   -------------------------------------------------------------------- */

int GetDot(int x, int y)
{
   union REGS regs;
   struct SREGS segregs;

   regs.x.ax = 0x0c00;
   regs.x.cx = x;
   regs.x.dx = y;

   int86x(0x5f, &regs, &regs, &segregs);

   return regs.x.ax;    /* returns color, 0=WHT, 1=BLK */
}


