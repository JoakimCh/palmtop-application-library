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
   SCCodeMode:  Determine where SC code is operating.

		mode bit values:
		  bit 0 on = Carousel is running in Extended Memory
		  bit 1 on = Carousel is running in Expanded Memory
		  bit 2 on = Carousel is storing code in Expanded Mem
		  bit 3 on = Carousel is storing code in Extended Mem
		  bit 4 on = Carousel is storing code on disk.

   -------------------------------------------------------------------- */
int SCCodeMode( int *mode )
{
   union REGS regs;
   struct SREGS segregs;

   regs.h.ah = 0xDF;
   regs.x.di = 0x534C;
   regs.x.dx = 0x534C;

   regs.h.al = 16;

   intdosx( &regs, &regs, &segregs);
   {
      int rc;
      if (regs.h.al == 1) {
         rc = 0;
         *mode = regs.x.cx;
      }
      else rc = regs.h.ah;
      return(rc);
   }
}
