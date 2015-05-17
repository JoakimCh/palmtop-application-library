/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  TXTEXHFN.C
   Author:  Gilles Kohl/Harry Konstas
   Started: 7. Dec. 94
   Subject: Proportional font support: Compute text extent of a string
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
   TextExtHfn:
   Computes text extent of a string in a given font
   Returns Width if OK.
   Skips chars that are out of MinChar/MaxChar range
   ------------------------------------------------------------ */

int TextExtHfn(FONTDESC *pFont, char *p)
{
   BYTE *s = p;
   int c, Width;
   int MinChar = pFont->FontHeader.MinChar;
   int MaxChar = pFont->FontHeader.MaxChar;

   for(Width = 0; c = *s; s++) {
      /* Check Limits */
      if(c < MinChar || c > MaxChar) continue; /* skip illegal char */
      Width += pFont->pChar[c]->Width;
   }
   return Width;
}


