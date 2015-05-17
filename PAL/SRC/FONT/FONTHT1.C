/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  FONTHT1.C
   Author:  Gilles Kohl
   Started: 17. Dec. 94
   Subject: Unified font support: Extended version of FontHeight
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
   FontHeight1:
   Computes height of a built-in or loadable font
   ------------------------------------------------------------ */

int FontHeight1(int FntId)
{
   /* A built-in font has a positive Id */
   if(FntId > 0) return FontHeight0(FntId);

   /* not a built-in font, compute array offset from Id */
   FntId = -(FntId+1);
   if(FntId < MAXFONTENTRIES && PalFontTab[FntId] != NULL) {
      return FontHeightHfn(PalFontTab[FntId]);
   }
   /* Id is out of valid range, return zero */
   return 0;
}


