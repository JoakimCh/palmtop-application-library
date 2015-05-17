/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  SCROLL.C                             TESTED on TC 2.0 OK
   Author:  Harry Konstas
   Started: 11/20/1994
   Subject: Scrolls a rectangular area of the screen
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
   Scroll:
   Scrolls a rectangular area of the screen.
   Use these direction constants:  SCROLL_UP, SCROLL_DOWN, SCROLL_LEFT,
                                   SCROLL_RIGHT
   Use these color constants:      WHITE_COLOR, BLACK_COLOR
   -------------------------------------------------------------------- */

void Scroll(int Direction, int Distance, int Color,
            int x1, int y1, int x2, int y2)
{
    union REGS inregs, outregs;
    struct SREGS segregs;

    if(!PalStateSaved++) SaveState(&PalState);
    SetPos(x1, y1);

    inregs.h.ah = 0x12;
    inregs.h.al = Direction;
    inregs.x.cx = x2;
    inregs.x.dx = y2;
    inregs.x.si = Color;
    inregs.x.di = Distance;

    int86x(0x5f, &inregs, &outregs, &segregs);
    if(!--PalStateSaved) RestoreState(&PalState);
}

