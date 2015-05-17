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
   SCKeyboardCommands: Enable/Disable Work Area Switching.

   -------------------------------------------------------------------- */
int SCKeyboardCommands( int *cmd )
{
   union REGS regs;
   struct SREGS segregs;

   regs.h.ah = 0xDF;
   regs.x.di = 0x534C;
   regs.x.dx = 0x534C;

   regs.h.al = 8;
   regs.h.bl = *cmd;

   intdosx( &regs, &regs, &segregs);
   {
      int rc;
      if (regs.h.al == 1) {
         *cmd = regs.h.bl;
         rc = 0;
      }
      else rc = regs.h.ah;
      return(rc);
   }
}
