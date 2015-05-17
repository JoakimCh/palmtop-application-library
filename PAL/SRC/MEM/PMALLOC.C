/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  PMALLOC.C
   Author:  Gilles Kohl
   Started: 26. Nov 95
   Subject: Main module for alternative memory allocation routines
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
                             prototypes
   -------------------------------------------------------------------- */
static int PalMemFail(size_t size);

/* --------------------------------------------------------------------
                              globals
   -------------------------------------------------------------------- */
int (*pPalMemFail)(size_t size) = PalMemFail;

/* --------------------------------------------------------------------
                           Functions
   -------------------------------------------------------------------- */

static int PalMemFail(size_t size)
{
   FatalExit("PalMemFail: Out of memory !", 99);
   return FALSE;
}

void *SetPalMemFail(int (*NewFun)(size_t size))
{
   void *OldFun = pPalMemFail;
   pPalMemFail = NewFun;
   return OldFun;
}

void *PalMalloc(size_t size)
{
   void *p;

   do p = (malloc)(size); while(!p && (pPalMemFail)(size));
   return p;
}

void PalFree(void *p)
{
   if(p) (free)(p);
}

