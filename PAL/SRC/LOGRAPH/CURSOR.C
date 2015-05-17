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
   MoveCursor:
   Moves graphics cursor to the specified X:Y location of the screen
   -------------------------------------------------------------------- */

void MoveCursor(int x1, int y1)
{
    union REGS regs;
    struct SREGS segregs;

    regs.x.cx = x1;
    regs.x.dx = y1;
    regs.x.ax = 0xdc03;
    int86x(0x10, &regs, &regs, &segregs);
}


/* --------------------------------------------------------------------
   Cursor:
   Cursor control function: ( CURSOR_ON, CURSOR_OFF, CURSOR_BLINK,
                              CURSOR_NOBLINK, CURSOR_INIT )
   -------------------------------------------------------------------- */

void Cursor(WORD Function)
{

   /* Default Cursor definition table (used by CURSOR_INIT) */

   static char default_cursor[]={
    0,0,
    0x80,0x0f, 0x80,0x0f, 0x80,0x0f, 0x80,0x0f,
    0x80,0x0f, 0x80,0x0f, 0x80,0x0f, 0x80,0x0f,
    0x80,0x0f, 0x80,0x0f, 0x80,0x0f, 0xff,0xff,
    0xff,0xff, 0xff,0xff, 0xff,0xff, 0xff,0xff,
    0xff,0xff, 0xff,0xff, 0xff,0xff, 0xff,0xff,
    0xff,0xff, 0xff,0xff, 0xff,0xff, 0xff,0xff,
    0xff,0xff, 0xff,0xff, 0xff,0xff, 0xff,0xff,
    0xff,0xff, 0xff,0xff, 0xff,0xff, 0xff,0xff,

    0x7f,0xf0, 0x7f,0xf0, 0x7f,0xf0, 0x7f,0xf0,
    0x7f,0xf0, 0x7f,0xf0, 0x7f,0xf0, 0x7f,0xf0,
    0x7f,0xf0, 0x7f,0xf0, 0x7f,0xf0, 0x00,0x00,
    0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,
    0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,
    0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,
    0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,
    0x00,0x00, 0x00,0x00, 0x00,0x00, 0x00,0x00,

   };

   union REGS regs;
   struct SREGS segregs;

   if(Function == CURSOR_INIT) {   /* CURSOR_INIT defines default cursor */
      regs.x.si = FP_OFF(default_cursor);
      segregs.es= FP_SEG(default_cursor);
      segregs.ds= FP_SEG(default_cursor);
      BlinkRate(9);
   }

   regs.x.ax = Function;
   int86x(0x10, &regs, &regs, &segregs);
}


