/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  SCPresnt.c
   Author:  Bob Chernow
   Started: 11/24/1995
   Subject: Software Carousel Presence Detection
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
   SCVersion: Returns SC Version Number

   -------------------------------------------------------------------- */
int SCVersion( void )
{
   union REGS regs;
   struct SREGS segregs;
   char buf[16];
   int h,l;

   regs.h.ah = 0xDF;
   regs.x.di = 0x534C;
   regs.x.dx = 0x534C;

   regs.h.al = 16;

   intdosx( &regs, &regs, &segregs);

   {
      int rc;
      if (regs.h.al == 1) {

         rc = atoi(itoa(regs.h.bl,buf,16)) +
             (100 * atoi(itoa(regs.h.bh,buf,16)));
      }
      else rc = 0;

      return(rc);
   }

}
/* --------------------------------------------------------------------
   SCPresent: Returns Version if SC is present.

   -------------------------------------------------------------------- */
int SCPresent( void )
{
   union REGS regs;
   struct SREGS segregs;

   regs.h.ah = 0xDF;
   regs.x.di = 0x534C;
   regs.x.dx = 0x534C;

   regs.h.al = 0x00;

   intdosx( &regs, &regs, &segregs);

   if (regs.x.ax == 0xFF) {
      return(SCVersion());
   }else {
      return(0);
   }

}

