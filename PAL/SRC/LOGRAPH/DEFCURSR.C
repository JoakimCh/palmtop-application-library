/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  CURSOR.C                           TESTED on TC 2.0 OK
   Author:  Harry Konstas
   Started: 11/20/1994
   Subject: Graphics cursor control functions
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
                           functions
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
   DefineCursor:
   Defines the shape of the graphics cursor. The *CursorDef pointer
   points to a 130-byte definition table.
   -------------------------------------------------------------------- */
void DefineCursor(char far *CursorDef)
{
    union REGS regs;
    struct SREGS segregs;

    regs.x.si = FP_OFF(CursorDef);
    segregs.es= FP_SEG(CursorDef);
    segregs.ds= FP_SEG(CursorDef);
    regs.x.ax = 0xdc00;
    int86x(0x10, &regs, &regs, &segregs);
}
