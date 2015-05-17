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
   SCSendCommand: Send Keystrokes to a Work Area

   -------------------------------------------------------------------- */
int SCSendCommand( int session, char * cmd )
{
   union REGS regs;
   struct SREGS segregs;

   regs.h.ah = 0xDF;
   regs.x.di = 0x534C;
   regs.x.dx = 0x534C;

   regs.h.al = 0x07;
   regs.h.bl = session;
   segregs.ds = FP_SEG(cmd);
   regs.x.si = FP_OFF(cmd);
   regs.x.cx = min(127,strlen(cmd));

   intdosx( &regs, &regs, &segregs);
   {
      int rc;
      if (regs.h.al == 1) rc = 0;
      else rc = regs.h.ah;

      return(rc);
   }
}

