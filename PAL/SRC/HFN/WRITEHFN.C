/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  WRITEHFN.C
   Author:  Gilles Kohl/Harry Konstas
   Started: 7. Dec. 94
   Subject: Proportional font support
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
   WriteHfn:
   Writes string using Hfn font
   Returns Width.
   Skips chars that are out of MinChar/MaxChar range
   ------------------------------------------------------------ */

int WriteHfn(FONTDESC *pFont, int x, int y, char *p)
{
   BYTE *s = p;
   int c, Width;
   int MinChar = pFont->FontHeader.MinChar;
   int MaxChar = pFont->FontHeader.MaxChar;

   for(Width = 0; c = *s; s++) {
      /* Check Limits */
      if(c < MinChar || c > MaxChar) continue; /* skip illegal char */

      /* Draw character */
      PutImg(x+Width, y, FORCE_RULE, pFont->pChar[c]);
      Width += pFont->pChar[c]->Width;
   }
   return Width;
}


