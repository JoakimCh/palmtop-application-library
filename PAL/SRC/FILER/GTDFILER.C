/* --------------------------------------------------------------------
   Project: HP200LX FILER PROTOCOL (CLIENT) COMMUNICATIONS FOR PAL
   Module:  GTDFILER.C
   Author:  Harry Konstas / Gilles Kohl
   Started: 17. Oct. 95
   Subject: Get directory entry from Server
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
                  Get directory entry from server
   -------------------------------------------------------------------- */

int  FilerGetDir(FILERCOM *pFiler)
{
   int f, stat;

   unsigned long *pHold, *pHold2;

   /* send directory name */
   stat = FilerRequest(pFiler, GET_DIR, 0, NULL);
   if(stat == CANNOT_GET_ENTRY) return stat;

   /* retreive filename */
   for(f=0;f<14;f++)
      pFiler->Name[f]=pFiler->pData[f+9];

   /* retreive date information */
   pHold = (unsigned long *)&pFiler->pData[1];
   pHold2= (unsigned long *)&pFiler->DateStamp;
   *pHold2= *pHold;

   /* retreive file size */
   pHold = (unsigned long *)&pFiler->pData[5];
   pFiler->FileSize = *pHold;

   /* retreive attribute */
   pFiler->Attribute = pFiler->pData[0];

   return GOT_DIR_ENTRY;

}

