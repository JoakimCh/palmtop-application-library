/* --------------------------------------------------------------------
   Project: HP200LX FILER PROTOCOL (CLIENT) COMMUNICATIONS FOR PAL
   Module:  MKDFILER.C
   Author:  Harry Konstas
   Started: 17. Oct. 95
   Subject: Create directory on Server
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
                            Get file from server
   -------------------------------------------------------------------- */

int  FilerMakeDir(FILERCOM *pFiler, char *RemoteDir)
{

   /* send directory name */
   if(FilerRequest(pFiler, MAKE_DIR, strlen(RemoteDir),
                 RemoteDir) == NO_RESPONSE) {
      return CANNOT_CREATE_DIR;
   }

   return DIR_CREATED;

}

