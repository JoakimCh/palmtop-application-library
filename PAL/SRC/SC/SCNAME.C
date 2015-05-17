/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  filename
   Author:  Bob Chernow
   Started: 11/24/1995
   Subject: Software Carousel Name Gat/Set functions
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
   SCGetName: Gets the name into name of session.

   -------------------------------------------------------------------- */
int SCGetName( int session, char * name )
{
   union REGS regs;
   struct SREGS segregs;

   regs.h.ah = 0xDF;
   regs.x.di = 0x534C;
   regs.x.dx = 0x534C;

   regs.h.al = 0x04;
   regs.h.bl = session;


   intdosx( &regs, &regs, &segregs);
   {
      int rc;
      if (regs.h.al == 1) {
         if (regs.x.cx) {
            char far *aux = (char far *)name; /* Power C workaround */
            movedata(segregs.es,regs.x.bx,
                     FP_SEG(aux),FP_OFF(aux),
                     regs.x.cx);
            name[regs.x.cx] = '\0';
         } else {
            sprintf(name, "Work Area Number %d",session);
         }
         rc = 0;
      }
      else rc = regs.h.ah;
      return(rc);
   }
}

/* --------------------------------------------------------------------
   SCSetName: Sets the name of session.

   -------------------------------------------------------------------- */
int SCSetName( int session, char * name )
{
   union REGS regs;
   struct SREGS segregs;

   regs.h.ah = 0xDF;
   regs.x.di = 0x534C;
   regs.x.dx = 0x534C;

   regs.h.al = 0x06;
   regs.h.bl = session;
   segregs.ds = FP_SEG(name);
   regs.x.si = FP_OFF(name);
   regs.x.cx = min(24,strlen(name));

   intdosx( &regs, &regs, &segregs);
   {
      int rc;
      if (regs.h.al == 1) rc = 0;
      else rc = regs.h.ah;

      return(rc);
   }
}

