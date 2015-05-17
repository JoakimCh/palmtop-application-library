/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  PROTECT.C
   Author:  Andreas Garzotto
   Started: Aug. 3 1995
   Subject: Enable / disable hardware RAM Protection.
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
    Unprotect: remove protection for hardware protected RAM area.
               Return old protection status.
   -------------------------------------------------------------------- */

WORD Unprotect(void)
{
   unsigned int i;

   AsmCLI();
   i = inportb(0x22);          /* save hornet index register */
   outportb(0x22, 0x87);       /* set index reg to write protect reg */
   i += (inportb(0x23) << 8);
   outportb(0x23, 0xa0);       /* set write protect to 640K */
   return i;
}


/* --------------------------------------------------------------------
    Protect: restore protection state previously altered with Unprotect
   -------------------------------------------------------------------- */

void Protect(unsigned int i)
{
   outport(0x22, (i & 0xff00) | 0x87);  /* restore previous write protect */
   outportb(0x22, i & 0xff);       /* restore previous index register value */
   AsmSTI();
}

