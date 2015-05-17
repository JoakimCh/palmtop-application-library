/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  LOADADAT.C
   Author:  Gilles Kohl
   Started: 3. Nov. 95
   Subject: Save APPMGR.DAT to file, given pointer to it.
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
   SaveAppDat
   Save APPMGR.DAT to file, given pointer to it.
   -------------------------------------------------------------------- */

int SaveAppDat(APPDESC *pAd, char *FileName)
{
   FILE *f;
   int FileSize;

   if(!FileName) FileName = APPMGRFILE;
   FileSize = pAd->IcnTotal*200 + APPDEFSIZE;

   if(!(f = fopen(FileName, "wb"))) return FALSE;
   if(fwrite(&pAd->A, 1, FileSize, f) != FileSize) {
      return FALSE;
   }
   fclose(f);
   return TRUE;
}

