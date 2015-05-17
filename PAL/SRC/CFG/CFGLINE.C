/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  CFGLINE.C
   Author:  GK
   Started: 12/03/94
   Subject: Get line number of last entry got via GetFirst/NextEntry
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

/* ------------------------------ GetCfgLine ------------------------------ */
/* Return line number of last entry got via GetFirst/NextEntry              */

int GetCfgLine(void)
{
   if(TmpEntry == NULL) return 0;
   return TmpEntry->LineNbr;
}

