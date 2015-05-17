/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  FNTHTHFN.C
   Author:  Gilles Kohl/Harry Konstas
   Started: 7. Dec. 94
   Subject: Proportional font support: Compute height of a font
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
   FontHeightHfn:
   Returns height of an Hfn given font
   ------------------------------------------------------------ */

int FontHeightHfn(FONTDESC *pFont)
{
   return pFont->FontHeader.MaxHeight;
}

