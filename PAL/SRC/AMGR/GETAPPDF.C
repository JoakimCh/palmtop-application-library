/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  GETAPPDEF.C
   Author:  Gilles Kohl
   Started: 3. Nov. 95
   Subject: Get an applications definition
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
   GetAppDef

   Get an applications definition
   -------------------------------------------------------------------- */
int GetAppDef(APPDESC *pAd, int AppNbr, char *pTitle, char *pPath,
              char *pComments, int *pFlags, WORD *pKey, int *pIcon)
{
   APPDEF *pDef;

   if(pAd == NULL || AppNbr < 0 || AppNbr >= pAd->A.Count) return FALSE;
   pDef = &pAd->A.App[AppNbr];

   if(pTitle)     strcpy(pTitle,    pDef->Title);
   if(pPath)      strcpy(pPath,     pDef->Path);
   if(pComments)  strcpy(pComments, pDef->Comments);
   if(pFlags)     *pFlags         = pDef->Flags;
   if(pKey)       *pKey           = pDef->HotKey;
   if(pIcon) {
      if(pDef->IcnNbr == APPMAXAPPS - pAd->IcnTotal) {
         *pIcon = -pDef->RAMIcnNbr-1;
      }
      else *pIcon = -pDef->IcnNbr-1; /* internal icon: as negative number */
   }
   return TRUE;
}

