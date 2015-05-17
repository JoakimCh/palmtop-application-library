/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  BATTIMER.C
   Author:  Andreas Garzotto
   Started: Aug. 27 1995
   Subject: Battery charging timer control routines
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                          standard includes
   -------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdio.h>

/* --------------------------------------------------------------------
                            msdos includes
   -------------------------------------------------------------------- */
#include <dos.h>

/* --------------------------------------------------------------------
                           local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"


/* --------------------------------------------------------------------
                           Functions
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
    BattGetTimer: read the battery charging timer and return the
                  timer value in seconds.
   -------------------------------------------------------------------- */

int BattGetTimer(void)
{
   return (int)((*((long far *)0x9000F848L) * 55L) / 1000L);
}

/* --------------------------------------------------------------------
    BattSetTimer: set the battery charging timer to the given
                  value (in seconds).
   -------------------------------------------------------------------- */

void BattSetTimer(int secs)
{
   int x;

   x = Unprotect();
   *((long far *)0x9000F848L) = ((long)secs * 1000L) / 55L;
   Protect(x);
}

