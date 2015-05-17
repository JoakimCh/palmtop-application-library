/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  DISCHFN.C
   Author:  Gilles Kohl/Harry Konstas
   Started: 7. Dec. 94
   Subject: Discard a proportional font
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                          standard includes
   -------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdio.h>

/* --------------------------------------------------------------------
                           local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"

/* --------------------------------------------------------------------
                              Functions
   -------------------------------------------------------------------- */

/* ------------------------------------------------------------
   DiscardHfn:
   Discards (Removes) font from memory
   (ignores request if called for a resident font)
   ------------------------------------------------------------ */

void DiscardHfn(FONTDESC *pFont)
{
    if(pFont->Discardable) free(pFont);
}

