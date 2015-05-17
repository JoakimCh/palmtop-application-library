/* --------------------------------------------------------------------
   Project: PAL FONT Editor Vr. 1.02
   Module:  PFE.C
   Author:  Harry Konstas
   Started: 10 May '95
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
#include "pfe.h"
#include "pfers.h"    /* resource header */


/* --------------------------------------------------------------------
                        global variables
   -------------------------------------------------------------------- */

CHARIMG *pCIM[256];   /* bitmap pointer table */
int Select;           /* current selected character */
int Modified;         /* Modified flag */

BYTE Matrix[16][16];  /* bitmap matrix */
BYTE Undo[16][16];    /* Undo matrix   */
BYTE Rot[16][16];     /* rotation matrix */

BYTE Paste[40];       /* Paste buffer  */

char FontName[80];    /* loaded font filename */
char Opename[80];     /* Opened font name */

/* define the function key labels */
char *KeyLabels[10] = {
   "Help", "New", "Edit", "Size", "Copy",
   "Paste", "ISO\x1aIBM", "Rotate", "Open", "Save"
};

char *EditLabels[10] = {
   "Rotate","Invert","Width","Height","Scroll\x1b",
   "Scroll\x1a","Scroll\x18","Scroll\x19",
   "Restore","O.K."
};

char *OKCANLabels[10] = {
   NULL, NULL, NULL, NULL, NULL,
   NULL, NULL, NULL, "Cancel", "O.K."
};


/* --------------------------------------------------------------------
                            Menu structures
   -------------------------------------------------------------------- */

/* the 'Application' pulldown: Items and Menu */
MENUITEM AppsItems[] = {
   { "&New font         F2", MA_VAL, MENUVAL(F2_KEY)  },
   { "&Open font file   F9", MA_VAL, MENUVAL(F9_KEY)  },
   { "&Save font       F10", MA_VAL, MENUVAL(F10_KEY) },
   { "--------------------", MT_SEPARATOR, MENUVAL(0) },
   { "&Font size setup  F4", MA_VAL, MENUVAL(F4_KEY)  },
   { "&Rotate font      F8", MA_VAL, MENUVAL(F8_KEY)  },
   { "&Convert ISO->IBM F7", MA_VAL, MENUVAL(F7_KEY)  },
   { "&Edit character   F3", MA_VAL, MENUVAL(F3_KEY)  },
};
MENU AppsMenu = { NULL, MS_PULLDN, 0,0,8,8, AppsItems };


/* the 'About' pulldown: Items and Menu */
MENUITEM AboutItems[] = {
   { "&Help  F1", MA_VAL, MENUVAL(F1_KEY)  },
   { "&About   ", MA_VAL, MENUVAL(CMD_ABOUT)},
};
MENU AboutMenu = { NULL, MS_PULLDN, 0,0,2,2, AboutItems };


/* the menu bar  */
MENUITEM BarItems[] = {
   { "&Font",         MA_MNU, &AppsMenu  },
   { "&Quit",         MA_VAL, MENUVAL(CMD_QUIT) },
   { "&About",        MA_MNU, &AboutMenu },
};
MENU BarMenu = { NULL, MS_HORIZ|MS_TOPLVL, 0,0,3,3, BarItems };


/* --------------------------------------------------------------------
                                 Initialize
   -------------------------------------------------------------------- */

void Initialize(void)
{
   int c,r;

   ShowTopLine(" PAL Font Editor Vr.1.02", " ");
   ShowFKeys(KeyLabels);

   SelectFont(MEDIUM_FONT);
   SetRule(FORCE_RULE);
   Rectangle(HOFS-10,VOFS-6,188,185,OUTLINE_FILL);
   Rectangle(HOFS-9,VOFS-5,187,184,OUTLINE_FILL);
   Rectangle(HOFS-8,VOFS-4,186,183,OUTLINE_FILL);

   Line(0,9,639,9);
   Line(0,10,639,10);

   /* allocate table & bitmap storage */
   for(c=0;c<256;c++) {
      pCIM[c] = malloc(sizeof(CHARIMG));
      pCIM[c]->Flag=0;   /* undefined */
      pCIM[c]->Bits=1;
      pCIM[c]->Width=8;
      pCIM[c]->Depth=8;
   }

   /* display grid */
   for(r=0;r<16;r++) {
      GraphByte(0,r,0);
      GraphByte(8,r,0);
   }

   /* clear paste buffer */
   for(c=0;c<40;c++) Paste[c]=0;

   Opename[0]=0;         /* nothing opened */
   Select=65;            /* First character = 'A' */
   Modified = FALSE;
   AnnuncPos(AN_LEFT);   /* place annunciators left */

}


/* --------------------------------------------------------------------
                                 Interact
   -------------------------------------------------------------------- */

void Interact(char *filename)
{
   WORD Key, skey;
   int c, r, f;
   char *pBits;

   do {
      Key = GetKey();
      skey = Key;
      AscCursor(Select);
      if(Key & 0xff) Key &= 0xff;

      if(Key == MENU_KEY || Key == AF10_KEY) {
         Key = HandleMenu(&BarMenu, 0, 10);
      }

      switch(Key) {

         case RIGHT_KEY:
            Select++;
            if(Select>255) Select=0;
            break;

         case LEFT_KEY:
            Select--;
            if(Select<0) Select=255;
            break;

         case DOWN_KEY:
            Select+=26;
            if(Select>255) Select=0;
            break;

         case UP_KEY:
            Select-=26;
            if(Select<0) Select=255;
            break;


         case CMD_ABOUT:   About();           break;
         case CMD_QUIT:
            if(Modified) SaveFont(filename);  return;             /* quit */

         case DEL_KEY:     DelChar(Select);   Modified=TRUE; break;

         case F1_KEY:      Help();            break;
         case F2_KEY:      ClearFont();       Modified=TRUE; break;

         case ENTER_KEY:
         case F3_KEY:      EditChar(Select);  Modified=TRUE; break;
         case F4_KEY:      FontSetup(0);      Modified=TRUE; break;

         case F5_KEY:      /* copy */
            if(!pCIM[Select]->Flag) {
               printf("\a");
               break;
            }
            pBits=(char *)pCIM[Select];
            for(f=0;f<40;f++) {
               Paste[f]=*pBits;
               pBits++;
            }
            break;


         case F6_KEY:     /* paste */
            if(!Paste[0]) {
               printf("\a");
               break;
            }
            pBits=(char *)pCIM[Select];
            for(f=0;f<40;f++) {
               *pBits = Paste[f];
               pBits++;
            }

            r=Select/26;
            c=Select%26;
            PutImg(AHOFS+(c*16),AVOFS+(r*16), TXT_RULE, pCIM[Select]);
            OnGrid(Select);
            Modified=TRUE;
            break;


         case F7_KEY:   FromLatin();        Modified=TRUE; break;
         case F8_KEY:   RotateFont();       Modified=TRUE; break;
         case F9_KEY:   OpenFNT();          Modified=TRUE; break;
         case F10_KEY:  SaveFont(filename); break;

         default:
            skey=skey & 0xff;
            if((skey)&&(skey!=27)) Select=skey;
      }

    AscCursor(Select);
    OnGrid(Select);

   } while(Key != ESC_KEY && toupper(Key) != 'Q');

   if(Modified) SaveFont(filename);
}


/* --------------------------------------------------------------------
                                 MAIN
   -------------------------------------------------------------------- */

void main(int argc, char *argv[])
{
   if(!PalInit(1)) FatalExit("Runs only on HP100/200LX...", 1);

   Initialize();

   strcpy(FontName,"resident.hfn");
   if(argc>1) {
      strcpy(FontName,argv[1]);
   }

   LoadByExt(FontName);      /* Load font    */
   DisplayFont();            /* Display font */
   AscCursor(Select);        /* Selector     */

   Interact(FontName);       /* handle user interaction */

   AnnuncPos(AN_RIGHT);      /* place annunciators right*/
   PalDeInit(1);             /*      quit program       */
   printf("PFE: Portions Copyright 1994, The PAL Group.\n");
}


