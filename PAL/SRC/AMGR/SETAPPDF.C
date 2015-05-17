/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  SETAPPDEF.C
   Author:  Gilles Kohl
   Started: 3. Nov. 95
   Subject: Set/modify an applications definition
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
   SetAppDef

   Set/modify an applications definition.
   -------------------------------------------------------------------- */
int SetAppDef(APPDESC *pAd, int AppNbr, char *pTitle, char *pPath,
              char *pComments, int *pFlags, WORD *pKey, int *pIcon)
{
   APPDEF *pDef;

   if(pAd == NULL || AppNbr < 0 || AppNbr >= pAd->A.Count) return FALSE;
   pDef = &pAd->A.App[AppNbr];

   if(pTitle)     strncpy(pDef->Title,     pTitle,    sizeof(pDef->Title));
   if(pPath)      strncpy(pDef->Path,      pPath,     sizeof(pDef->Path));
   if(pComments)  strncpy(pDef->Comments,  pComments, sizeof(pDef->Comments));
   if(pFlags)     pDef->Flags  = *pFlags;
   if(pKey)       pDef->HotKey = *pKey;
   if(pIcon) {
      if(*pIcon < 0) {
         pDef->IcnNbr = -*pIcon-1;
      }
      else {
         if(*pIcon < pAd->IcnTotal) {
            pDef->IcnNbr = APPMAXAPPS - pAd->IcnTotal;
            pDef->RAMIcnNbr = -*pIcon-1;
         }
      }
   }
   return TRUE;
}

