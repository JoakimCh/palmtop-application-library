/* --------------------------------------------------------------------
   Project: HP200LX FILER PROTOCOL (CLIENT) COMMUNICATIONS FOR PAL
   Module:  GETFILER.C
   Author:  Harry Konstas
   Started: 17. Oct. 95
   Subject: Get file from Server
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                             includes
   -------------------------------------------------------------------- */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* --------------------------------------------------------------------
                         local includes
   -------------------------------------------------------------------- */

#include "pal.h"
#include "palpriv.h"

/* --------------------------------------------------------------------
                            Get file from server
   -------------------------------------------------------------------- */

int  FilerGetFile(FILERCOM *pFiler, char *RemoteFile, char *LocalFile)
{
   void *Handle;
   size_t size;

   if(!(Handle = (pFiler->pCb->FlcbRecvOpen)(LocalFile))) {
      return CANNOT_CREATE;
   }

   /* send filename */
   if(FilerRequest(pFiler, GET_FILENAME, strlen(RemoteFile),
                 RemoteFile) == NO_RESPONSE) {
      return CANNOT_SEND_FNAME;
   }

   /* initiate transfer */
   if(FilerRequest(pFiler, INIT_GET, 0,NULL) == NO_RESPONSE) {
      (pFiler->pCb->FlcbRecvClose)(Handle);
      return CANNOT_INITIATE;
   }

   pFiler->Size = PACKET_DATA_SIZE;

   /* get complete file */
   for(;;) {
      if(FilerRequest(pFiler, GET_DATA, pFiler->Size,
                    pFiler->pData) == NO_RESPONSE) {
         (pFiler->pCb->FlcbRecvClose)(Handle);
         return NO_RESPONSE;
      }

      size = (pFiler->pCb->FlcbRecvBlock)(pFiler->pData, pFiler->Size, Handle);
      if(size != PACKET_DATA_SIZE) break;  /* end of file */
   }

   (pFiler->pCb->FlcbRecvClose)(Handle);

   /* signal end of data */
   if(FilerRequest(pFiler,DATA_END,0,NULL) == NO_RESPONSE) {
      return NO_RESPONSE;
   }

   return GOT_FILE_OK;

}

