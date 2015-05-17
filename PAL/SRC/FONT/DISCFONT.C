/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  DISCFONT.C
   Author:  Gilles Kohl
   Started: 17. Dec. 94
   Subject: Unified font support: Discard a loaded font (given by handle)
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
   DiscardFont:
   Discards (Removes) font from memory
   (ignores request if called for a resident font)
   ------------------------------------------------------------ */

void DiscardFont(int FntId)
{
   if(FntId > 0) return; /* ignore requests to discard built-in fonts */

   FntId = -(FntId+1);
   if(FntId < MAXFONTENTRIES && PalFontTab[FntId] != NULL) {
      DiscardHfn(PalFontTab[FntId]);
      PalFontTab[FntId] = NULL; /* invalidate slot */
   }
   /* Id is out of valid range, do nothing */
}

