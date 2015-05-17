/* --------------------------------------------------------------------
   Project: PAL utility
   Module:  PCX2IMG
   Author:  Gilles Kohl
   Started: January 7, 1995
   Subject: Convert a PCX file into HP100/200LX IMG format
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                       standard includes
   -------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------
                         local includes
   -------------------------------------------------------------------- */
#include "pal.h"

/* --------------------------------------------------------------------
                           functions
   -------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
   IMGHDR *pImg;
   char Target[70];
   char *s;

   if(argc != 2) FatalExit("Usage: PCX2IMG <FILE.PCX>", 1);
   if(!PalInit(1)) FatalExit("Init fail - is CGAGRAPH resident ?", 1);

   /* Try to load PCX file, exit on failure */
   pImg = LoadPcx(argv[1], 0);
   if(!pImg) FatalExit("Load PCX file failed", 1);

   /* now build the name of the target image file */
   strcpy(Target, argv[1]);
   s = strrchr(Target, '.'); /* find the last '.' */
   if(!s) s = Target+strlen(Target); /* if none, point to end */
   strcpy(s, ".img"); /* either replace or append extension */

   /* Display the image */
   PutImg(0, 0, FORCE_RULE, pImg);

   /* and save it. PAL does all the work. */
   SaveImg(pImg, Target);

   GetKey();

   /* That's it, PAL */
   PalDeInit(1);
}

