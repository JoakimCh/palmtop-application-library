/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  READMYCF.C
   Author:  GK
   Started: 12/03/94
   Subject: Read config given programs location and name (argv[0])
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

/* ----------------------------- ReadMyConfig ----------------------------- */
/* Read config given programs location and name (usually argv[0])           */

void ReadMyConfig(char *MyName)
{
   char DefPath[64];
   char *s;

   strcpy(DefPath, MyName);
   if(s = strrchr(DefPath, '.')) *s = '\0';
   strcat(DefPath, ".CFG");
   ReadConfig(DefPath);
}


