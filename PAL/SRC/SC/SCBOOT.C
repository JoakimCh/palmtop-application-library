/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  filename
   Author:  Bob Chernow
   Started: 11/24/1995
   Subject: Software Carousel Interface function
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
#include "palsc.h"

/* --------------------------------------------------------------------
                           functions
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
   SCBoot:
   Reboots the system. If type = 1 the user is not asked, if it is
   0 the user is asked first.
   -------------------------------------------------------------------- */
void SCBoot( int type )
{
    union REGS regs;
    struct SREGS segregs;

    regs.h.ah = 0xDF;
    regs.x.di = 0x534C;
    regs.x.dx = 0x534C;

    regs.h.al = 9;
    regs.h.bl = type;


    intdosx( &regs, &regs, &segregs);
}
