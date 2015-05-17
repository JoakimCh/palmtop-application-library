/* --------------------------------------------------------------------
   Project: PAL FONT Editor Vr. 1.02
   Module:  PFECO.C
   Author:  Harry Konstas
   Started: 10 May '95
   Subject: PFE core
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                            standard includes
   -------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#if defined _MSC_VER || defined _QC
#include <malloc.h>
#else
#include <alloc.h>
#endif

/* --------------------------------------------------------------------
                             local includes
   -------------------------------------------------------------------- */

#include "pal.h"      /* PAL Header file */
#include "pfe.h"      /* PFE Header file */
#include "pfers.h"    /* PFE resource header file */


/* --------------------------------------------------------------------
                          Global Variables
   -------------------------------------------------------------------- */

extern int Select;
extern int Modified;
extern CHARIMG *pCIM[256];   /* bitmap pointer table */
extern char *KeyLabels[10];
extern char *EditLabels[10];
extern char *OKCANLabels[10];
extern BYTE Matrix[16][16];
extern BYTE Undo[16][16];    /* Restore matrix */
extern BYTE Rot[16][16];     /* rotation matrix */

extern BYTE ResFont[];
extern char FontName[80];
extern char Opename[80];

int ftype;           /* Font type */
int AscMin;          /* ASCII minimum */
int AscMax;          /* ASCII Maximum */
int MaxWidth;
int MaxHeight;

/* Empty character definition */
char space[]= {1,0, 1,0, 8,0,8,0, 0,0,0,0,0,0,0,0,0,0,0,0 };


/* --------------------------------------------------------------------
                                 Help window
   -------------------------------------------------------------------- */

void Help(void)

{
    PALWIN *pWin;
    int x=40,y=8;

    pWin  = OpenWin(WS_HELP, 30, 10, 600, 190, "Help");
    SelectFont(MEDIUM_FONT);

    WinText(pWin, x,y,    " KEY |    SELECT-MODE     |     EDIT-MODE      ");
    WinText(pWin, x,y+10, "-----|--------------------|--------------------");
    WinText(pWin, x,y+20, "  F1 | This help screen.  | Rotate character.  ");
    WinText(pWin, x,y+30, "  F2 | New font.          | Invert character.  ");
    WinText(pWin, x,y+40, "  F3 | Enter Edit mode -> | Width + (ShiftF3 -)");
    WinText(pWin, x,y+50, "  F4 | Set font size.     | Height+ (ShiftF4 -)");
    WinText(pWin, x,y+60, "  F5 | Copy char to buffer| Scroll Left.       ");
    WinText(pWin, x,y+70, "  F6 | Paste char->screen.| Scroll Right.      ");
    WinText(pWin, x,y+80, "  F7 | Convert ISO to IBM.| Scroll Up.         ");
    WinText(pWin, x,y+90, "  F8 | Rotate all chars.  | Scroll Down.       ");
    WinText(pWin, x,y+100,"  F9 | Open new font.     | Restore character. ");
    WinText(pWin, x,y+110," F10 | Save the font.     | Save/exit edit mode");
    WinText(pWin, x,y+120," DEL | Delete character.  | Clear character.   ");
    WinText(pWin, x,y+133," * ALL OTHER KEYS: Selects character to edit.  ");

    GetKey();
    CloseWin(pWin);
}


/* --------------------------------------------------------------------
                           About window
   -------------------------------------------------------------------- */

void About(void)

{
    PALWIN *pWin;
    pWin  = OpenWin(WS_HP200, 140, 50, 470, 150, "About PFE");
    WinText(pWin, 35,5,   "         The PAL         ");
    WinText(pWin, 35,25,  "   Font Editor Vr. 1.02  ");
    WinText(pWin, 35,50 , "      The PAL group.");
    WinText(pWin, 35,70,  "         (c)1995.        ");

    GetKey();
    CloseWin(pWin);
}


/* --------------------------------------------------------------------
                        Del character from font
   -------------------------------------------------------------------- */

void DelChar(int ascii)
{
   PALWIN *pMsg;
   char *pBits;
   int c,r,x,y;

   pMsg = OpenWin(WS_HP200, 120, 50, 470, 110, "Attention!");
   WriteText(150,70, " Remove character from font?");
   WriteText(200,90, "Hit [Y] to confirm.");
   printf("\a");  /* bell */
   c=GetKey()&0xff;
   CloseWin(pMsg);

   if(!((c==89)||(c==121))) return; /* pressed Y ? */

   pBits=(char *)pCIM[ascii];
   for(r=0;r<40;r++) {
     *pBits = 0;
     pBits++;
   }

   pCIM[ascii]->Width=8;
   pCIM[ascii]->Depth=8;

   r=Select/26;
   c=Select%26;
   x=AHOFS+(c*16);
   y=AVOFS+(r*16);

   SetRule(TXT_RULE);
   SelectFont(MEDIUM_FONT);

   /* erase character */
   SetColor(WHITE_COLOR);
   Rectangle(x,y,x+15,y+15,SOLID_FILL);
   SetColor(BLACK_COLOR);
   WriteText(x,y,".");
   SetRule(FORCE_RULE);
   OnGrid(ascii);
}


/* --------------------------------------------------------------------
                         Font setup
   -------------------------------------------------------------------- */

void FontSetup(int clear)
{
   PALWIN *pWin;
   int w,h,r,f,istat;
   char *pBits;

   if(!clear) {

      InitDialog(&FontSize);
      ShowDialog(&FontSize, 140, 30, "Set Global font size");
      ShowFKeys(OKCANLabels);

      SetDlgItem(&FontSize, FONTW, EDSI_TXT, "8");
      SetDlgItem(&FontSize, FONTH, EDSI_TXT, "8");

      istat=HandleDialog(&FontSize, NULL);
      ShowFKeys(KeyLabels);

      if(istat==DN_CANCEL) {
         CloseDialog(&FontSize);
         return;
      }

      GetDlgItem(&FontSize, FONTW, EDGI_INT, &w);
      GetDlgItem(&FontSize, FONTH, EDGI_INT, &h);

      CloseDialog(&FontSize);
   }

   pWin = OpenWin(WS_HP200, 140, 50, 470, 110, "Please wait");
   WriteText(220,80,  "Re-Sizing Font...");

   if((w<0)||(w>16)) w=8;
   if((h<0)||(h>16)) h=8;

   for(f=0;f<256;f++) {

      if(clear) {                 /* clear font? */
         pBits=(char *)pCIM[f];
         for(r=0;r<40;r++) {
            *pBits = 0;
            pBits++;
         }
         pCIM[f]->Width=16;
         pCIM[f]->Depth=16;
      }


      if(!clear)  {  /* change only size? */
         if(pCIM[f]->Flag) Bit2Matrix(f);
         if(w) pCIM[f]->Width=w;
         if(h) pCIM[f]->Depth=h;
         if(pCIM[f]->Flag) Matrix2Bit(f);
      }

   }

   MaxWidth=w;
   MaxHeight=h;
   CloseWin(pWin);
   DisplayFont();
   OnGrid(Select);
}


/* --------------------------------------------------------------------
                         Clear complete font
   -------------------------------------------------------------------- */

void ClearFont(void)
{
   PALWIN *pMsg;
   int c;

   pMsg = OpenWin(WS_HP200, 120, 50, 470, 110, "Attention!");
   WriteText(200,70, " Create new font ?");
   WriteText(200,90, "Hit [Y] to confirm.");
   printf("\a");  /* bell */
   c=GetKey()&0xff;
   CloseWin(pMsg);

   if(!((c==89)||(c==121))) return;  /* is it Y? */

   FontSetup(1);      /* clear font size */
   FontSetup(0);      /* enter new size */
}


/* --------------------------------------------------------------------
                         Display graphical byte
   -------------------------------------------------------------------- */

void GraphByte(int col, int row, int value)
{
   int f,i=7;
   char binary[9]={9,9,9,9,9,9,9,9,0};

   SelectFont(MEDIUM_FONT);
   col=HOFS+(col*10);
   row=VOFS+(row*10);
   SetRule(TXT_RULE);
   WriteText(col,row,binary);

   for(f=1;f<130;f=f*2,i--) if(f&value) binary[i]=7;

   SetRule(FORCE_RULE);
   WriteText(col,row,binary);
}


/* --------------------------------------------------------------------
                        Bitmap Cursor display
   -------------------------------------------------------------------- */

void BitCursor(int state, int col, int row)
{
   int hcol, hrow;
   hcol=HOFS+(col*10);
   hrow=VOFS+(row*10);

   SelectFont(MEDIUM_FONT);
   SetRule(FORCE_RULE);
   SetColor(WHITE_COLOR);
   WriteText(hcol,hrow,"\x0a");
   SetColor(BLACK_COLOR);

   if(state==TRUE) {
      if(Matrix[col][row]) WriteText(hcol,hrow,"\x0a");
      else WriteText(hcol,hrow, "\x08");
   }

   if(state==FALSE) {
      if(Matrix[col][row]) WriteText(hcol,hrow,"\x07");
      else WriteText(hcol,hrow, "\x09");
   }

}


/* --------------------------------------------------------------------
                         ASCII table cursor
   -------------------------------------------------------------------- */

void AscCursor(int ascii)
{

   int c,r;

   r=ascii/26;
   c=ascii%26;
   SetRule(XOR_RULE);
   Rectangle(AHOFS+(c*16)-1,AVOFS+(r*16)-1,AHOFS+(c*16)+15,
             AVOFS+(r*16)+15, OUTLINE_FILL);
   SetRule(FORCE_RULE);
}


/* --------------------------------------------------------------------
                        Display character on grid
   -------------------------------------------------------------------- */

void OnGrid(int ascii)
{

   int r, depth, width;

   char *pBits;
   char buffer[80];

   SetRule(TXT_RULE);
   SelectFont(MEDIUM_FONT);

   r=Select;
   if(!r) r=32;
   sprintf(buffer, " ASCII:%3u \"%c\" SIZE:%2dx%-2d ",
           Select, r, pCIM[ascii]->Width,pCIM[ascii]->Depth);

   WriteText(200,0,buffer);

   SetRule(FORCE_RULE);

   pBits= (char *)pCIM[ascii]->Bitmap;
   width= pCIM[ascii]->Width;
   depth= pCIM[ascii]->Depth;

   SetColor(WHITE_COLOR);
   Rectangle(HOFS,VOFS,HOFS+160,VOFS+160, OUTLINE_FILL);
   SetColor(BLACK_COLOR);

   if(!pCIM[ascii]->Flag) {    /* undefined ? */
      for(r=0;r<16;r++) {
         GraphByte(0,r,0);
         GraphByte(8,r,0);
      }
      return;
   }

   for(r=0;r<16;r++) {

      if(r>=depth) {
         GraphByte(0,r,0);
         GraphByte(8,r,0);
      }

      else {
         GraphByte(0,r, *(pBits++));
         if(width>8)
            GraphByte(8,r, *(pBits++));   /* 16 bit wide fonts */
         else GraphByte(8,r, 0);
      }
   }
   Rectangle(HOFS,VOFS,HOFS+(width*10),VOFS+(depth*10), OUTLINE_FILL);
}


/* --------------------------------------------------------------------
                     Display font on screen
   -------------------------------------------------------------------- */

void DisplayFont(void)
{
   WORD i=0, r, c;

   SetColor(WHITE_COLOR);
   SetRule(FORCE_RULE);
   Rectangle(AHOFS-5,AVOFS-3,AHOFS+420,AVOFS+164,SOLID_FILL);
   SetColor(BLACK_COLOR);
   Rectangle(AHOFS-5,AVOFS-3,AHOFS+420,AVOFS+164,OUTLINE_FILL);

   for(r=0;r<10;r++) {
      for(c=0;c<26;c++) {
         if((i<256) && (pCIM[i]->Flag==1))
            PutImg(AHOFS+(c*16),AVOFS+(r*16), TXT_RULE, pCIM[i]);
         else WriteText(AHOFS+(c*16),AVOFS+(r*16),".");
         i++;
      }
   }
}


/* --------------------------------------------------------------------
                            Bitmap -> Matrix
   -------------------------------------------------------------------- */

void Bit2Matrix(int ascii)
{
   int v,b, width, depth, c, r;
   char *pBits;

   pBits=(char *)pCIM[ascii]->Bitmap;
   width=pCIM[ascii]->Width;
   depth=pCIM[ascii]->Depth;

   for(r=0;r<16;r++) {

      v=*pBits;
      for(c=7,b=1;b<130;b=b*2) {
         Matrix[c][r]=0;
         if((v & b) && (r<depth)) Matrix[c][r]=1;

         c--;
      }

      if(width>8) pBits++;

      v=*pBits;
      for(c=15,b=1;b<130;b=b*2) {
         Matrix[c][r]=0;
         if((width>8) && (r<depth)) {
            if(v & b) Matrix[c][r]=1;
         }
         c--;
      }
      pBits++;
   }

}


/* --------------------------------------------------------------------
                            Matrix -> Bitmap
   -------------------------------------------------------------------- */

void Matrix2Bit(int ascii)
{
   int c, r, b, width;
   BYTE bitmap;

   char *pBits;

   pBits=(char *)pCIM[ascii]->Bitmap;
   width=pCIM[ascii]->Width;

   for(r=0;r<16;r++) {

      bitmap=0;
      for(c=7,b=1;b<130;b=b*2) {
         if(Matrix[c][r]) bitmap += b;
         c--;
      }

      *pBits=bitmap;
      pBits++;

      bitmap=0;
      for(c=15,b=1;b<130;b=b*2) {
         if(Matrix[c][r]) bitmap += b;
         c--;
      }

      if(width>8) {
         *pBits=bitmap;
         pBits++;
      }
   }

}


/* --------------------------------------------------------------------
                            Rotate Font
   -------------------------------------------------------------------- */

void RotateFont(void)
{
   int f, width, depth, ir, ic;
   PALWIN *pWin;

   /* clear current width rectangle */
   SetRule(FORCE_RULE);
   width=pCIM[Select]->Width;
   depth=pCIM[Select]->Depth;
   SetColor(WHITE_COLOR);
   Rectangle(HOFS,VOFS,HOFS+(width*10),VOFS+(depth*10), OUTLINE_FILL);
   SetColor(BLACK_COLOR);

   pWin = OpenWin(WS_HP200, 140, 50, 470, 110, "Please wait");
   WriteText(220,80,  "Rotating Font...");

   for(f=0;f<256;f++) {
      if(pCIM[f]->Flag) {
         width=pCIM[f]->Width;
         depth=pCIM[f]->Depth;
         Bit2Matrix(f);

         /* clear ROT matrix */
         for(ic=0;ic<16;ic++)
            for(ir=0;ir<16;ir++)
               Rot[ic][ir]=0;

         /* rotate matrix */
         for(ir=0;ir<depth;ir++)
            for(ic=0;ic<width;ic++)
               Rot[depth-ir-1][ic]=Matrix[ic][ir];

         for(ic=0;ic<16;ic++)
            for(ir=0;ir<16;ir++)
               Matrix[ic][ir]=Rot[ic][ir];

         pCIM[f]->Width=depth;
         pCIM[f]->Depth=width;
         Matrix2Bit(f);

      }
   }

   CloseWin(pWin);
   DisplayFont();
   OnGrid(Select);

}


/* --------------------------------------------------------------------
                           Convert to VFN
   -------------------------------------------------------------------- */

void ConvVFN(void)
{
   int f, width, depth, ir, ic;
   PALWIN *pWin;

   /* clear current width rectangle */
   SetRule(FORCE_RULE);
   width=pCIM[Select]->Width;
   depth=pCIM[Select]->Depth;
   SetColor(WHITE_COLOR);
   Rectangle(HOFS,VOFS,HOFS+(width*10),VOFS+(depth*10), OUTLINE_FILL);
   SetColor(BLACK_COLOR);

   pWin = OpenWin(WS_HP200, 140, 50, 470, 110, "Please wait");
   WriteText(220,80,  "Converting Font...");

   for(f=0;f<256;f++) {
      if(pCIM[f]->Flag) {
         width=pCIM[f]->Width;
         depth=pCIM[f]->Depth;
         Bit2Matrix(f);

         /* clear ROT matrix */
         for(ic=0;ic<16;ic++)
            for(ir=0;ir<16;ir++)
               Rot[ic][ir]=0;

         /* rotate matrix */
         for(ir=0;ir<depth;ir++)
            for(ic=0;ic<width;ic++)
               Rot[depth-ir-1][ic]=Matrix[ic][ir];

         /* clear matrix */
         for(ic=0;ic<16;ic++)
            for(ir=0;ir<16;ir++)
               Matrix[ic][ir]=0;

         /* copy & flip matrix */
         for(ic=0;ic<depth;ic++)
            for(ir=0;ir<16;ir++)
               Matrix[depth-ic-1][ir]=Rot[ic][ir];

         pCIM[f]->Width=depth;
         pCIM[f]->Depth=width;
         Matrix2Bit(f);

      }
   }

   CloseWin(pWin);
   DisplayFont();
   OnGrid(Select);

}


/* --------------------------------------------------------------------
                        Convert ISO Latin to IBM 850
   -------------------------------------------------------------------- */


void FromLatin(void)
{
   PALWIN *pMsg;
   CHARIMG *pCIM2[256];
   int i,c;

   static char lat[] = {0xC7,
   0xFC, 0xE9, 0xE2, 0xE4, 0xE0, 0xE5, 0xE7, 0xEA, 0xEB, 0xE8, 0xEF, 0xEE, 
   0xEC, 0xC4, 0xC5, 0xC9, 0xE6, 0xC6, 0xF4, 0xF6, 0xF2, 0xFB, 0xF9, 0x20, 
   0xD6, 0xDC, 0xF8, 0xA3, 0xD8, 0xD7, 0x2D, 0xE1, 0xED, 0xF3, 0xFA, 0xF1, 
   0xD1, 0xAA, 0xBA, 0xBF, 0xAE, 0xAC, 0xBD, 0xBC, 0xA1, 0xAB, 0xBB, 0x2D, 
   0x2D, 0x2D, 0x2D, 0x2D, 0xC1, 0xC2, 0xC0, 0xA9, 0x2D, 0x2D, 0x2D, 0x2D, 
   0xA2, 0xA5, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0xE3, 0xC3, 0x2D, 
   0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0x2D, 0xA4, 0xF0, 0xD0, 0xCA, 0xCB, 0xC8, 
   0x2D, 0xCD, 0xCE, 0xCF, 0x2D, 0x2D, 0x2D, 0x2D, 0xA6, 0xCC, 0x2D, 0xD3, 
   0xDF, 0xD4, 0xD2, 0xF5, 0xD5, 0xB5, 0xFE, 0xDE, 0xDA, 0xDB, 0xD9, 0xFD, 
   0xDD, 0xAF, 0xB4, 0xAD, 0xB1, 0x2D, 0xBE, 0xB6, 0xA7, 0xF7, 0xB8, 0xB0, 
   0xA8, 0xB7, 0xB9, 0xB3, 0xB2, 0x2D, 0x2D, 
   };


   pMsg = OpenWin(WS_HP200, 120, 50, 470, 110, "Attention!");
   WriteText(130,70, " Convert ISO Latin to IBM 850 ?");
   WriteText(200,90, "Hit [Y] to confirm.");
   printf("\a");  /* bell */
   c=GetKey()&0xff;
   CloseWin(pMsg);

   if(!((c==89)||(c==121))) return;

   for (i = 128; i <= 255; i++)
      pCIM2[i] = pCIM[128 + (lat[i & 127] & 127)];
   for (i = 128; i <= 255; i++)
      pCIM[i] = pCIM2[i];

   Rectangle(AHOFS-5,AVOFS-3,AHOFS+420,AVOFS+164,OUTLINE_FILL);
   DisplayFont();
}


/* --------------------------------------------------------------------
                      Edit window 'hair' strings
   -------------------------------------------------------------------- */

void EditString(int ascii)
{
   int ic, ir;

   SetRule(XOR_RULE);
   SetLineType(DASHED);
   ir=ascii/26;
   ic=ascii%26;

   ic=AHOFS+(ic*16)-1;
   ir=AVOFS+(ir*16)-1;

   Line(188,VOFS-6,ic,ir);
   Line(188,183,ic,ir+16);

   SetLineType(SOLID);
   SetRule(FORCE_RULE);
}


/* --------------------------------------------------------------------
                            EDIT bitmap
   -------------------------------------------------------------------- */

void EditChar(int ascii)
{

   char *pBits;
   int f, j, ir,ic, c=0,r=0, width, depth, key;
   int oldWidth, oldDepth;
   char buffer[40];

   EditString(ascii);

   /* clear bitmap if undefined */
   if(!pCIM[ascii]->Flag) {
      pBits=(char *)pCIM[ascii];
      pBits+=8;
         for(ir=8;ir<40;ir++) {
            *pBits = 0;
            pBits++;
         }
   }

   pCIM[ascii]->Flag=1;        /* now defined */
   width= pCIM[ascii]->Width;
   depth= pCIM[ascii]->Depth;

   if((width<4)||(width>16)) width=8;
   if((depth<4)||(depth>16)) depth=8;

   oldWidth=width;
   oldDepth=depth;

   Bit2Matrix(ascii);
   /* save for undo */
   for(ic=0;ic<16;ic++)
      for(ir=0;ir<16;ir++)
         Undo[ic][ir]=Matrix[ic][ir];

   AscCursor(Select);

   ShowFKeys(EditLabels);
   BitCursor(TRUE,c,r);

   do {
      key=GetKey();
      BitCursor(FALSE,c,r);
      SetRule(XOR_RULE);
      Rectangle(HOFS,VOFS,HOFS+(width*10),VOFS+(depth*10), OUTLINE_FILL);
      SetRule(FORCE_RULE);

      switch(key) {

      case RIGHT_KEY:
         c++;
         if(c>=width) c=0;
         break;

      case LEFT_KEY:
         c--;
         if(c<0) c=width-1;
         break;

      case DOWN_KEY:
         r++;
         if(r>=depth) r=0;
         break;

      case UP_KEY:
         r--;
         if(r<0) r=depth-1;
         break;


      /* Function keys */

      case F2_KEY:          /* Invert */
         for(f=0;f<width;f++)
            for(j=0;j<depth;j++)
               Matrix[f][j]=!Matrix[f][j];
         Matrix2Bit(ascii);
         OnGrid(ascii);
         break;


      case DEL_KEY:           /* clear */
         for(f=0;f<16;f++)
            for(j=0;j<16;j++)
               Matrix[f][j]=0;
         Matrix2Bit(ascii);
         OnGrid(ascii);
         break;

      case F1_KEY:           /* rotate */

         /* clear ROT matrix */
         for(ic=0;ic<16;ic++)
            for(ir=0;ir<16;ir++)
               Rot[ic][ir]=0;

         /* rotate matrix */
         for(ir=0;ir<depth;ir++)
            for(ic=0;ic<width;ic++)
               Rot[depth-ir-1][ic]=Matrix[ic][ir];

         for(ic=0;ic<16;ic++)
            for(ir=0;ir<16;ir++)
               Matrix[ic][ir]=Rot[ic][ir];

         pCIM[ascii]->Width=depth;
         pCIM[ascii]->Depth=width;
         width=pCIM[ascii]->Width;
         depth=pCIM[ascii]->Depth;
         Matrix2Bit(ascii);
         OnGrid(ascii);
         break;


      case F5_KEY:           /* scroll left */
         for(ic=0;ic<width;ic++)
            for(ir=0;ir<depth;ir++)
               Matrix[ic][ir]=Matrix[ic+1][ir];

         Matrix2Bit(ascii);
         OnGrid(ascii);
         break;


      case F6_KEY:           /* scroll right */
         for(ic=width-1;ic>0;ic--)
            for(ir=0;ir<depth;ir++)
               Matrix[ic][ir]=Matrix[ic-1][ir];

         for(ir=0;ir<16;ir++) Matrix[0][ir]=0;
         Matrix2Bit(ascii);
         OnGrid(ascii);


         break;


      case F7_KEY:           /* scroll up */
         for(ic=0;ic<width;ic++)
            for(ir=0;ir<depth;ir++)
               Matrix[ic][ir]=Matrix[ic][ir+1];

         Matrix2Bit(ascii);
         OnGrid(ascii);
         break;


      case F8_KEY:           /* scroll down */
         for(ic=0;ic<width;ic++)
            for(ir=depth-1;ir>=0;ir--)
               Matrix[ic][ir]=Matrix[ic][ir-1];

         Matrix2Bit(ascii);
         OnGrid(ascii);
         break;


      case F9_KEY:           /* restore */
         for(ic=0;ic<16;ic++)
            for(ir=0;ir<16;ir++)
               Matrix[ic][ir]=Undo[ic][ir];

         Matrix2Bit(ascii);
         OnGrid(ascii);
         SetRule(XOR_RULE);
         Rectangle(HOFS,VOFS,HOFS+(width*10),VOFS+(depth*10), OUTLINE_FILL);
         SetRule(FORCE_RULE);
         width=oldWidth;
         depth=oldDepth;
         Rectangle(HOFS,VOFS,HOFS+(width*10),VOFS+(depth*10), OUTLINE_FILL);
         break;


      case F3_KEY:           /* width + */
         width++;
         if(width>16) width=4;
         break;

      case SF3_KEY:           /* width - */
         width--;
         if(width<4) width=16;
         break;

      case F4_KEY:           /* height + */
         depth++;
         if(depth>16) depth=4;
         break;

      case SF4_KEY:          /* height - */
         depth--;
         if(depth<4) depth=16;
         break;


      case KEYENTER:   /* exit edit mode */
      case F10_KEY:
         Rectangle(HOFS,VOFS,HOFS+(width*10),VOFS+(depth*10), OUTLINE_FILL);
         Matrix2Bit(ascii);   /* convert & save new character */

         /* insert new values */
         pCIM[ascii]->Flag=1;
         pCIM[ascii]->Bits=1;
         pCIM[ascii]->Width=width;
         pCIM[ascii]->Depth=depth;

         ShowFKeys(KeyLabels);
         EditString(ascii);
         AscCursor(Select);
         return;

      case SPCBAR:   /* toggle dot */
         Matrix[c][r]= !Matrix[c][r];
         break;

      }

      Matrix2Bit(ascii);
      ir=ascii/26; ic=ascii%26;
      AscCursor(ascii);

      SetColor(WHITE_COLOR);  /* clear area before draw */
      Rectangle(AHOFS+(ic*16),AVOFS+(ir*16),AHOFS+(ic*16)+15,
                AVOFS+(ir*16)+15, SOLID_FILL);
      SetColor(BLACK_COLOR);
      PutImg(AHOFS+(ic*16),AVOFS+(ir*16), TXT_RULE, pCIM[ascii]);
      AscCursor(ascii);
     
      BitCursor(TRUE,c,r);
      pCIM[ascii]->Width=width;
      pCIM[ascii]->Depth=depth;

      Rectangle(HOFS,VOFS,HOFS+(width*10),VOFS+(depth*10), OUTLINE_FILL);

      SetRule(TXT_RULE);
      SelectFont(MEDIUM_FONT);

      ir=ascii;
      if(!ir) ir=32;
      sprintf(buffer, " ASCII:%3u \"%c\" SIZE:%2dx%-2d ",
              ascii, ir, width,depth);

      WriteText(200,0,buffer);

      SetRule(FORCE_RULE);
      SelectFont(MEDIUM_FONT);

   } while(TRUE);

}


/* ------------------------------------------------------------
                     Display filename
   ------------------------------------------------------------ */

void DisplayFname(char *filename)
{
   char buffer[80];

   SelectFont(SMALL_FONT);
   SetRule(TXT_RULE);
   sprintf(buffer, "FONT:%15s", filename);
   buffer[21]=0;
   WriteText(470,1,buffer);
   SetRule(FORCE_RULE);
}


/* --------------------------------------------------------------------
                       Change extension of a filename
   -------------------------------------------------------------------- */

void AddExt(char *Dest, char *Source, char *Ext)
{
    char *s;
    strcpy(Dest, Source);
    s = strrchr(Dest, '.');
    if(s == NULL) s = Dest+strlen(Dest);
    strcpy(s, Ext);
}


/* --------------------------------------------------------------------
                         Grab HFN font in memory
   -------------------------------------------------------------------- */

int GrabFont(char *filename)
{

   int c,r,Width,Depth,len;
   char *pBits;
   FONTDESC *pFont;
   int sFlag=1;

   if(!(pFont=LoadHfn(filename))) {
      sFlag=0; /* not found */
      if(pFont = malloc(sizeof(FONTDESC))) {
         memcpy(&pFont->FontHeader, ResFont, sizeof(FNTFHDR));
         InitHfn(pFont, 1, (char *)ResFont+sizeof(FNTFHDR)-4);
      }
   }

   ftype=pFont->FontHeader.FontType;
   AscMin=pFont->FontHeader.MinChar;
   AscMax=pFont->FontHeader.MaxChar;
   MaxWidth=pFont->FontHeader.MaxWidth;
   MaxHeight=pFont->FontHeader.MaxHeight;

   /* store into 16x16 memory grids */
   for(c=0;c<256;c++) {

      pBits=(char *)pFont->pChar[c];

      if(pBits) {
         Width=pFont->pChar[c]->Width;
         Depth=pFont->pChar[c]->Depth;
         pCIM[c]->Flag=1;    /* defined */
      }

      else {
         Width=15;
         Depth=15;
         pCIM[c]->Flag=0;    /* undefined */
      }

      len=IMGLENGTH(Width, Depth);
      pCIM[c]->Width=Width;
      pCIM[c]->Depth=Depth;

      pBits+=8;   /* bypass IMG header */

      /* copy bitmap */
      for(r=0;r<len;r++)
         if(pCIM[c]->Flag)   /* defined ? */
            pCIM[c]->Bitmap[r] = *pBits++;
         else pCIM[c]->Bitmap[r]=0;
   }

   /* No need anymore, we have it in 16x16 format */
   DiscardHfn(pFont);

   OnGrid(Select);
   return sFlag;
}


/* ------------------------------------------------------------
                     VFN Font loader
   ------------------------------------------------------------ */

int LoadVfn(char *Name)
{
   VFNDESC *pFnt;
   FILE *f;
   char *pBits, *pSrc;
   long fs;
   int i,im,ascii,size;

   if(!(f = fopen(Name, "rb"))) return 0;

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
      free(pFnt);
      return NULL;
   }

   fclose(f);

   ftype=1;
   AscMin=pFnt->h.minchar;
   AscMax=pFnt->h.maxchar;
   MaxWidth=pFnt->h.vbytes * 8;
   MaxHeight=pFnt->h.maxwidth-1;

   /* clear font area */
   for(i=0;i<256;i++) {
      pCIM[i]->Flag=0;   /* undefined */
      pCIM[i]->Bits=1;
      pCIM[i]->Width=8;
      pCIM[i]->Depth=8;
   }

   pBits = (char *)&pFnt->h;
   pBits = pBits + 10;

   /* copy VFN into the HFN slots */
   for(ascii=AscMin;ascii<=AscMax;ascii++) {
      pCIM[ascii]->Flag=1;
      pCIM[ascii]->Width=MaxWidth;

      size= *pBits; pBits++;
      if((size>16)||(size==0)) {
         pCIM[ascii]->Flag=0;
         size=8;
      }

      pCIM[ascii]->Depth=size;
      pSrc =(char *)pCIM[ascii];
      pSrc =pSrc+8;

      im=MaxHeight;
      if(MaxWidth>8) {
         pBits++;
         im=im*2;
      }

      for(i=0;i<im;i++) {
         *pSrc = *pBits;
         pSrc++; pBits++;
      }
   }

   free(pFnt);  /* not needed anymore */

   DisplayFont();
   OnGrid(Select);
   ConvVFN();     /* convert to HFN */
   return TRUE;
}


/* --------------------------------------------------------------------
                            Load FCL Font
   -------------------------------------------------------------------- */

int LoadFCL(char *filename)
{
   int c,r,len,f=0,width;
   FILE *pF;
   char buffer[110];
   PALWIN *pWin;
   char *pBits;

   if(!(pF = fopen(filename, "rb"))) return FALSE;

   pWin = OpenWin(WS_HP200, 140, 50, 470, 110, "Please wait");
   WriteText(190,80,  "Loading FCL font file...");

   /* erase resident font */
   for(f=0;f<256;f++) {
      pBits=(char *)pCIM[f];
      for(r=0;r<40;r++) {
         *pBits = 0;
          pBits++;
      }
      pCIM[f]->Width=8;
      pCIM[f]->Depth=8;
   }

   f=0;

   while(fgets(buffer, 100, pF)) {

      if((strstr(buffer,"Char"))&&(buffer[0]!=';')) {

         /* get ASCII number/char */
         if(strstr(buffer,"'"))
            sscanf(buffer, "%*[^']'%c", &f);
         else
            sscanf(buffer, "%*s %d", &f);

         if(f>255) f=0;

         /* clear matrix */
         for(r=0;r<16;r++)
            for(c=0;c<16;c++)
               Matrix[c][r]=0;

         /* get bitmap into matrix */
         for(r=0;r<16;r++) {
            if(!(fgets(buffer, 100, pF))) break;
            len=strlen(buffer)-4;

            if(len<4) break;
               else width=len;

            for(c=0;c<len;c++)
               if(buffer[c+2]=='*') Matrix[c][r]=1;

         }

         if(!pCIM[f]->Flag) {      /* undefined ? */
            pCIM[f]->Flag=1;
            pCIM[f]->Width=width;
            pCIM[f]->Depth=r;
            Matrix2Bit(f);
         }

      }
   }

   fclose(pF);
   CloseWin(pWin);
   DisplayFname(filename);
   OnGrid(Select);
   return TRUE;
}


/* --------------------------------------------------------------------
                           Open Font
   -------------------------------------------------------------------- */

void OpenFNT(void)
{
   int istat;
   char fntname[80];

   InitDialog(&OpenFont);
   SetDlgItem(&OpenFont, OPNAME, EDSI_TXT, Opename);
   ShowDialog(&OpenFont, 80, 30, "Open new font");
   ShowFKeys(OKCANLabels);

   do {
      HandleDialog(&OpenFont, &istat);
      ShowFKeys(KeyLabels);

      if((istat==F9_KEY)||(istat==ESCKEY)) {
         CloseDialog(&OpenFont);
         return;
      }
   }while(!((istat==KEYENTER)||(istat==F10_KEY)));

   GetDlgItem(&OpenFont, OPNAME, EDGI_TXT, fntname);
   CloseDialog(&OpenFont);

   strcpy(Opename,fntname); /* save to global var */
   LoadByExt(fntname);
   DisplayFont();
   OnGrid(Select);

}


/* --------------------------------------------------------------------
                        Write FCL character
   -------------------------------------------------------------------- */

void WFCLchar(FILE *dst, int ascii, int type)
{

   int r, c, height, width;
   char binary[20];

   if(type==SMALL_FONT) {
      width=8; height=8;
   }

   if(type==MEDIUM_FONT) {
      width=10; height=11;
   }

   if(type==LARGE_FONT) {
      width=16; height=12;
   }

   Bit2Matrix(ascii);

   fprintf(dst,"\r\nChar %d\r\n",ascii);

   for(r=0;r<height;r++) {
      for(c=0;c<width;c++) {
         if(Matrix[c][r]) binary[c]='*';
         else binary[c]='.';
      }
      binary[c]=0;
      fprintf(dst,"= %s\r\n", binary);
   }
}


/* --------------------------------------------------------------------
                        Write FCL font
   -------------------------------------------------------------------- */

void WriteFCL(char *filename, FILE *dst, int type)
{
   int f;
   char ComName[80];

   fprintf(dst,"\r\n; FCL control file.\r\n\r\n");
   fprintf(dst,"LoadFont \"");

   if(type==SMALL_FONT)
     fprintf(dst,"8x8\"\r\n");

   if(type==MEDIUM_FONT)
     fprintf(dst,"10x11\", CodePage 437\r\n");

   if(type==LARGE_FONT)
     fprintf(dst,"16x12\", CodePage 437\r\n");

   for(f=AscMin;f<=AscMax;f++) {
      if(pCIM[f]->Flag)
         WFCLchar(dst,f,type); /* write character */
   }

   AddExt(ComName,filename,".com");
   fprintf(dst,"\r\nCreate \"%s\"\r\n\r\n",ComName);
}


/* --------------------------------------------------------------------
                            Save HFN header
   -------------------------------------------------------------------- */

void WriteHFN(FILE *dst)
{
   BYTE hi,lo;
   WORD Val;

   /* Write HFN header */
   lo = ftype; hi = ftype >> 8;
   fputc(lo,dst);            /* Font type */
   fputc(hi,dst);

   Val = AscMin;             /* ASCII min character */
   lo = Val; hi = Val >> 8;
   fputc(lo,dst);
   fputc(hi,dst);

   Val = AscMax;             /* ASCII max character */
   lo = Val; hi = Val >> 8;
   fputc(lo,dst);
   fputc(hi,dst);

   Val = MaxWidth;           /* MaxWidth */
   lo = Val; hi = Val >> 8;
   fputc(lo,dst);
   fputc(hi,dst);

   Val = MaxHeight;          /* MaxHeight */
   lo = Val; hi = Val >> 8;
   fputc(lo,dst);
   fputc(hi,dst);

}


/* --------------------------------------------------------------------
                            Save VFN header
   -------------------------------------------------------------------- */

void WriteVFN(FILE *dst)
{
   BYTE hi,lo;
   WORD Val, vbytes;

   /* Write VFN header */
   Val = MaxHeight+1;           /* VR Width */
   lo = Val; hi = Val >> 8;
   fputc(lo,dst);
   fputc(hi,dst);

   vbytes=1;
   if(MaxWidth>8) vbytes=2;

   Val = vbytes   ;          /* Vbytes */
   lo = Val; hi = Val >> 8;
   fputc(lo,dst);
   fputc(hi,dst);


   Val = AscMin;             /* ASCII min character */
   lo = Val; hi = Val >> 8;
   fputc(lo,dst);
   fputc(hi,dst);

   Val = AscMax;             /* ASCII max character */
   lo = Val; hi = Val >> 8;
   fputc(lo,dst);
   fputc(hi,dst);

   lo = ftype; hi = ftype >> 8;
   fputc(lo,dst);            /* is fixed */
   fputc(hi,dst);

}


/* --------------------------------------------------------------------
                            Save Font
   -------------------------------------------------------------------- */

void SaveFont(char *fntname)
{

   FILE *dst;
   int format, istat, flag, PropFlag=1;
   char *fptr;

   BYTE hold;
   char buffer[80];

   WORD Width, Depth, Val, len, b, f;
   PALWIN *pWin;

   strcpy(fntname,FontName);
   AddExt(fntname,FontName,"");

   InitDialog(&SFont);
   SetDlgItem(&SFont, FNAME, EDSI_TXT, fntname);
   itoa(ftype,buffer,10);
   SetDlgItem(&SFont, FONTID, EDSI_TXT, buffer);

   ShowDialog(&SFont, 80, 30, "Save font ?");
   ShowFKeys(OKCANLabels);

   istat = HandleDialog(&SFont, NULL);
   ShowFKeys(KeyLabels);

   if(istat==DN_CANCEL) {
     CloseDialog(&SFont);
     return;
   }

   GetDlgItem(&SFont, FNAME, EDGI_TXT, FontName);
   GetDlgItem(&SFont, FONTID, EDGI_INT, &ftype);

   GetDlgItem(&SFont, HFNTYPE, RAGI_STATE, &flag);
   if(flag) format=72;
   GetDlgItem(&SFont, VFNTYPE, RAGI_STATE, &flag);
   if(flag) format=86;
   GetDlgItem(&SFont, FCLTYPE, RAGI_STATE, &flag);
   if(flag) format=70;

   CloseDialog(&SFont);

   /* Convert into VR font */
   if(format=='V') ConvVFN();

   pWin = OpenWin(WS_HP200, 140, 50, 470, 110, "Attention.");
   WriteText(200,80,  "    Saving Font...");


   /* Find AscMin limit */
   for(f=0;f<256;f++) {
      if(pCIM[f]->Flag) {
         AscMin=f;
         break;
      }
   }

   /* Find AscMax limit */
   for(f=256;f>1;f--) {
      if(pCIM[f]->Flag) {
         AscMax=f;
         break;
      }
   }

   /* fill in empty slots in between if HFN or VFN */
   for(f=AscMin;f<=AscMax;f++) {

      if((!pCIM[f]->Flag)&&(format!='F')) {
         fptr=(char *)pCIM[f];
         for(b=0;b<20;b++) {    /* fill with SPACES */
            *fptr=space[b];
            fptr++;
         }
      }

      if(pCIM[f]->Depth > MaxHeight)
         MaxHeight=pCIM[f]->Depth;
      if(pCIM[f]->Width > MaxWidth)
         MaxWidth=pCIM[f]->Width;
      /* check for VR proportionality */
      if(pCIM[f]->Depth != MaxHeight) PropFlag=0;
   }


   /* CREATE OUTPUT FILES */

   strcpy(buffer,FontName);
   if(format=='F') AddExt(buffer,FontName,".fcl");
   if(format=='H') AddExt(buffer,FontName,".hfn");
   if(format=='V') AddExt(buffer,FontName,".vfn");

   strcpy(FontName,buffer);
   if(!(dst = fopen(FontName,"wb"))) {
      printf("\a");   /* can't save, error */
      CloseWin(pWin);
      return;
   }

   if(format=='F') {      /* FCL output */
      /* check font size */
      format=MEDIUM_FONT;
      if((MaxWidth<9)&&(MaxHeight<9)) format=SMALL_FONT;
      if((MaxWidth>10)&&(MaxHeight>11)) format=LARGE_FONT;

      WriteFCL(FontName,dst,format);
      fclose(dst);
      CloseWin(pWin);
      return;
   }


   /* Write HFN/VFN header */
   if(format=='H') WriteHFN(dst);  /* Write HFN header */

   if(format=='V') {
      ftype=PropFlag;
      WriteVFN(dst);  /* Write VFN header */
   }

   /* Write bitmap to file */

    for(Val=0;Val<256;Val++) {

        fptr=(char *)pCIM[Val];

        if(pCIM[Val]->Flag==1) {
            Width= pCIM[Val]->Width;
            Depth= pCIM[Val]->Depth;
            len=IMGLENGTH(Width,Depth);

            if(format=='H') {
               len=len+8;
               for(b=4;b<len;b++) fputc(*(fptr+b),dst);
            }

            if(format=='V') {
               len=MaxHeight;
               if(MaxWidth>8) len=len*2;
               hold=Depth & 0xff;
               fputc(hold,dst);
               if(MaxWidth>8) fputc(0,dst);
               for(b=8;b<len+8;b++) fputc(*(fptr+b),dst);
            }
        }

    }

    fclose(dst);
    CloseWin(pWin);
    if(format=='V') ConvVFN();   /* convert back to normal */
    Modified=FALSE;
}


/* --------------------------------------------------------------------
                         Load Font by Extension
   -------------------------------------------------------------------- */

void LoadByExt(char *filename)
{

   char buffer[80];

   /* check filename extensions */

   strlwr(filename);
   if(!((strstr(filename,".vfn"))||(strstr(filename,".fcl")))) {
      AddExt(buffer,filename,".hfn");   /* HFN */
      strcpy(filename,buffer);
   }

   if(strstr(filename,".vfn")) {        /* VFN */
      if(!(LoadVfn(filename))) {
         strcpy(filename,"resident.hfn");
         GrabFont(filename);
      }
   }

   if(strstr(filename,".fcl")) {        /* FCL */
       if(!(LoadFCL(filename))) {
         strcpy(filename,"resident.hfn");
         GrabFont(filename);
      }
   }

   if(strstr(filename,".hfn")) {        /* HFN */
      if(!(GrabFont(filename)))
         strcpy(filename,"resident.hfn");
   }

   strcpy(FontName, filename);
   DisplayFname(filename);
}

