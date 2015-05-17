/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  LABELOUT.C
   Author:  Gilles Kohl
   Started: 17.06.95
   Subject: Output label string - same as TextOut, with provision for
            special '&' handling
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                          standard includes
   -------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------
                           local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"

/* --------------------------------------------------------------------
                           Functions
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
   LabelOut:
   Like TextOut, but handles underlined label strings.
   (Internal utility, not exported)
   -------------------------------------------------------------------- */
void LabelOut(int x, int y, int Style, int FontId, char *Lbl)
{
   int Pos = -1;
   int lx;
   int y2;
   char buf[80];
   char *s, *p;

   if(strlen(Lbl) > sizeof(buf)-1) return;
   y2 = y+FontHeight(FontId);
   strcpy(buf, Lbl);
   s = strchr(buf, '&');
   for(p = buf; *p; p++) if(*p == '\023') *p = '&';
   if(s) {
      Pos = s - buf;
      *s = '\0'; lx = TextExt(FontId, buf);
      while(*s = *(s+1)) ++s;
   }
   TextOut(x, y, Style, FontId, buf);
   if(Pos >= 0) {
      Line(x+lx, y2, x+lx+TextExt(FontId, "&")-1, y2);
   }
}

