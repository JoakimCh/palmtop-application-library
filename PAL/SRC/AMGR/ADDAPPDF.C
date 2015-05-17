/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  ADDAPPDEF.C
   Author:  Gilles Kohl
   Started: 3. Nov. 95
   Subject: Add an applications definition
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
   AddAppDef

   Add an applications definition
   -------------------------------------------------------------------- */
int AddAppDef(APPDESC *pAd, char *pTitle, char *pPath,
              char *pComments, int Flags, WORD Key, int Icon)
{
   if(pAd == NULL || pTitle == NULL) return FALSE;
   if(pPath == NULL) pPath = "";
   if(pComments == NULL) pComments = "";
   if(pAd->A.Count >= 38) return FALSE; /* no slots left */
   ++pAd->A.Count;
   return SetAppDef(pAd, pAd->A.Count-1, pTitle, pPath, pComments,
                    &Flags, &Key, &Icon);
}

