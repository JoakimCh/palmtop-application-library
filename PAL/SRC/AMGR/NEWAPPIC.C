/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  NEWAPPIC.C
   Author:  Gilles Kohl
   Started: 3. Nov. 95
   Subject: Return an available icon slot not used by an application
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
   NewAppIcn

   Return an available icon slot not used by an application.
   Will return -1 if no slot found
   -------------------------------------------------------------------- */
int NewAppIcn(APPDESC *pAd)
{
   char *pTab;
   int i, n;

   if(!pAd) return -1;
   pTab = calloc(pAd->IcnTotal, 1);
   for(i = 0; i < pAd->A.Count; i++) {
      GetAppDef(pAd, i, NULL, NULL, NULL, NULL, NULL, &n);
      if(n >= 0 && n < pAd->IcnTotal) ++pTab[n];
   }
   for(i = 0; i < pAd->IcnTotal; i++) if(pTab[i] == 0) break;
   if(i >= pAd->IcnTotal) i = (WhichHp() == 100) ? -16 : -18;
   free(pTab);
   return i;
}
