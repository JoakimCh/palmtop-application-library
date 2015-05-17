/* --------------------------------------------------------------------
   Project: HP200LX FILER PROTOCOL (CLIENT) COMMUNICATIONS FOR PAL
   Module:  ASDFILER.C
   Author:  Harry Konstas
   Started: 17. Oct. 95
   Subject: Request remote directory from Server
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
                Request Remote directory from server
   -------------------------------------------------------------------- */

int  FilerAskDir(FILERCOM *pFiler, char *RemoteDir)
{

   /* send directory name */
   if(FilerRequest(pFiler, ASK_DIR, strlen(RemoteDir),
                 RemoteDir) == NO_RESPONSE) {
      return NO_RESPONSE;
   }

   return SERVER_ACK;

}

