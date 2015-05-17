#include <stdio.h>
#include <stdlib.h>

#include <dos.h>

#include "pal.h"

#define SCREENS 10

int Handle;

void Report(void)
{
   MsgBox(" EMS Status report ",
          "|Version of EMS driver : %d|"
          "Total pages available : %d|"
          "Number of free pages  : %d|"
          "Frame segment location: %04X|",
          NULL, " OK ",
          EmsVersion(), EmsPageCnt(), EmsFreeCnt(), EmsFrameSeg());
}

void Screen(int n)
{
   int i;
   char buf[200];

   for(i = 0; i < 640; i+=5) Line(320, 0, i, 199);
   sprintf(buf, "This is screen #%d", n);
   TextOut(0, 0, TXT_RULE, LARGE_FONT, buf);

   for(i = 0; i < n; i++) {
      Rectangle(320-20*i, 100-10*i, 320+20*i, 100+10*i, OUTLINE_FILL);
   }
}

void main(void)
{
   int i, j;
   void far *p;

   if(!PalInit(1)) FatalExit("Pal init fail", 1);
   if(!EmsInit())  FatalExit("EMS support not available", 1);

   Report();
   if((Handle = EmsAlloc(SCREENS)) == EMS_ERR) FatalExit("EMS Alloc fail",1);
   Report();

   for(i = 0; i < SCREENS; i++) {
      Screen(i);
      p = EmsMap(Handle, i, 0);
      GetImg(0, 0, 639, 199, p);
   }
   GetKey();

   for(j = 0; j < 5; j++) {
      for(i = 0; i < SCREENS; i++) PutImg(0, 0, FORCE_RULE, EmsMap(Handle, i, 0));
   }
   EmsFree(Handle);

   PalDeInit(1);
}


