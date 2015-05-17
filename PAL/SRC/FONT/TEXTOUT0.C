/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  TEXTOUT0.C
   Author:  Gilles Kohl
   Started: 17. Dec. 94
   Subject: Unified font support: Trivial version of TextOut
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
                            global variables
   -------------------------------------------------------------------- */
int (*TextOut)(int x, int y, int Style, int FntId, char *s) = TextOut0;


/* --------------------------------------------------------------------
                              Functions
   -------------------------------------------------------------------- */

/* ------------------------------------------------------------
   TextOut0:
   Writes string using built-in font, returns width.
   ------------------------------------------------------------ */

int TextOut0(int x, int y, int Style, int FntId, char *s)
{
   if(!PalStateSaved++) SaveState(&PalState);
   SetRule(Style);
   SelectFont(FntId);
   WriteText(x, y, s);
   if(!--PalStateSaved) RestoreState(&PalState);
   return TextExt0(FntId, s);
}


