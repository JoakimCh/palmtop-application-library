/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  TEXTEXT1.C
   Author:  Gilles Kohl
   Started: 17. Dec. 94
   Subject: Unified font support: Extended version of TextExt
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                          standard includes
   -------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* --------------------------------------------------------------------
                           local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"

/* --------------------------------------------------------------------
                              Functions
   -------------------------------------------------------------------- */

/* ------------------------------------------------------------
   TextExt1:
   Computes text extent of a string in a built-in or loaded font
   ------------------------------------------------------------ */
int TextExt1(int FntId, char *s)
{
   /* A built-in font has a positive Id */
   if(FntId > 0) return TextExt0(FntId, s);

   /* not a built-in font, compute array offset from Id */
   FntId = -(FntId+1);
   if(FntId < MAXFONTENTRIES && PalFontTab[FntId] != NULL) {
      return TextExtHfn(PalFontTab[FntId], s);
   }
   /* Id is out of valid range, return zero */
   return 0;
}


