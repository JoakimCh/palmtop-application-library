/* --------------------------------------------------------------------
   Project: HP200LX FILER PROTOCOL (CLIENT) COMMUNICATIONS FOR PAL
   Module:  SNDFILER.C
   Author:  Harry Konstas / Gilles Kohl
   Started: 17. Oct. 95
   Subject: Send a file to Server
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
                            Send file to server
   -------------------------------------------------------------------- */

int  FilerSendFile(FILERCOM *pFiler, char *LocalFile, char *RemoteFile)
{
   void *Handle;
   size_t size;

   if(!(Handle = (pFiler->pCb->FlcbSendOpen)(LocalFile))) {
      return NO_SOURCE_FILE;
   }

   /* send filename */
   if(FilerRequest(pFiler, SEND_FILENAME, strlen(RemoteFile),
                 RemoteFile) == NO_RESPONSE) {
      (pFiler->pCb->FlcbSendClose)(Handle);
      return CANNOT_SEND_FNAME;
   }

   /* send complete file */
   for(;;) {
      size = (pFiler->pCb->FlcbSendBlock)(pFiler->pData, PACKET_DATA_SIZE,
                                        Handle);
      if(!size) break;
      if(FilerRequest(pFiler, SEND_DATA, size, pFiler->pData) == NO_RESPONSE) {
         (pFiler->pCb->FlcbSendClose)(Handle);
         return DISK_FULL;
      }
      if(size != PACKET_DATA_SIZE) break;  /* end of file */
   }

   (pFiler->pCb->FlcbSendClose)(Handle);

   /* signal end of data */
   if(FilerRequest(pFiler,DATA_END,0,NULL) == NO_RESPONSE) {
      return NO_RESPONSE;
   }

   return FILE_SEND_OK;
}

