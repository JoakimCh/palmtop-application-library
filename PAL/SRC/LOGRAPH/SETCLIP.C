/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  SETCLIP.C                             TESTED on TC 2.0 OK
   Author:  Harry Konstas
   Started: 11/20/1994
   Subject: Defines a rectangular clip region.
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                       standard includes
   -------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>

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
                           functions
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
   SetClip:
   Defines a rectangular clip region.
   -------------------------------------------------------------------- */

void SetClip(int x1, int y1, int x2, int y2)
{
    union REGS inregs, outregs;
    struct SREGS segregs;

    inregs.h.ah = 4;
    inregs.x.cx = x1;
    inregs.x.dx = y1;
    inregs.x.si = x2;
    inregs.x.di = y2;

    int86x(0x5f, &inregs, &outregs, &segregs);
}

