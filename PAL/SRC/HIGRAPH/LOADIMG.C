/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  LOADIMG.C
   Author:  Gilles Kohl
   Started: 04. Dec. 94
   Subject: Load a bitmap from file
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
   LoadImg:
   Load an Image file (in icon format), malloc memory for it and
   return a pointer to it. Returns NULL on failure
   -------------------------------------------------------------------- */

IMGHDR *LoadImg(char *Name)
{
   IMGHDR *pImg;
   FILE *f;
   long fs;

   if(!(f = fopen(Name, "rb"))) {
      return NULL;
   }

   fseek(f, 0L, SEEK_END);  /* seek to end of file */
   fs = ftell(f);           /* get lenght of  file */
   fseek(f, 0L, SEEK_SET);  /* seek to beginning again */

   /* now allocate storage for bitmap. Note that the area
      pointed to by pImg extends well beyond the actual IMGHDR
      structure */
   if(!(pImg = malloc(fs))) {
      fclose(f);
      return NULL;
   }

   /* Load the image */
   if(fread(pImg, fs, 1, f) != 1) {
      fclose(f);
      return NULL;
   }
   fclose(f);
   return pImg;
}


