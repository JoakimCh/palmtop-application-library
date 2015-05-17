/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  CLSEAMGR.C
   Author:  Gilles Kohl
   Started: 3. Nov. 95
   Subject: Close application manager
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                          standard includes
   -------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdio.h>

/* --------------------------------------------------------------------
                           local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"



/* --------------------------------------------------------------------
   CloseAppMgr
   Try to close application manager if it is running
   -------------------------------------------------------------------- */

void CloseAppMgr(void)
{
   /* if not running AppMgr, nothing needs to be done */
   if(!IsAppActive(APPMGR_KEY)) return;

   /* otherwise, switch to AppMgr and close it by pushing the
      appropriate key */
   CallInt(0x16, 0x0500, 0, APPMGR_KEY, 0, NULL); /* switch to AMGR */
   CallInt(0x16, 0x0500, 0, 0x011b, 0, NULL);     /* Push ESC to leave it */
   /* Push a second ESC to work around an HP100LX glitch */
   CallInt(0x16, 0x0500, 0, 0x011b, 0, NULL);
   if(KeyWaiting()) GetKey(); /* eat up second ESC */
}

