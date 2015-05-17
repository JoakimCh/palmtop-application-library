/* --------------------------------------------------------------------
   Project: HP200LX FILER PROTOCOL (CLIENT) COMMUNICATIONS FOR PAL
   Module:  FILERCB.C
   Author:  Gilles Kohl
   Started: 17. Oct. 95
   Subject: Standard filer callback structure and functions
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

void   *StdFlcbSendOpen(char *FileName);
size_t  StdFlcbSendBlock(void *Buf, size_t Size, void *Handle);
void    StdFlcbSendClose(void *Handle);

void   *StdFlcbRecvOpen(char *FileName);
size_t  StdFlcbRecvBlock(void *Buf, size_t Size, void *Handle);
void    StdFlcbSendClose(void *Handle);


FLCB FlCb = {
   StdFlcbSendOpen,
   StdFlcbSendBlock,
   StdFlcbSendClose,

   StdFlcbRecvOpen,
   StdFlcbRecvBlock,
   StdFlcbSendClose
};

void   *StdFlcbSendOpen(char *FileName)
{
   return fopen(FileName, "rb");
}

size_t  StdFlcbSendBlock(void *Buf, size_t Size, void *Handle)
{
   return fread(Buf, 1, Size, Handle);
}

void    FlcbSendClose(void *Handle)
{
   fclose(Handle);
}

void   *StdFlcbRecvOpen(char *FileName)
{
   return fopen(FileName, "wb");
}

size_t  StdFlcbRecvBlock(void *Buf, size_t Size, void *Handle)
{
   return fwrite(Buf, 1, Size, Handle);
}

void    StdFlcbSendClose(void *Handle)
{
   fclose(Handle);
}

