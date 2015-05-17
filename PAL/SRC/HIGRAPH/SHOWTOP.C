/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  SHOWTOP.C
   Author:  Dave Vickers
   Started: 20. Jan. 95
   Subject: Show top line of the screen
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                          standard includes
   -------------------------------------------------------------------- */

#include <stdio.h>
#include <string.h>

/* --------------------------------------------------------------------
                           local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"

/* --------------------------------------------------------------------
   ShowTopLine:
   Display a string on the top line of the HP display.
   Inputs:
      char *pTitle     - Title string for left  part of top line.
      char *pTime      - Title string for right part of top line.
                         (optional)
   Note: if two strings are given, one is displayed flush left and
   the other one flush right - if only one title is given, it will
   be centered.
   -------------------------------------------------------------------- */

void ShowTopLine(char *pTitle, char *pTime)
{
   if(!PalStateSaved++) SaveState(&PalState);
   SetColor(BLACK_COLOR);
   SetRule(FORCE_RULE);
   Rectangle(0, 0, MAX_X-1, 1+FNTD(SMALL_FONT), SOLID_FILL);

   SetColor(WHITE_COLOR);
   SetRule(TXT_RULE);
   SelectFont(SMALL_FONT);
   WriteText(pTime ? 0 : (MAX_X-FNTW(SMALL_FONT)*strlen(pTitle))/2,
             1, pTitle);
   if(pTime) {
      WriteText(MAX_X - 2 - (FNTW(SMALL_FONT) * strlen(pTime)), 1, pTime);
   }
   if(!--PalStateSaved) RestoreState(&PalState);
}


