/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  LOADHFN.C
   Author:  Gilles Kohl/Harry Konstas
   Started: 7. Dec. 94
   Subject: Load a proportional font
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

/* --------------------------------------------------------------------
   LoadHfn:
   Loads a font in memory and returns pointer to the font or NULL
   -------------------------------------------------------------------- */

FONTDESC *LoadHfn(char *FileName)
{
   FONTDESC *pFont;
   FILE *FontFile;
   long FileSize;

   if(!(FontFile = fopen(FileName, "rb"))) return NULL;

   fseek(FontFile, 0L, SEEK_END);     /* seek to end of file */
   FileSize = ftell(FontFile);        /* get lenght of  file */
   fseek(FontFile, 0L, SEEK_SET);     /* seek to beginning again */

   /* allocate storage for font  */
   if(!(pFont = malloc(sizeof(FONTDESC) + FileSize))) {
      fclose(FontFile);
      return NULL;                    /* not enough memory   */
   }

   /* Load font file in memory */
   if(fread(&pFont->FontHeader, FileSize, 1, FontFile) != 1) {
      free(pFont);
      fclose(FontFile);
      return NULL;
   }
   fclose(FontFile);

   /* initialize font as discardable */
   InitHfn(pFont, TRUE, NULL);

   return pFont;            /* return pointer to the font */
}


