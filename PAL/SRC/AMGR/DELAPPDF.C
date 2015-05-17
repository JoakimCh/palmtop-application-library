/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  DELAPPDEF.C
   Author:  Gilles Kohl
   Started: 5. Nov. 95
   Subject: Delete application definition.
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
   DelAppDef

   Delete an applications definition.
   -------------------------------------------------------------------- */
int DelAppDef(APPDESC *pAd, int AppNbr)
{
   int i;

   if(pAd == NULL || AppNbr < 0 || AppNbr >= pAd->A.Count) return FALSE;

   for(i = AppNbr; i < pAd->A.Count-1; i++) {
      pAd->A.App[i] = pAd->A.App[i+1];
   }
   --pAd->A.Count;
   return TRUE;
}

