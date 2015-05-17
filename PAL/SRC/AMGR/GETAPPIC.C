/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  GETAPPIC.C
   Author:  Gilles Kohl
   Started: 6. Nov. 95
   Subject: Get (pointer to) an icons representation
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
   GetAppIcn

   Get (pointer to) an icons representation
   -------------------------------------------------------------------- */
IMGHDR *GetAppIcn(APPDESC *pAd, int IcnNbr, IMGHDR *(*Aux)(int n, int m))
{
   IMGHDR *pImg;

   if(pAd == NULL) return NULL;
   if(IcnNbr < 0) pImg = Aux ? Aux(-IcnNbr-1, WhichHp()) : NULL;
   else {
      if(IcnNbr >= pAd->IcnTotal) return NULL;
      pImg = (IMGHDR *)&pAd->A.Icn[IcnNbr];
   }
   return ValidIcon(pImg) ? pImg : NULL;
}

