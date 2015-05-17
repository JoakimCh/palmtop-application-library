/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  LOADPCX.C
   Author:  Gilles Kohl
   Started: 05. Dec. 94
   Subject: Load a bitmap from a monochrome PCX file
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
                              type definitions
   -------------------------------------------------------------------- */
typedef struct {
   BYTE  Manufacturer;
   BYTE  Version;
   BYTE  Encoding;
   BYTE  BitsPerPixel;
   WORD  Xmin;
   WORD  Ymin;
   WORD  Xmax;
   WORD  Ymax;
   WORD  Hres;
   WORD  Vres;
   BYTE  Colormap[16][3];
   BYTE  reserved;
   BYTE  NPlanes;
   WORD  BytesPerLine;
   BYTE  Filler[60];
} PCXHEADER;

/* --------------------------------------------------------------------
                           Functions
   -------------------------------------------------------------------- */

/*
 * Get next (logical) byte from PCX file - expands run length encoding
 *
 */
static char PcxNextByte(FILE *f)
{
   static int  Count = 0;
   static char Byte;
   
   if(!Count) {
      if(fread(&Byte, 1, 1, f)==0) {
         return 0; /* 8 white bits */
      }
      Count = 1;
      if(0xc0 == (Byte & 0xc0)) {
         Count = Byte & 0x3f;
         if(fread(&Byte, 1, 1, f)==0) {
            return 0; /* 8 white bits */
         }
      }
      if(!Count) exit(1);
   }
   --Count;
   return(Byte);
}

/* --------------------------------------------------------------------
   LoadPcx:
   Load an Image file (in PCX format), malloc memory for it and
   return a pointer to it. Returns NULL on failure
   -------------------------------------------------------------------- */

IMGHDR *LoadPcx(char *Name, int Inv)
{
   PCXHEADER PcxHdr;
   IMGHDR *pImg;
   FILE *f;
   BYTE *pBits;

   int i, y, c, xlim;
   int Xor = Inv ? 0 : 0xff; /* invert while loading PCX ? */

   Xor = Inv ? 0 : 0xff;
   if((f = fopen(Name, "rb")) == NULL) return NULL;
   if(fread(&PcxHdr, sizeof(PCXHEADER), 1, f) != 1) {
      fclose(f);
      return NULL;
   }
   xlim = (PcxHdr.Xmax - PcxHdr.Xmin+8)/8;
   pImg = malloc(sizeof(IMGHDR) + xlim * (PcxHdr.Ymax+1));
   if(pImg == NULL) return NULL;
   pBits = (BYTE *)(pImg+1);

   for(y = 0; y <= PcxHdr.Ymax; y++) {
      for(i = 0; i < PcxHdr.BytesPerLine; i++) {
         c = PcxNextByte(f) ^ Xor;
         if(i < xlim) pBits[i] = c;
      }
      pBits += xlim;
   }
   fclose(f);
   pImg->Planes = 1;
   pImg->Bits   = 1;
   pImg->Width  = PcxHdr.Xmax+1;
   pImg->Depth  = PcxHdr.Ymax+1;
   return pImg;
}

