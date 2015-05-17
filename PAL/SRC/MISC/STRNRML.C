/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  STRNRML.C
   Author:  Gilles Kohl
   Started: 24.2.95
   Subject: Normalize a string: copy suppressing ampersand character
            and trailing blanks, convert \023 back to ampersand
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                       standard includes
   -------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>

/* --------------------------------------------------------------------
                         local includes
   -------------------------------------------------------------------- */

#include "pal.h"
#include "palpriv.h"

/* --------------------------------------------------------------------
                           Functions
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
   StrNmrl
   Will copy the source string to the destination, skipping any
   ampersand characters, and suppressing trailing blanks . Useful in
   menus, combo items and later in GDB routines. Returns pointer to
   destination string.
   -------------------------------------------------------------------- */
char *StrNrml(char *Dst, char *Src)
{
   char *d = Dst;
   char *s = Src;
   char *p = NULL;

   while(*d = *s) {
      if(*d != '&') {
         if(*d == '\023') *d = '&';
         if(*d != ' ') p = d;
         ++d;
      }
      ++s;
   }
   if(p) p[1] = '\0';
   return Dst;
}

