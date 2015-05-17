/* --------------------------------------------------------------------
   Project: HP200LX FILER PROTOCOL (CLIENT) COMMUNICATIONS FOR PAL
   Module:  SYNFILER.C
   Author:  Harry Konstas
   Started: 17. Oct. 95
   Subject: Re-synchronize with filer.
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
                          Synchronize with filer
   -------------------------------------------------------------------- */

int  FilerSync(FILERCOM *pFiler)
{

   /* re-synchronize  */
   SendPacket(pFiler, CONNECT_SERVER, 0,0, NULL);
   GetPacket(pFiler);
   SendPacket(pFiler, CONNECT_SERVER, 0,0, NULL);
   GetPacket(pFiler);
   FilerRequest(pFiler, CONNECT_SERVER, 0, NULL);

   return SERVER_ACK;
}

