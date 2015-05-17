/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  INITHFN.C
   Author:  Gilles Kohl/Harry Konstas
   Started: 7. Dec. 94
   Subject: Initialize proportional font support
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
                          macro definitions
   -------------------------------------------------------------------- */

#define HFNIMGLEN(Width, Depth) (4+((((Width)+7)/8)*(Depth)))

/* --------------------------------------------------------------------
                              Functions
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
   InitHfn:
   Initializes a font: sets the 'Discardable' flag according to a
   flag passed by the caller, and computes the IMGHDR pointer table
   for the characters that make up the font. This routine is used
   both to initialize resident fonts as well as transient ones.
   -------------------------------------------------------------------- */
void InitHfn(FONTDESC *pFnt, int Discardable, void *pBitmap)
{
   IMGHDR *pBmp;                      /* Bitmap pointer      */
   int Size;
   int c;

   /* indicate type of font as requested by caller */
   pFnt->Discardable = Discardable;


   /* point to bitmaps */
   pBmp = pBitmap ? pBitmap : (IMGHDR *)((char *)pFnt+sizeof(FONTDESC)-4);

   for(c = 0; c < 256; c++) pFnt->pChar[c] = NULL; /* clear table */

   /* Now, initialize pointer table */
   for(c = pFnt->FontHeader.MinChar; c <= pFnt->FontHeader.MaxChar; c++) {
      Size  = HFNIMGLEN(pBmp->Width, pBmp->Depth);
      pFnt->pChar[c] = pBmp; /* PUTIMG header trick */
      pBmp = (IMGHDR *)((char *)pBmp+Size);
   }
}


