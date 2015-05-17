/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  MNUSEL.C
   Author:  Gilles Kohl
   Started: 11. Jun 95
   Subject: Simple menu interface: select from a list of strings
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                            standard includes
   -------------------------------------------------------------------- */
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* --------------------------------------------------------------------
                           local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"

/* --------------------------------------------------------------------
                            constants
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                            globals
   -------------------------------------------------------------------- */

int MenuSelect(char *Items, int PosX, int PosY, int Default, int Show)
{
   MENU SelMnu;
   char *List = strdup(Items);
   int n, i;
   char *s;
   int retval;

   /* loop over list, terminating strings and counting them */
   for(n = 0, s = List; *s; ++s) {
      if(*s == '|') {
         *s = '\0';
         n++;
      }
   }
   ++n; /* add one more - last string doesn't have a | character */
   if(Show <= 0 || Show > n) Show = n;

   /* initialize menu */
   SelMnu.pMnuWin  = NULL;
   SelMnu.Flags    = 0;
   SelMnu.CurItem  = Default;
   SelMnu.ItmDisp  = Show;
   SelMnu.TopItem  = max(0, Default-Show+1);
   SelMnu.ItmCount = n;

   SelMnu.Items = malloc(n*sizeof(MENUITEM));

   /* initialize items */
   for(s = List, i = 0; i < n; i++, s += strlen(s)+1) {
      MENUITEM *pMnuItm = &SelMnu.Items[i];

      pMnuItm->Text      = s;
      pMnuItm->Flags     = MT_TXT|MA_VAL;
      pMnuItm->ActionPtr = MENUVAL(i);
   }
   retval = HandleMenu(&SelMnu, PosX, PosY);
   free(SelMnu.Items);
   free(List);
   return retval;
}

