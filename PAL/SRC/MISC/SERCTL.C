/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  SERCTL.C
   Author:  Gilles Kohl
   Started: 27. Dec. 95
   Subject: Toggle SysMgr serial port interference, but stay switchable
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                            standard includes
   -------------------------------------------------------------------- */
#include <stdlib.h>
#include <string.h>


/* --------------------------------------------------------------------
                           local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"

/* --------------------------------------------------------------------
                             functions
   -------------------------------------------------------------------- */

typedef BYTE far *FARPTR;

int SerialCtl(int State)
{
   FARPTR p1c = *((FARPTR far *)0x70L);
   FARPTR pFlag;
   WORD Offset;
   WORD CSeg;

   if(!CheckSysMgr()) return TRUE;

   if(p1c[0] != 0x9c || p1c[0x17] != 0x83 || p1c[0x18] != 0x3e) return FALSE;
   Offset = *((WORD far *)(p1c+0x19));
   CSeg = FP_SEG(p1c);

   pFlag = MK_FP(CSeg, Offset);
   *pFlag = !!State;

   return TRUE;
}


