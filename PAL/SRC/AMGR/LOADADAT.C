/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  LOADADAT.C
   Author:  Gilles Kohl
   Started: 3. Nov. 95
   Subject: Load APPMGR.DAT into memory, return pointer to it.
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
   LoadAppDat
   Load APPMGR.DAT into memory, return pointer to it.
   -------------------------------------------------------------------- */
APPDESC *LoadAppDat(char *FileName)
{
   FILE *f;
   APPDESC *pAd;
   int IcnTotal;
   int FileSize;

   pAd = malloc(sizeof(*pAd));
   if(!pAd) return NULL;

   if(!FileName) FileName = APPMGRFILE;
   if(!(f = fopen(FileName, "rb"))) {
      free(pAd);
      return NULL;
   }
   FileSize = fread(&pAd->A, 1, sizeof(*pAd), f);
   fclose(f);

   IcnTotal = (FileSize-APPDEFSIZE)/200;
   if(APPDEFSIZE+200*IcnTotal != FileSize) {
      free(pAd);
      return NULL;
   }
   pAd->IcnTotal = IcnTotal;
   return pAd;
}

