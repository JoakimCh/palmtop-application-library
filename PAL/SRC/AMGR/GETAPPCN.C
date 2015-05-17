/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  GETAPPCN.C
   Author:  Gilles Kohl
   Started: 3. Nov. 95
   Subject: Get number of installed applications
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
   GetAppCnt

   Get number of installed applications
   -------------------------------------------------------------------- */
int GetAppCnt(APPDESC *pAd)
{
   return pAd->A.Count;
}

