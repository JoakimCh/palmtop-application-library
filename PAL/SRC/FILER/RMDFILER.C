/* --------------------------------------------------------------------
   Project: HP200LX FILER PROTOCOL (CLIENT) COMMUNICATIONS FOR PAL
   Module:  RMDFILER.C
   Author:  Harry Konstas
   Started: 17. Oct. 95
   Subject: Delete directory on Server
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

int  FilerDelDir(FILERCOM *pFiler, char *RemoteDir)
{

   /* send directory name */
   if(FilerRequest(pFiler, DEL_DIR, strlen(RemoteDir),
                 RemoteDir) == NO_RESPONSE) {
      return CANNOT_DELETE_DIR;
   }

   return DIR_DELETED;

}

