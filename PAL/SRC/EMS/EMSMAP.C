/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  EMSMAP.C
   Author:  Gilles Kohl
   Started: 07. Oct. 95
   Subject: Map a given logical page into the physical frame
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
#include "pal.h"
#include "palpriv.h"

/* --------------------------------------------------------------------
                           Functions
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
   EmsMap:
   Map a given page into the frame
   -------------------------------------------------------------------- */
void far *EmsMap(int Handle, int LogPage, int PhysPage)
{
   union REGS regs;

   regs.h.ah = 0x44;
   regs.h.al = PhysPage;
   regs.x.bx = LogPage;
   regs.x.dx = Handle;
   int86(EMS_INT, &regs, &regs);
   return (EmsErr = regs.h.ah) ? NULL :
          (void far *)((char far *)pEmsBase+16384L*PhysPage);
}

