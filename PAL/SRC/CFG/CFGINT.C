/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  CFGINT.C
   Author:  GK
   Started: 12/03/94
   Subject: GetConfigInt: Get an integer value from config file
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                       standard includes
   -------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* --------------------------------------------------------------------
                         local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"

/* --------------------------------------------------------------------
                           functions
   -------------------------------------------------------------------- */

/* ----------------------------- GetConfigInt ----------------------------- */
/* Return int value of a config entry - default value if not found          */

int GetConfigInt(char *Section, char *Key, int Default)
{
   char *s;
   char *p;
   long val;

   s = GetConfigString(Section, Key, "NAN");
   val = strtol(s, &p, 0);
   if(strchr(" \t;", *p)) return val;
   return Default;
}


