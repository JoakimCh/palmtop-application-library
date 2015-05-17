/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  PREALLOC.C
   Author:  Gilles Kohl
   Started: 26. Nov 95
   Subject: Replacement for realloc function
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                            standard includes
   -------------------------------------------------------------------- */
#include <stdlib.h>

/* --------------------------------------------------------------------
                           local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"

/* --------------------------------------------------------------------
                           Functions
   -------------------------------------------------------------------- */

void *PalRealloc(void *p, size_t size)
{
   void *q;

   do q = (realloc)(p, size); while(!q && (pPalMemFail)(size));
   return q;
}


