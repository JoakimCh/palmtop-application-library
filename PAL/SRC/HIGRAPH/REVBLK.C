/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  REVBLK.C
   Author:  Harry Konstas/Gilles Kohl
   Started: 10. Nov. 94
   Subject: Reverse a rectangular area.
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
   RevBlock:
   Reverse a rectangle given by upper left (x1,y1) and lower right
   (x2,y2) coordinates.
   -------------------------------------------------------------------- */

void RevBlock(int x1, int y1, int x2, int y2)
{
   if(!PalStateSaved++) SaveState(&PalState);
   SetRule(XOR_RULE);
   SetColor(BLACK_COLOR);
   Rectangle(x1, y1, x2, y2, SOLID_FILL);
   SetRule(FORCE_RULE);
   if(!--PalStateSaved) RestoreState(&PalState);
}


