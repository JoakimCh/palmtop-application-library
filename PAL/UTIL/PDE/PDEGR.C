/* --------------------------------------------------------------------
   Project: PAL Dialog Editor  Vr. 1.2
   Module:  PDEGR.C
   Author:  Harry Konstas / Gilles Kohl
   Started: 29. Jan '95
   Subject: Graphics control module for PDE.C
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                       standard includes
   -------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined _MSC_VER || defined _QC
#include <malloc.h>
#define coreleft _memavl
#else
#include <alloc.h>
#endif

/* --------------------------------------------------------------------
                         local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "pde.h"


/* --------------------------------------------------------------------
                           variables
   -------------------------------------------------------------------- */

extern char *KeyLabels[10];
extern char *KeyLabels2[10];
extern char *OKCANLabels[10];
extern char *EditLabels[10];

unsigned char RadioBtn[] = {
   0x01, 0x00, 0x01, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x1F, 0x00,
   0x20, 0x80, 0x40, 0x40, 0x80, 0x20, 0x80, 0x20, 0x80, 0x20,
   0x80, 0x20, 0x80, 0x20, 0x40, 0x40, 0x20, 0x80, 0x1F, 0x00,
   0x00, 0x00,
};

unsigned char RadioEdt[] = {
   0x01, 0x00, 0x01, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x1F, 0x00,
   0x20, 0x80, 0x4E, 0x40, 0x9F, 0x20, 0xBF, 0xA0, 0xBF, 0xA0,
   0xBF, 0xA0, 0x9F, 0x20, 0x4E, 0x40, 0x20, 0x80, 0x1F, 0x00,
   0x00, 0x00,
};


/* --------------------------------------------------------------------
                           About window
   -------------------------------------------------------------------- */

void About(void)

{
    PALWIN *pWin;
    pWin  = OpenWin(WS_HP200, 140, 50, 470, 150, "About program");
    WinText(pWin, 35,5,   "         The PAL         ");
    WinText(pWin, 35,25,  "      DIALOG EDITOR          ");
    WinText(pWin, 35,50,  "         (c)1995.        ");
    WinText(pWin, 35,70 , "      The PAL group.");

    GetKey();
    CloseWin(pWin);
}


/* --------------------------------------------------------------------
                                 Help window
   -------------------------------------------------------------------- */

void Help(void)

{
    PALWIN *pWin;
    int x=40,y=8;

    pWin  = OpenWin(WS_HELP, 30, 10, 600, 180, "Help");
    SelectFont(MEDIUM_FONT);

    WinText(pWin, x,y,    "You can use the following key combinations to  ");
    WinText(pWin, x,y+10, "move or change the size of any field item while");
    WinText(pWin, x,y+20, "placing it on the screen:                      ");
    WinText(pWin, x,y+30, "                                               ");
    WinText(pWin, x,y+40, "  KEY | ARROWS ONLY | SHIFT+ARROWS | Fn+ARROWS ");
    WinText(pWin, x,y+50, " -----|-------------|--------------|---------- ");
    WinText(pWin, x,y+60, "  UP  |  move UP    | smooth UP    | height(-) ");
    WinText(pWin, x,y+70, " DOWN |  move DOWN  | smooth DOWN  | height(+) ");
    WinText(pWin, x,y+80, " LEFT |  move LEFT  | smooth LEFT  | width (-) ");
    WinText(pWin, x,y+90, " RIGHT|  move RIGHT | smooth RIGHT | width (+) ");
    WinText(pWin, x,y+100,"            A T T E N T I O N !                ");
    WinText(pWin, x,y+110," ON DESKTOP: Use PGUP/PGDN, HOME/END, and the  ");
    WinText(pWin, x,y+120," numeric keypad arrows for precision alignment.");

    GetKey();
    CloseWin(pWin);
}


/* --------------------------------------------------------------------
                                Clear screen
   -------------------------------------------------------------------- */

void ClrScrn(int type)

{
    SetColor(WHITE_COLOR);
    SetRule(FORCE_RULE);
    Rectangle(0,10,639,190,SOLID_FILL);
    SetColor(BLACK_COLOR);

    if(type==ABOUT) {
      SelectFont(LARGE_FONT);
      WriteText(150,80,"P.A.L. Dialog Editor");
      SelectFont(MEDIUM_FONT);
      Line(150,72,470,72);
      Line(150,73,470,73);
      Line(150,95,470,95);
      Line(150,96,470,96);

      SetRule(TXT_RULE);
      WriteText(160,67,"The");
      WriteText(210,105,"(c)1995 The PAL Group.");
   }
}


/* --------------------------------------------------------------------
                    Display Width, Depth & coordinates
   -------------------------------------------------------------------- */

void DispInfo(DIAGDEF *pDialog, int type, int x1, int y1, int x2, int y2)
{
   char buffer[15];
   int width=x2-x1;
   int depth=y2-y1;

   char *types[] = { "Window", "IhEdit", "IhStatic", "IhButton",
                            "IhCheck","IhRadio", "IhGroup", "IhCombo",
                            "IhBitmap", "IhListBox","Other" };

   if(type>0) {
      /* relative display */
      x1 = x1 - pDialog->x1 - HOFFSET_ADJ;
      y1 = y1 - pDialog->y1 - VOFFSET_ADJ;
   }

   SetRule(TXT_RULE);
   SelectFont(SMALL_FONT);

   sprintf(buffer, "FreeMem:%5u", coreleft());
   WriteText(188,1,buffer);

   sprintf(buffer, "TYPE:%-8s", types[type]);
   WriteText(300,1,buffer);

   sprintf(buffer, "X:%4d", x1);
   WriteText(412,1,buffer);
   sprintf(buffer, "Y:%4d", y1);
   WriteText(468,1,buffer);
   sprintf(buffer,"W:%4d",width);
   WriteText(524,1,buffer);
   sprintf(buffer,"D:%4d",depth);
   WriteText(580,1,buffer);

   SetRule(XOR_RULE);
   SelectFont(MEDIUM_FONT);
   return;
}


/* --------------------------------------------------------------------
                           Move & Size an Item
   -------------------------------------------------------------------- */

void MoveSize(DIAGDEF *pDialog, int key, int mode, int type, int step,
              int *x1, int *y1, int *x2, int *y2)
{
   int width, depth;
   width = *x2-*x1;
   depth = *y2-*y1;

   if(mode==S_MODE) {

      /* SIZE WINDOW */

      switch(key) {
      case UP_KEY:
        *y1-=4; *y2+=4;
        if(*y1<10) *y1=10;
        if(*y2>185) *y2=185;
        break;

      case DOWN_KEY:
        *y1+=4; *y2-=4;
        if(*y2<*y1) {
           *y2+=4;
           *y1-=4;
        }
        break;

      case LEFT_KEY:
         *x1+=4; *x2-=4;
         if(*x2<*x1) {
           *x2+=4;
           *x1-=4;
         }
         break;

      case RIGHT_KEY:
        *x2+=4; *x1-=4;
        if(*x1<0) *x1=0;
        if(*x2>635) *x2=635;
        break;

      case SMDOWN:
        *y1-=1; *y2+=1;
        if(*y1<10) *y1=10;
        if(*y2>185) *y2=185;
        break;

      case SMUP:
        *y1+=1; *y2-=1;
        if(*y2<*y1) {
           *y2+=1;
           *y1-=1;
        }
        break;

      case SMLEFT:
         *x1+=1; *x2-=1;
         if(*x2<*x1) {
           *x2+=1;
           *x1-=1;
         }
         break;

      case SMRIGHT:
        *x2+=1; *x1-=1;
        if(*x1<0) *x1=0;
        if(*x2>635) *x2=635;
        break;
      }

      DispInfo(pDialog, type, *x1, *y1, *x2, *y2);
      return;
   }



   if(mode==MS_MODE) {

      switch(key) {

      /* CHANGE POSITION */

      case UP_KEY: *y1-=10;
        if(*y1<1) *y1=1;
        *y2=*y1+depth;
        break;

      case DOWN_KEY:
        *y2+=10;
        if(*y2>185) *y2=185;
        *y1=*y2-depth;
        break;

      case LEFT_KEY:
        *x1-=10;
        if(*x1<0) *x1=0;
        *x2=*x1+width;
        break;

      case RIGHT_KEY:
        *x2+=10;
        if(*x2>635) *x2=635;
        *x1=*x2-width;
        break;


      /* SMOOTH DISPLACEMENT */

      case SMUP:
         *y1-=1;
         if(*y1<10) *y1=1;
         *y2=*y1+depth;
         break;

      case SMDOWN:
         *y2+=1;
         if(*y2>185) *y2=185;
         *y1=*y2-depth;
         break;

      case SMLEFT:
        *x1-=1;
        if(*x1<0) *x1=0;
        *x2=*x1+width;
        break;

      case SMRIGHT:
         *x2+=1;
         if(*x2>635) *x2=635;
         *x1=*x2-width;
         break;


      /* SIZE AREA */

      case PGUP_KEY:
        *y2-=step;
        if((*y2-*y1)<1) *y2=*y1+step;
        break;

      case PGDN_KEY:
        *y2+=step;
        if(*y2>185) *y2=185;
        break;

      case HOME_KEY:
        *x2-=step;
        if((*x2-*x1)<1) *x2=*x1+step;
        break;

      case END_KEY:
        *x2+=step;
        if(*x2>635) *x2=635;
        break;

      }
   }

   DispInfo(pDialog, type, *x1, *y1, *x2, *y2);
   return;
}


/* --------------------------------------------------------------------
                           Create rectangular frame
   -------------------------------------------------------------------- */

int CreateRectan(DIAGDEF *pDialog, int mode, int *x1, int *y1, int *x2, int *y2)
{
   int key, step;
   step = 2;

   ShowFKeys(OKCANLabels);
   SetRule(XOR_RULE);
   SetLineType(DASHED);
   Rectangle(*x1, *y1, *x2, *y2, OUTLINE_FILL);
   DispInfo(pDialog, S_MODE, *x1, *y1, *x2, *y2);

   while(1) {
      key=GetKey();
      Rectangle(*x1, *y1, *x2, *y2, OUTLINE_FILL);
      MoveSize(pDialog, key, mode, 0, step, x1, y1, x2, y2);
      Rectangle(*x1, *y1, *x2, *y2, OUTLINE_FILL);

      switch(key) {

      case F10_KEY:
      case KEYENTER:
         SetLineType(SOLID);
         Rectangle(*x1, *y1, *x2, *y2, OUTLINE_FILL);
         ShowFKeys(KeyLabels2);
         SetRule(FORCE_RULE);
         return TRUE;

      case F9_KEY:
      case KEYESC:
         SetLineType(SOLID);
         ShowFKeys(KeyLabels);
         SetRule(FORCE_RULE);
         *x1=0;*y1=0;*x2=0;*y2=0;
         return FALSE;

      }
   }
   return 0;
}


/* --------------------------------------------------------------------
                             Place Item
   -------------------------------------------------------------------- */

int PlaceItem(DIAGDEF *pDialog, int ItemNo, int step, int type, int x1,
              int y1, int x2, int y2, char *label)
{
   int key;

   while(1) {
      key=GetKey();

      DisField(type,DASHED, x1, y1, x2, y2, label);
      MoveSize(pDialog, key, MS_MODE, type, step, &x1, &y1, &x2, &y2);

      switch(key) {

      case TAB_KEY:
      case STAB_KEY:
      case KEYENTER:
      case DEL_KEY:
      case F2_KEY:
      case F3_KEY:
      case F4_KEY:
      case F5_KEY:
      case F6_KEY:
      case F7_KEY:
      case F8_KEY:
      case F10_KEY:
         SetLineType(SOLID);
         DisField(type,SOLID, x1, y1, x2, y2, label);
         SetRule(FORCE_RULE);
         pDialog->item[ItemNo]->x1=x1;
         pDialog->item[ItemNo]->y1=y1;
         pDialog->item[ItemNo]->x2=x2;
         pDialog->item[ItemNo]->y2=y2;
         return key;

      case F9_KEY:
      case KEYESC:
         SetLineType(SOLID);
         SetRule(FORCE_RULE);
         return FALSE;

      default: DisField(type,DASHED, x1, y1, x2, y2, label);
               break;
      }
   }
   return 0;
}


/* --------------------------------------------------------------------
                           New Item handler
   -------------------------------------------------------------------- */

void ItemHandler(DIAGDEF *pDialog)
{

   int step, type, labelen, x1, y1, x2, y2;
   char label[60];

   ShowFKeys(OKCANLabels);

   type=pDialog->item[pDialog->INum]->type;
   strcpy(label,pDialog->item[pDialog->INum]->label);
   labelen=(strlen(label)*10);


   /* Give startup positions */

   switch(type) {

      case TEXT:
         x1=100; y1=60; x2=labelen+202; y2=74; step=2; break;

      case LABEL:
         x1=100; y1=100; x2=(x1+strlen(label)*10); y2=110; step=1;
         if(strlen(label)==0) {
            x1=100; y1=100; x2=180; y2=114; step=1;
         }
         break;

      case CHECK:
         x1=100; y1=100; x2=115; y2=110; step=2; break;

      case OPTION:
         x1=100; y1=100; x2=100; y2=100; step=2; break;

      case BUTTON:
         x1=100; y1=100; x2=(x1+strlen(label)*10)+20; y2=125; step=2; break;

      case GROUP:
         x1=100; y1=50; x2=labelen+200; y2=100; step=2; break;

      case COMBO:
         x1=100; y1=100; x2=labelen+202; y2=114; step=2; break;

      case BITMAP:
         x1=100; y1=50; x2=180; y2=100; step=1; break;

      case LISTBOX:
         x1=150; y1=40; x2=500; y2=150; step=1; break;

      case OTHER:
         x1=100; y1=50; x2=140; y2=80; step=1; break;

   }


   /* Display & move them on the screen */

   DisField(type,DASHED, x1, y1, x2, y2, label);
   DispInfo(pDialog, type, x1, y1, x2, y2);

   if(!(PlaceItem(pDialog, pDialog->INum, step, type, x1, y1, x2, y2, label))) {
      free(pDialog->item[pDialog->INum]);
      pDialog->INum--;
   }

   ShowFKeys(KeyLabels2);
   return;

}


/* --------------------------------------------------------------------
                             Display Item
   -------------------------------------------------------------------- */

void DisField(int type, int style, int x1, int y1,
              int x2, int y2, char *label)
{
   int labelen;
   IMGHDR *pImg;

   pImg = (IMGHDR*)RadioBtn;
   if(style==DASHED)
      pImg = (IMGHDR*)RadioEdt;

   SetRule(XOR_RULE);
   SetLineType(style);
   SelectFont(MEDIUM_FONT);

   labelen=(strlen(label)*10);

   switch(type) {

   case TEXT:
      LabelOut(x1,y1+2,XOR_RULE,MEDIUM_FONT,label);
      Rectangle(x1+labelen,y1,x2,y2,OUTLINE_FILL);
      break;

   case LABEL:
      Rectangle(x1-1,y1-1,x2,y2-1,OUTLINE_FILL);
      WriteText(x1,y1,label);
      break;

   case BUTTON:
      Rectangle(x1,y1,x2,y2,OUTLINE_FILL);
      Rectangle(x1+1,y1+1,x2-1,y2-1,OUTLINE_FILL);
      LabelOut(x1+10,y1+8,XOR_RULE,MEDIUM_FONT,label);
      break;

   case CHECK:
      Rectangle(x1,y1,x1+13,y1+10,OUTLINE_FILL);
      LabelOut(x1+15,y1+1,XOR_RULE,MEDIUM_FONT,label);
      break;

   case OPTION:
      PutImg(x1, y1-5, XOR_RULE, pImg);
      LabelOut(x1+15,y1-4,XOR_RULE,MEDIUM_FONT,label);
      break;

   case GROUP:
      Rectangle(x1,y1+5,x2,y2+1,OUTLINE_FILL);
      Line(x1+10,y1+5,x1+12+labelen,y1+5);
      WriteText(x1+12,y1,label);
      break;

   case COMBO:
      LabelOut(x1,y1+2,XOR_RULE,MEDIUM_FONT,label);
      Rectangle(x1+labelen,y1,x2,y2,OUTLINE_FILL);
      WriteText(x2+1,y1,"\x19");
      RevBlock(x2+1,y1,x2+11,y2);
      break;

   case BITMAP:
      Rectangle(x1,y1,x2,y2,OUTLINE_FILL);
      if(strlen(label)<2) WriteText(x1+10,y1+10,"BitMap");
      else WriteText(x1+10,y1+10, label);
      break;

   case LISTBOX:
      Rectangle(x1,y1,x2,y2,OUTLINE_FILL);
      if(strlen(label)<2) WriteText(x1+10,y1+10,"ListBox");
      else WriteText(x1+10,y1+10, label);
      break;

   case OTHER:
      Rectangle(x1,y1,x2,y2,OUTLINE_FILL);
      Line(x1,y1,x2,y2);
      Line(x2,y1,x1,y2);
      break;

   }

   SetRule(FORCE_RULE);
}


/* --------------------------------------------------------------------
                             Display Loaded Dialog
   -------------------------------------------------------------------- */

void DisplayDlg(DIAGDEF *pDialog)
{
   PALWIN *pWin;

   int f, type, x1, x2, y1, y2;

   x1 = pDialog->x1;
   x2 = pDialog->x2;
   y1 = pDialog->y1;
   y2 = pDialog->y2;

   pWin  = OpenWin(WS_HP200, x1, y1, x2, y2, pDialog->name);
   /* static window, free memory */
   free(pWin->Buffer);
   free(pWin);

   for(f=1;f<pDialog->INum+1;f++) {

      x1 = pDialog->item[f]->x1;
      x2 = pDialog->item[f]->x2;
      y1 = pDialog->item[f]->y1;
      y2 = pDialog->item[f]->y2;
      type = pDialog->item[f]->type;

      DisField(type, SOLID, x1, y1, x2, y2, pDialog->item[f]->label);
   }

}


/* --------------------------------------------------------------------
                             Bubble Sort
   -------------------------------------------------------------------- */

void Bubble(DIAGDEF *pDialog)
{
   char *pSwap;
   int  swap, limit, f;

   limit=pDialog->INum;
   do {
      swap=0;

      for(f=1;f<limit;f++) {

         /* sort by YPOS first */
         if(pDialog->item[f]->y1 > pDialog->item[f+1]->y1) {

            /* swap pointers */
            pSwap=(char *)pDialog->item[f];
            pDialog->item[f]=pDialog->item[f+1];
            pDialog->item[f+1]=(void *)pSwap;
            swap=f;
         }

         /* if YPOS are equal, sort by XPOS */
         if(pDialog->item[f]->y1 == pDialog->item[f+1]->y1) {
            if(pDialog->item[f]->x1 > pDialog->item[f+1]->x1) {

               /* swap pointers */
               pSwap=(char *)pDialog->item[f];
               pDialog->item[f]=pDialog->item[f+1];
               pDialog->item[f+1]=(void *)pSwap;
               swap=f;
            }
         }


      }
      limit=swap;
   }while (swap);
}


/* --------------------------------------------------------------------
                  Change item order in dialog
   -------------------------------------------------------------------- */

void ChangePos(DIAGDEF *pDialog, int src, int dst)
{
   int f, n=1;
   char *pHold[NUM_OF_ELMNT+4];

   if((src==0)||(dst==0)||(src==dst)) return;  /* invalid indexes */

   /* create new pointer table */
   for(f=1;f<pDialog->INum+1;f++) {
      if(f==dst) {
         pHold[n++]=(char *)pDialog->item[src];
         pHold[n++]=(char *)pDialog->item[f];
      }
      else 
         if(f!=src) pHold[n++]=(char *)pDialog->item[f];
   }

   /* copy back pointer table */
   for(f=1;f<pDialog->INum+1;f++)
      pDialog->item[f]=(void *)pHold[f];
}

