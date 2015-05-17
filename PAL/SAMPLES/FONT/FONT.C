/* --------------------------------------------------------------------
   Project: PAL Font test program
   Module:  FONT.C
   Author:  Gilles Kohl
   Started: Dec 18, 1994
   Subject: Illustrates the use of the unified font functions.
            Also shows the use of resident fonts.
   -------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>

#include "pal.h"

#define MAX_FONTS 20

/* declare resident 'emergency' font image - will be linked in */
extern unsigned char ResFont[];

/* List a few fonts to be loaded */
char *FontNames[] = {
   "span0015.hfn",
   "spab0015.hfn",
   "fpan0015.hfn",
   "fpab0015.hfn",
   NULL
};


void main(void)
{
   int i;
   int ResId;
   int CurId;
   int PosY = 10;

   if(!PalInit(1)) FatalExit("Init failed - CGAGRAPH not loaded ?", 1);

   /* first 'load' resident emergeny font */
   ResId = LoadFont(ResFont, 1);
   if(ResId == 0) FatalExit("Load emergency font failed - panic !", 1);

   /* now try to get more */
   for(i = 0; FontNames[i] != NULL; i++) {
      char buf[256];

      CurId = LoadFont(FontNames[i], 0);
      if(CurId == 0) {
         sprintf(buf, "Load of font %s failed !", FontNames[i]);
         TextOut(10, PosY, TXT_RULE, ResId, buf);
         PosY += FontHeight(ResId);
      }
      else {
         sprintf(buf, "This is font %s !", FontNames[i]);
         TextOut(10, PosY, TXT_RULE, CurId, buf);
         PosY += FontHeight(CurId);
         DiscardFont(CurId); /* Throw away this font */
      }
   }
   GetKey();
   PalDeInit(1);
}

