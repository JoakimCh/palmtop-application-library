/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  MEMOFILE.C
   Author:  Gilles Kohl
   Started: 1. Dec .95
   Subject: Return pathname of last file opened in MEMO
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

char *LastMemoFile(void)
{
   static char Name[70];
   FILE *f;

   f = fopen("c:\\_dat\\memoed.env", "rb");
   if(!f) return(NULL);
   fseek(f, 0x48, SEEK_SET);
   fread(Name, 63, 1, f);
   fclose(f);
   return Name;
}

