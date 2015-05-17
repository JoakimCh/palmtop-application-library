/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  SETAPPIC.C
   Author:  Gilles Kohl
   Started: 6. Nov. 95
   Subject: Set an icon slot to a given bitmap
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
   SetAppIcn

   Set an icon slot to a given bitmap
   -------------------------------------------------------------------- */
int SetAppIcn(APPDESC *pAd, int IcnNbr, IMGHDR *pImg)
{
   APPICN *pIcn = pImg;

   if(pAd == NULL) return FALSE;
   if(IcnNbr < 0 || IcnNbr >= pAd->IcnTotal) return FALSE;
   pAd->A.Icn[IcnNbr] = *pIcn;
   return TRUE;
}

