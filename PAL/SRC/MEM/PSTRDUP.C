/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  PSTRDUP.C
   Author:  Gilles Kohl
   Started: 26. Nov 95
   Subject: Replacement for strdup function
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                            standard includes
   -------------------------------------------------------------------- */
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------
                           local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"

/* --------------------------------------------------------------------
                           Functions
   -------------------------------------------------------------------- */

char *PalStrdup(char *s)
{
   char *p;

   do p = (strdup)(s); while(!p && (pPalMemFail)(strlen(s)+1));

   return p;
}

