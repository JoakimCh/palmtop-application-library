/* ------------------------------------------------------------
        VFN to HFN converter utility  Vr.1.01 June 14, 1995
   ------------------------------------------------------------ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "pal.h"

#define IMGLENGTH(Width, Depth) ((((Width)+7)/8)*(Depth))

/* VR font header */
typedef struct {
   int maxwidth;
   int vbytes;
   int minchar;
   int maxchar;
   int isfixed;
} VFNHDR;

typedef struct FONTDESCTag {
   BYTE *pBits;
   VFNHDR h;
} VFNDESC;

IMGHDR *pHfn[256]; /* HFN font pointer table */


/* ------------------------------------------------------------
                         Font loader
   ------------------------------------------------------------ */

VFNDESC *LoadVfnFont(char *Name)
{
   VFNDESC *pFnt;
   FILE *f;
   long fs;

   if(!(f = fopen(Name, "rb"))) return NULL;

   fseek(f, 0L, SEEK_END);  /* seek to end of file */
   fs = ftell(f);           /* get lenght of  file */
   fseek(f, 0L, SEEK_SET);  /* seek to beginning again */

   if(!(pFnt = malloc(sizeof(VFNDESC)+fs-sizeof(VFNHDR)))) {
      fclose(f);
      return NULL;
   }

   /* get File font header and bitmap at once */
   if(fread(&pFnt->h, fs, 1, f) != 1) {
      fclose(f);
      return NULL;
   }

   pFnt->pBits = (BYTE *)(pFnt+1);
   fclose(f);
   return pFnt;
}


/* ------------------------------------------------------------
                   Capture a VFN character
   ------------------------------------------------------------ */

IMGHDR *CaptureChr(VFNDESC *pFnt, int Chr)
{
   int w;
   int d;
   WORD   *pChr;   /* word pointer */
   BYTE   *pChb;   /* byte pointer */

   IMGHDR *pHfn;
   int ImgSize;
   int i;

   d = pFnt->h.vbytes * 8;     /* calculate depth */
   pChr = (WORD *)pFnt->pBits + pFnt->h.maxwidth * (Chr - pFnt->h.minchar);
   pChb = (BYTE *)pFnt->pBits + pFnt->h.maxwidth * (Chr - pFnt->h.minchar);

   if(d==16) w = *pChr++;     /* 16 bit fonts */
   else      w = *pChb++;     /* 8 bit fonts  */

   SetColor(WHITE_COLOR);
   Rectangle(0, 0, 32, 32, SOLID_FILL);  /* clear display area */
   SetColor(BLACK_COLOR);


   /* Rotate and plot character */
   for(i = 0; i < w; i++) {

      WORD m;
      BYTE mb;
      int  n;

      if(d == 8) {
          for(mb = 128, n = 0; mb != 0; mb /= 2, n++) {
             BYTE c = pChb[i];
             if(c & mb) PlotDot(i, n);
         }
       }

      if(d == 16) {
          for(m = 32768, n = 0; m != 0; m /= 2, n++) {
             WORD c = (pChr[i] << 8)+(pChr[i]/256);
             if(c & m) PlotDot(i, n);
          }
      }

   }

   ImgSize = IMGSIZE(0, 0, w-1, d-1);
   pHfn = malloc(ImgSize);
   GetImg(0, 0, w-1, d-1, pHfn);
   return pHfn;
}


/* -----------------------------------------------------------
                     Capture all VFN characters
   ----------------------------------------------------------- */

void CaptureFont(VFNDESC *pFnt)
{
   int Chr;

   for(Chr=0;Chr<256;Chr++) pHfn[Chr]=0;   /* clear table */

   for(Chr = pFnt->h.minchar; Chr <= pFnt->h.maxchar; Chr++) {
      pHfn[Chr] = CaptureChr(pFnt, Chr);
   }
}


/* -----------------------------------------------------------
                       Create HFN file
   ----------------------------------------------------------- */

void vfn2hfn(VFNDESC *pFnt, FILE *dst, int ftype)
{

   int  *lptr;
   char *fptr;
   BYTE hi,lo;
   WORD Width, Depth, Val, len, b;

   /* Write HFN header */
   lo = ftype; hi = ftype >> 8;
   fputc(lo,dst);            /* Font type */
   fputc(hi,dst);

   Val = pFnt->h.minchar;    /* ASCII min character */
   lo = Val; hi = Val >> 8;
   fputc(lo,dst);
   fputc(hi,dst);

   Val = pFnt->h.maxchar;    /* ASCII max character */
   lo = Val; hi = Val >> 8;
   fputc(lo,dst);
   fputc(hi,dst);

   Val = pFnt->h.maxwidth;   /* MaxWidth */
   lo = Val; hi = Val >> 8;
   fputc(lo,dst);
   fputc(hi,dst);

   Val = pFnt->h.vbytes;     /* MaxHeight */
   lo = Val*8; hi = Val >> 8;
   fputc(lo,dst);
   fputc(hi,dst);

   SelectFont(MEDIUM_FONT);
   WriteText(160,80,"Converting, please Stand-by...");
   CaptureFont(pFnt);
   TextMode();              /* Back to MDA mode */


   /* Write bitmap to file */

    printf("\nWriting HFN font...");

    for(Val=0;Val<256;Val++) {

        fptr=(char *)pHfn[Val];
        lptr=(int *)pHfn[Val];

        if(*fptr!=0) {
            Width= *(lptr+2);
            Depth= *(lptr+3);
            if((Width>16)||(Width==0)||(Depth>16)||(Depth==0)) {
               Width=1; *(lptr+2)=1;
               Depth=1; *(lptr+3)=1;
            }

            len=IMGLENGTH(Width,Depth);

            len=len+8;
            for(b=4;b<len;b++) fputc(*(fptr+b),dst);
        }
    }

    printf("\n\nConversion completed.\n");
}


/* ------------------------------------------------------------
                            MAIN
   ------------------------------------------------------------ */

void main(int argc, char *argv[])
{
   VFNDESC *pVfnt;      /* VFN file */
   FILE *pHfnt;         /* HFN file */
   int  ftype, b;
   char outfile[80];

   if(argc != 3)  FatalExit("Usage: VFN2HFN <VFN file> <Font ID number>", 1);
   if(!PalInit(1)) FatalExit("GraphMode() fail - CGAGRAPH not loaded ?", 1);
   if(!(pVfnt = LoadVfnFont(argv[1]))) FatalExit("Load Font failed", 1);

   /* Change filename to .HFN */
   strcpy(outfile,argv[1]);
   for (b=0;b<80;b++)
      if(outfile[b]=='.') outfile[b+1]='h';
   pHfnt = fopen(outfile,"wb");

   ftype = atoi(argv[2]);

   vfn2hfn(pVfnt, pHfnt, ftype);     /* convert file */
   PalDeInit(1);
}

