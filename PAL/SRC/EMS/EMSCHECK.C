/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  EMSCHECK.C
   Author:  Gilles Kohl
   Started: 07. Oct. 95
   Subject: Check for presence of EMS driver
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                            msdos includes
   -------------------------------------------------------------------- */
#include <dos.h>

/* --------------------------------------------------------------------
                           local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"

int EmsErr = 0;

/* --------------------------------------------------------------------
                           Functions
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
   EmsCheck:
   Check for the presence of an EMS driver - returns TRUE if found,
   FALSE otherwise.
   -------------------------------------------------------------------- */

int EmsCheck(void)
{
   static char DriverName[] = { 'E', 'M', 'M', 'X', 'X', 'X', 'X', '0' };

   union REGS   regs;
   struct SREGS sregs;
   char far *s;
   int i;

   /* retrieve pointer to driver header */
   regs.x.ax = 0x3567;

   intdosx(&regs, &regs, &sregs); /* getvect */
   s = (char far *)MK_FP(sregs.es, 10); /* build far pointer */

   /* now compare with what it should be */
   for(i = 0; i < sizeof(DriverName); i++) {
      if(s[i] != DriverName[i]) break;
   }
   return i == sizeof(DriverName);
}

