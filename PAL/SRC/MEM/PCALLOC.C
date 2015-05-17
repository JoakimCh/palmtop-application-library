/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  PCALLOC.C
   Author:  Gilles Kohl
   Started: 26. Nov 95
   Subject: Replacement for calloc function
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

void *PalCalloc(size_t n, size_t size)
{
   void *p;

   do p = (calloc)(n, size); while(!p && (pPalMemFail)(n*size));
   return p;
}

