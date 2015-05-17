/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  MSDELAY.C
   Author:  Gilles Kohl
   Started: 14.4.95
   Subject: Wait a specified number of milliseconds
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

/* --------------------------------------------------------------------
   MsDelay:
   Wait a specified number of milliseconds
   -------------------------------------------------------------------- */
void MsDelay(unsigned int Millisec)
{
   WORD  far *volatile ClockLoc  = (WORD far *volatile)0x0040006cL;
   DWORD limit;
   DWORD start;
   DWORD current;

   start = *ClockLoc;
   limit = (DWORD)start + Millisec/55;
   do {
      current = *ClockLoc;
      if(current < start) current += 65536L;
   } while(current < limit);
}

