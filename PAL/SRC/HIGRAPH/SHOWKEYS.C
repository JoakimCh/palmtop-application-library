/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  SHOWKEYS.C
   Author:  Gilles Kohl
   Started: 03. Jan. 95
   Subject: Show function key definitions
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
                      constant definitions
   -------------------------------------------------------------------- */
#define KEYPOSX   31
#define KEYPOSY  190
#define KEYWIDTH  57
#define KEYDEPTH   9

/* --------------------------------------------------------------------
                           Functions
   -------------------------------------------------------------------- */

void ShowFKeys(char **pKeys)
{
   int i;
   int Len;
   int PosX;

   if(!PalStateSaved++) SaveState(&PalState);
   SetColor(BLACK_COLOR);
   for(i = 0, PosX = 31;  i < 10; PosX += ((i&3)==3) ? 64 : 60, i++)  {
      SetRule(FORCE_RULE);
      Rectangle(PosX, KEYPOSY, PosX + KEYWIDTH,
                KEYPOSY+KEYDEPTH, SOLID_FILL);

      SetRule(XOR_RULE);
      SelectFont(SMALL_FONT);
      if(pKeys[i]) {
         Len = strlen(pKeys[i])*FNTW(SMALL_FONT);
         WriteText(PosX + (KEYWIDTH-Len)/2, KEYPOSY+1, pKeys[i]);
      }
   }
   if(!--PalStateSaved) RestoreState(&PalState);
}


