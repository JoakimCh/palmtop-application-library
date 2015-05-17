/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  REVBLK.C
   Author:  Gilles Kohl
   Started: 21. Jan. 95
   Subject: Clear a rectangular area.
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                           local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"

/* --------------------------------------------------------------------
                           Functions
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
   ClrBlock:
   Clear a rectangle given by upper left (x1,y1) and lower right
   (x2,y2) coordinates to given color
   -------------------------------------------------------------------- */

void ClrBlock(int x1, int y1, int x2, int y2, int color)
{
   if(!PalStateSaved++) SaveState(&PalState);
   SetColor(color);
   SetRule(FORCE_RULE);
   Rectangle(x1, y1, x2, y2, SOLID_FILL);
   SetColor(BLACK_COLOR);
   if(!--PalStateSaved) RestoreState(&PalState);
}

