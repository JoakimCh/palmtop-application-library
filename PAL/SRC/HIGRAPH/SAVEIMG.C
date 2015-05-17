/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  SAVEIMG.C
   Author:  Gilles Kohl
   Started: 04. Dec. 94
   Subject: Save a bitmap to file
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
   SaveImg:
   Save an Image to file (in icon format)
   Returns TRUE if successful
   -------------------------------------------------------------------- */

int SaveImg(IMGHDR *pImg, char *Name)
{
   FILE *f;
   long fs;

   if(!(f = fopen(Name, "wb"))) return FALSE;

   fs = IMGSIZE(0, 0, pImg->Width-1, pImg->Depth-1);

   /* Save the image */
   if(fwrite(pImg, fs, 1, f) != 1) {
      fclose(f);
      return FALSE;
   }
   fclose(f);
   return TRUE;
}


