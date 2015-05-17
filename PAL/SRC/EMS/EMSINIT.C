/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  EMSINIT.C
   Author:  Gilles Kohl
   Started: 10. Oct. 95
   Subject: Check for presence of EMS driver, retrieve base segment
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                          standard includes
   -------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>

/* --------------------------------------------------------------------
                            msdos includes
   -------------------------------------------------------------------- */
#include <dos.h>

/* --------------------------------------------------------------------
                           local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"

void far *pEmsBase = NULL;

/* --------------------------------------------------------------------
                           Functions
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
   EmsInit:
   Check for the presence of an EMS driver - if present, returns far
   pointer to EMS frame, and also stores it. Returns NULL on failure
   -------------------------------------------------------------------- */

void far *EmsInit(void)
{
   WORD w;

   EmsErr = 0;
   if(!EmsCheck()) return NULL;
   if((w = EmsFrameSeg()) == EMS_ERR) return NULL;
   return pEmsBase = MK_FP(w, 0);
}

