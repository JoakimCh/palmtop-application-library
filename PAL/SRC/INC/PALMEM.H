/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  PALMEM.H
   Author:  Gilles Kohl
   Started: 26. Nov. 95
   Subject: PAL memory handling replacement macros
   -------------------------------------------------------------------- */

#ifndef _PALMEM_H
#define _PALMEM_H

#define malloc(s)    PalMalloc(s)
#define calloc(n, s) PalCalloc((n),(s))
#define free(p)      PalFree(p)
#define strdup(s)    PalStrdup(s)
#define realloc(p,s) PalRealloc((p),(s))

#endif

