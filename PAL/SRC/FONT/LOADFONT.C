/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  LOADFONT.C
   Author:  Gilles Kohl
   Started: 17. Dec. 94
   Subject: Unified font support: Load a proportional font, return id
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
                           global variables
   -------------------------------------------------------------------- */
FONTDESC *PalFontTab[MAXFONTENTRIES];

/* --------------------------------------------------------------------
                              Functions
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
   LoadFont:
   Loads a font in memory and returns 'handle'.
   A handle of zero indicates an error. (Either file not found or
   no slots available.)

   -------------------------------------------------------------------- */

int LoadFont(void *Font, int Resident)
{
   static int FirstCall = 1;
   FONTDESC *pFont;
   int Slot;

   if(FirstCall) { /* we have to initialize ! */
      FirstCall = 0;

      /* empty table */
      for(Slot = 0; Slot < MAXFONTENTRIES; Slot++) PalFontTab[Slot] = NULL;

      /* make font functions point to extended versions */
      TextOut    = TextOut1;
      TextExt    = TextExt1;
      FontHeight = FontHeight1;
   }

   /* First, find an empty slot */
   for(Slot = 0; Slot < MAXFONTENTRIES; Slot++) {
      if(PalFontTab[Slot] == NULL) break;
   }
   if(Slot >= MAXFONTENTRIES) return 0; /* no more slots available */

   /* got the slot, load the font. If we have a resident font, 'Font' is
      the pointer to the binary file image - it needs a FONTDESC structure
      and initializing. In this case, we allocate a separate FONTDESC,
      and have the character pointers point into the actual bitmaps.
      The file header is copied over. If we have a regular file
      load, load it - LoadHfn will do the loading, initializing,
      and mark it as discardable.
    */

   if(Resident) {
      if(pFont = malloc(sizeof(FONTDESC))) { /* alloc OK ? */
         memcpy(&pFont->FontHeader, Font, sizeof(FNTFHDR));
         InitHfn(pFont, 1, (char *)Font+sizeof(FNTFHDR)-4);
      }
   }
   else pFont = LoadHfn(Font);

   if(!pFont) return 0; /* loading failed */

   /* store pointer to font in table */
   PalFontTab[Slot] = pFont;

   /* return 'handle' to the font */
   return -(Slot+1);
}


