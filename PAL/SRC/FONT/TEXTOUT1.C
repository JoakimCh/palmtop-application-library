/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  TEXTOUT1.C
   Author:  Gilles Kohl
   Started: 17. Dec. 94
   Subject: Unified font support: Extended version of TextOut
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
   TextOut1:
   Writes string using built-in or loadable font, returns width.
   (Returns 0 in case of error)
   ------------------------------------------------------------ */

int TextOut1(int x, int y, int Style, int FntId, char *s)
{
   int retval = 0;

   if(!PalStateSaved++) SaveState(&PalState);

   /* A built-in font has a positive Id */
   if(FntId > 0) return TextOut0(x, y, Style, FntId, s);

   /* not a built-in font, compute array offset from Id */
   FntId = -(FntId+1);
   if(FntId < MAXFONTENTRIES && PalFontTab[FntId] != NULL) {
      SetRule(Style);
      retval = WriteHfn(PalFontTab[FntId], x, y, s);
   }

   if(!--PalStateSaved) RestoreState(&PalState);
   return retval;
}


