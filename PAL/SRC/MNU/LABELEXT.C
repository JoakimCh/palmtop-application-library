/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  LABELEXT.C
   Author:  Gilles Kohl
   Started: 17.06.95
   Subject: Compute extension of label string - suppressing '&'
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
   LabelExt:
   Will return the width of a menu or dialog label given a string
   and a font.
   (Internal utility, not exported)
   -------------------------------------------------------------------- */
int LabelExt(int FontId, char *Lbl)
{
   int w;

   w = TextExt(FontId, Lbl);
   if(strchr(Lbl, '&')) w -= TextExt(FontId, "&");
   return w;
}

