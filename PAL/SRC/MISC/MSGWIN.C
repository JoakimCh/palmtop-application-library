/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  MSGWIN.C
   Author:  Gilles Kohl
   Started: 21. Dec. 95
   Subject: Display a non-modal message window
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                            standard includes
   -------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* --------------------------------------------------------------------
                            constants
   -------------------------------------------------------------------- */
#define TTLDIST 4
#define LINDIST 4
#define XOFF    4
#define YOFF    4

/* --------------------------------------------------------------------
                           local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"

/* --------------------------------------------------------------------
                             Functions
   -------------------------------------------------------------------- */

void MsgWin(char *Title, char *Fmt, ...)
{
   va_list ArgPtr;        /* used for variable paramater list   */
   char *LinPtr[10];      /* array of pointers to line starts   */
   int LinCnt;            /* number of text lines */
   int MaxLin = 0;        /* width of largest line */
   int i, x, y, w, d;     /* auxiliary variables */
   int fh = FontHeight(MEDIUM_FONT);
   static char *Msg = NULL;
   static char *Win = NULL;

   va_start(ArgPtr, Fmt);

   if(Msg) free(Msg); Msg = NULL;
   if(Win) CloseWin(Win); Win = NULL;
   if(!Fmt) return;

   /* duplicate/allocate strings since StrSplit modifies them */
   if(!(Msg  = malloc(1024))) return NULL;

   vsprintf(Msg, Fmt, ArgPtr);

   /* Insert terminators, compute number of lines and buttons */
   LinCnt = StrSplit(LinPtr, Msg,  ELMS(LinPtr));

   /* find largest line */
   for(i = 0; i < LinCnt; i++) {
      int n = TextExt(MEDIUM_FONT, LinPtr[i]);
      if(n > MaxLin) MaxLin = n;
   }

   /* Compute depth of window */
   d =   LinCnt * (fh+LINDIST);
   if(Title)  d += fh+TTLDIST;
   d += 2*YOFF;

   /* compute width of window */
   w = max(MaxLin+4, Title ? TextExt(MEDIUM_FONT, Title) : 0) + 2*XOFF;

   /* Center window */
   x = (MAX_X - w)/2;
   y = (MAX_Y - d)/2;

   Win = OpenWin(WS_HP200, x, y, x+w, y+d, Title);

   /* Display strings */
   for(i = 0; i < LinCnt; i++) {
      WinText(Win, XOFF, YOFF+i*(fh+LINDIST), LinPtr[i]);
   }
}

