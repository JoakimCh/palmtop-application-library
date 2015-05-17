/* --------------------------------------------------------------------
   Project: PCX load test program
   Module:  PCXTEST.c
   Author:  Gilles Kohl
   Started: December 5, 1994
   Subject: Test program - load and display a PCX file
   -------------------------------------------------------------------- */

#include <stdio.h>

#include "pal.h"

void main (void)
{
   IMGHDR *pImg;

   if(!PalInit(1)) FatalExit("Init failed - is CGAGRAPH resident ?", 1);

   pImg = LoadPcx("pal.pcx", 0);
   if(!pImg) FatalExit("Load PCX file failed", 1);

   PutImg(220, 0, FORCE_RULE, pImg);
   GetKey();
   PalDeInit(1);
}

