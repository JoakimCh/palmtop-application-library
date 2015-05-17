/* Dump all entries in APPMGR.DAT */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pal.h"
#include "palkeys.h"

char MenuStr[16*40];

void main(void)
{
   IMGHDR  *pImg;
   APPDESC *pAd;
   char     Title[80];
   char     Comments[80];
   char     Path[80];
   int      Flags;
   WORD     Key;
   int      Icon;
   int      Count, i;

   if(!PalInit(0)) FatalExit("PALINIT failed - not running on palmtop ?", 1);
   CloseAppMgr();
   pAd = LoadAppDat(NULL);

   if(!pAd) FatalExit("Could not load APPMGR.DAT", 1);
   Count = GetAppCnt(pAd);
   printf("Number of installed applications: %d\n", Count);

   for(i = 0; i < Count; i++) {
      printf("--------------------------------------------------\n");
      GetAppDef(pAd, i, Title, Path, Comments, &Flags, &Key, &Icon);
      printf("  #%2d: %s\n", i+1, Title);
      printf(" Path: %s\n", Path);
      printf(" Cmnt: %s\n", Comments);
      printf("Flags: %s%s%s\n",
             (Flags & AMGR_ROMAPP) ?     "ROMAPP " : "",
             (Flags & AMGR_SYSMGR) ?     "SYSMGR " : "",
             (Flags & AMGR_PERSISTENT) ? "PERSISTENT " : "");
      printf(" Scan: %04X\n", Key);
      printf("Icon#: %d\n\n", Icon);
   }
   printf("\nNext free icon slot: %d\n", NewAppIcn(pAd));
   free(pAd);
   PalDeInit(0);
}

