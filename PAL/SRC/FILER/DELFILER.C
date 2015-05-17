/* --------------------------------------------------------------------
   Project: HP200LX FILER PROTOCOL (CLIENT) COMMUNICATIONS FOR PAL
   Module:  DELFILER.C
   Author:  Harry Konstas
   Started: 17. Oct. 95
   Subject: Delete a file in Server.
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                             includes
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
                          Delete Server file
   -------------------------------------------------------------------- */

int  FilerDelFile(FILERCOM *pFiler, char *RemoteFile)
{

   /* delete file */
   if(FilerRequest(pFiler, DEL_FILE, strlen(RemoteFile),
                 RemoteFile) == NO_RESPONSE) {
      return CANNOT_DELETE;
   }

   return FILE_DELETED;
}

