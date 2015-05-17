/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  STRSPLIT.C
   Author:  Gilles Kohl
   Started: 21. Dec .95
   Subject: Utility function: split a '|'-separated string
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
                             functions
   -------------------------------------------------------------------- */
int StrSplit(char **pPtr, char *Txt, int Max)
{
   char *s;
   int i;

   if(Txt == NULL || strlen(Txt) == 0) return 0;

   i = 0;
   s = Txt;
   do {
      if(i < Max) pPtr[i++] = s;
      s = strchr(s, '|');
      if(s) *s++ = '\0';
   } while(s);

   return i;
}

