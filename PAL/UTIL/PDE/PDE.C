/* --------------------------------------------------------------------
   Project: PAL Dialog Editor  Vr. 1.2
   Module:  PDE.C
   Author:  Harry Konstas / Gilles Kohl
   Started: 29. Jan '95
   Subject: Create .C and .H resource files for PAL dialog windows.
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                            standard includes
   -------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


/* --------------------------------------------------------------------
                             local includes
   -------------------------------------------------------------------- */

#include "pal.h"      /* PAL Header file */
#include "pde.h"      /* PDE Header file */


/* --------------------------------------------------------------------
                          Function Key labels
   -------------------------------------------------------------------- */

/* define the function key labels */
char *KeyLabels[10] = {
   "Help", NULL, NULL, NULL, NULL,
    NULL,  NULL, "Quit", "Open", NULL
};

char *KeyLabels2[10] = {
   "Help", "Add", "Edit", NULL, "Size",
   "Sort", NULL,  "Quit", "New", "Save"
};

char *OKCANLabels[10] = {
   NULL, NULL, NULL, NULL, NULL,
   NULL, NULL, NULL, "Cancel", "O.K."
};

char *EditLabels[10] = {
   NULL, NULL, "Edit", "Delete", "Next",
   "Init", "Focus", "Order", "Cancel", "O.K."
};


/* --------------------------------------------------------------------
                            Menu structures
   -------------------------------------------------------------------- */

/* the 'Dialog' pulldown: Items and Menu */
MENUITEM DialogItems[] = {
   { "&Open dialog  F9",   MA_VAL,        MENUVAL(CMD_DEFDIAG) },
   { "&Save dialog F10",   MA_VAL,        MENUVAL(CMD_SAVE)  },
};
MENU DialogMenu = { NULL, MS_PULLDN, 0,0,2,2, DialogItems };


/* the 'Items' pulldown: Items and Menu */
MENUITEM ItemsItems[] = {
   { "&Add    item  F2", MA_VAL, MENUVAL(F2_KEY)  },
   { "&Edit  items  F3", MA_VAL, MENUVAL(F3_KEY)  },
};
MENU ItemsMenu = { NULL, MS_PULLDN, 0,0,2,2, ItemsItems };


/* the 'About' pulldown: Items and Menu */
MENUITEM AboutItems[] = {
   { "&Help  F1", MA_VAL, MENUVAL(F1_KEY)  },
   { "&About   ", MA_VAL, MENUVAL(CMD_ABOUT)},
};
MENU AboutMenu = { NULL, MS_PULLDN, 0,0,2,2, AboutItems };


/* the menu bar  */
MENUITEM BarItems[] = {
   { "&Dialog",       MA_MNU, &DialogMenu     },
   { "&Items",        MA_MNU, &ItemsMenu      },
   { "&About",        MA_MNU, &AboutMenu      },
   { "&Quit",         MA_VAL, MENUVAL(F8_KEY) },
};
MENU BarMenu = { NULL, MS_HORIZ|MS_TOPLVL, 0,0,4,4, BarItems };



/* --------------------------------------------------------------------
                                 Initialize
   -------------------------------------------------------------------- */

DIAGDEF *Initialize(void)
{
   int f;
   DIAGDEF *pDialog;

   /* allocate storage for dialog data */
   pDialog = malloc(sizeof(DIAGDEF));

   for(f=0;f<NUM_OF_ELMNT;f++)
      pDialog->item[f]=NULL;

   pDialog->fname[0] = NULL;
   pDialog->name[0] = NULL;
   pDialog->INum = 0;        /* no items in dialog */
   pDialog->DiagFlag=0;      /* dialog not defined */
   pDialog->Modified=FALSE;  /* not modified (yet) */

   ShowTopLine(" PAL Editor Vr.1.2"," ");
   ClrScrn(ABOUT);
   ShowFKeys(KeyLabels);
   AnnuncPos(AN_LEFT);   /* place annunciators left */

   DispInfo(pDialog, 0, 0, 0, 0, 0);
   return pDialog;
}


/* --------------------------------------------------------------------
                                 Interact
   -------------------------------------------------------------------- */

void Interact(DIAGDEF *pDialog)
{
   WORD Key;

   do {
      Key = GetKey();
      if(Key & 0xff) Key &= 0xff;

      if(Key == MENU_KEY || Key == AF10_KEY) {
         Key = HandleMenu(&BarMenu, 0, 10);
      }

      switch(Key) {

         case CMD_ABOUT:   About();             break;
         case CMD_DEFDIAG: pDialog=DefineDiag(pDialog);
                                                break;
         case CMD_SAVE:    Save(pDialog);       break;

         case F1_KEY:      Help();              break;
         case F9_KEY:      pDialog=DefineDiag(pDialog);
                                                break;
         case F10_KEY:     Save(pDialog);       break;

         case F2_KEY:      NewItem(pDialog);    break;
         case F3_KEY:      EditItems(pDialog);  break;

         case F5_KEY:      DlgWinSize(pDialog); break;
         case F6_KEY:      SortDlg(pDialog);    break;
         case F8_KEY:      if(pDialog->INum>0)
                              Save(pDialog);
                           return;

      }

   } while(Key != ESC_KEY && toupper(Key) != 'Q');
   if(pDialog->Modified) Save(pDialog);
}


/* --------------------------------------------------------------------
                                 MAIN
   -------------------------------------------------------------------- */

void main(int argc, char *argv[])
{
   DIAGDEF *pDialog;

   if(!PalInit(1)) FatalExit("Init failed - CGAGRAPH not loaded ?", 1);

   pDialog = Initialize();

   if(argc>1) {
      if(!LoadDlg(pDialog, argv[1], argv[2]))
         pDialog=DefineDiag(pDialog);
   }

   Interact(pDialog);        /* handle user interaction */

   AnnuncPos(AN_RIGHT);      /* place annunciators right*/
   PalDeInit(1);             /*      quit program       */
}


