/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  scsize.c
   Author:  Bob Chernow
   Started: 11/24/1995
   Subject: Software Carousel Session Size and Status
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
   SCSizeStatus:
   Take session number (number) and pointers to two ints (size) and
   (status) and fills in the ints with the session size and status.
   status = 1 if a program is running, 0 if not.

   -------------------------------------------------------------------- */
int  SCSizeStatus( int number, int* size, int* status )
{
   union REGS regs;
   struct SREGS segregs;

   regs.h.ah = 0xDF;
   regs.x.di = 0x534C;
   regs.x.dx = 0x534C;

   regs.h.al = 0x03;
   regs.h.bl = number;

   intdosx( &regs, &regs, &segregs);
   {
      int rc;
      if (regs.h.al == 1) {
         *size = regs.x.dx;
         *status = regs.h.bl;
         rc = 0;
      } else rc = regs.h.ah;

      return(rc);
   }
}

int  SCSetSize( int number, int size )
{
   union REGS regs;
   struct SREGS segregs;

   regs.h.ah = 0xDF;
   regs.x.di = 0x534C;
   regs.x.dx = 0x534C;

   regs.h.al = 0x05;
   regs.h.bl = number;
   regs.x.dx = size;

   intdosx( &regs, &regs, &segregs);
   {
      int rc;
      if (regs.h.al == 1) {
         rc = 0;
      } else rc = regs.h.ah;

      return(rc);
   }
}
