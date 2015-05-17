/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  VALIDICN.C
   Author:  Gilles Kohl
   Started: 6. Nov. 95
   Subject: Check if an IMGHDR * points to a valid icon
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
   ValidIcon

   Check if an IMGHDR * points to a valid icon
   -------------------------------------------------------------------- */
int ValidIcon(IMGHDR *pIcn)
{
   return pIcn->Planes == 1  &&
          pIcn->Bits   == 1  &&
          pIcn->Width  == 44 &&
          pIcn->Depth  == 32;
}

