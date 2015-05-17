/* --------------------------------------------------------------------
   Project: PAL menu function test
   Module:  MENU.C
   Author:  Gilles Kohl
   Started: Nov. 12, 1994
   Subject: Exercise some of the PAL menu functions
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                       standard includes
   -------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------
                         local includes
   -------------------------------------------------------------------- */
#include "pal.h"

/* Define a window with the largest possible border, for test */
#define WS_TEST (WS_SHADOW|WS_BMASK|WS_SAVEBG|WS_SMLFONT)

/* --------------------------------------------------------------------
                         type definitions
   -------------------------------------------------------------------- */

/* let the compiler take care of CMD numbering with an enum */
typedef enum {
   CMD_ABOUT = 1, /* zero is reserved in menues */
   CMD_QUIT, CMD_VALUE, CMD_DYNAMENU, CMD_TOGGLE,
   CMD_WINHP100, CMD_WINHP200, CMD_WINHELP, CMD_WINTEST,
} MENUCOMMANDS;

/* --------------------------------------------------------------------
                        local prototypes
   -------------------------------------------------------------------- */
int ChangeIcon(MENU *pMnu);
int MenuCallback(MENU *pMnu);

/* --------------------------------------------------------------------
                  The Menues and their Items
   -------------------------------------------------------------------- */

/* This is a sample menu that will be called from within a pulldown */
MENUITEM SampleItems[] = {
   { "Return 100",  MA_VAL, MENUVAL(100) },
   { "Return 200",  MA_VAL, MENUVAL(200) },
   { "Return 300",  MA_VAL, MENUVAL(300) },
};
MENU SampleMenu[] = { NULL, 0, 0, 0, 3, 3, SampleItems };

/* the 'File' pulldown: Items and Menu */
MENUITEM FileItems[] = {
   { " About ... ",  MA_VAL,    MENUVAL(CMD_ABOUT)      },
   { " &Quit  Exit",       MA_VAL,    MENUVAL(CMD_QUIT)       },
   /* Note: the next item is of type 'bitmap'. It will be dynamically
      loaded - although it could also refer to a resident bitmap */
   {   NULL,         MT_BMP|MA_VAL,    MENUVAL(CMD_QUIT)       },
};
MENU FileMenu = { NULL, MS_PULLDN, 0, 0, 3, 3, FileItems };

/* The 'Menu' pulldown: Items and Menu */
MENUITEM MenuItems[] = {
   { "Type &VALUE",      MA_VAL, MENUVAL(CMD_VALUE)      },
   { "Type &CALLFUNC",   MA_FUN, MenuCallback            },
   { "Separ", MT_SEPARATOR | MA_FUN, MenuCallback       },
   { "Type C&ALLMENU",   MA_MNU, SampleMenu              },
   { "Type &Dynamical",  MA_VAL, MENUVAL(CMD_DYNAMENU)   },
   { "&Toggle MenuBar",  MA_VAL, MENUVAL(CMD_TOGGLE)     },
};
MENU MenuesMenu = { NULL, MS_PULLDN, 0, 0, 6, 6, MenuItems };
/* oops - sorry about that name, but it's consistent with the rest */

/* the 'Windows' pulldown: Items and Menu */
MENUITEM WindowItems[] = {
   { "Style HP100LX",   MA_VAL,    MENUVAL(CMD_WINHP100)   },
   { "Style HP200LX",   MA_VAL,    MENUVAL(CMD_WINHP200)   },
   { "Style Help",      MA_VAL,    MENUVAL(CMD_WINHELP)    },
   { "Style Test",      MA_VAL,    MENUVAL(CMD_WINTEST)    },
};
MENU WindowsMenu = { NULL, MS_PULLDN, 0, 0, 4, 4, WindowItems };

/* Finally, the menu bar itself, branching off to the four submenues */
MENUITEM BarItems[] = {
   { "&File",               MA_MNU, &FileMenu               },
   { "&Menues",             MA_MNU, &MenuesMenu             },
   { "Separ", MT_SEPARATOR | MA_FUN, &MenuCallback           },
   { "&Windows",            MA_MNU, &WindowsMenu            },
};
MENU BarMenu = { NULL, MS_HORIZ|MS_TOPLVL, 0, 0, 4, 4, BarItems };


/* --------------------------------------------------------------------
                           Functions
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
   MessageBox:
   Output a small notification window with a title and a message in a
   given font. I'll add something like this to PAL  but with multiline
   support - this is just a quick shot.
   -------------------------------------------------------------------- */
void MessageBox(char * Title, char *Str, int Font)
{
   PALWIN *pWin;
   int Width, Depth, PosX, PosY;


   Width = max(strlen(Str), strlen(Title)) * FNTW(Font)+8;
   Depth = 2*FNTD(Font)+8;
   PosX  = (MAX_X - Width)/2;
   PosY  = (MAX_Y - Depth)/2;
   pWin  = OpenWin(WS_HP200, PosX, PosY, PosX+Width, PosY+Depth, Title);
   SelectFont(Font);
   WinText(pWin, 2, 2, Str);
   GetKey(); /* wait for a keystroke */
   CloseWin(pWin);
}

/* --------------------------------------------------------------------
   MenuCallback:
   This is a sample callback function that doesn't do much except
   saying that it got called.
   The pointer to the menu can be used to determine from where the
   routine got called, and eventually to modify the menu item itself.
   Return MSC_STAY if you want the menu to stay open, return a
   positive integer if you want the menu to close returning that
   integer to the (toplevel) routine that called 'HandleMenu'.
   -------------------------------------------------------------------- */
int MenuCallback(MENU *pMenu) {
   (void)pMenu; /* dummy: suppress compiler warning */

   MessageBox("Hi there", "Greetings from MenuCallback", MEDIUM_FONT);
   return MSC_STAY;
}

/* --------------------------------------------------------------------
   ShowRetval:
   Utility function - simply display a return value in a window.
   -------------------------------------------------------------------- */
void ShowRetval(int RetVal)
{
   char buf[80];

   sprintf(buf, " HandleMenu returned %d ", RetVal);
   MessageBox("Note", buf, MEDIUM_FONT);
}

/* --------------------------------------------------------------------
   AboutWindow:
   Utility function - open a window and display some text.
   The WinText usage is unsatisfactory - we'll need much better
   support for window output in PAL. There'll be a WinPrintf style
   function soon.
   -------------------------------------------------------------------- */
void AboutWindow(void)
{
   PALWIN *pWin;
   int Font = MEDIUM_FONT;
   int PosX = 12;
   int PosY = 12;

   pWin = OpenWin(WS_HP200, 160, 50, 460, 130,
                  " About PALTEST ");
   WinText(pWin, PosX, PosY, "  A PAL test program ");  PosY += FNTD(Font)+2;
   WinText(pWin, PosX, PosY, " -------------------- "); PosY += FNTD(Font)+2;
   WinText(pWin, PosX, PosY, " G. Kohl, November 94 "); PosY += FNTD(Font)+2;
   WinText(pWin, PosX, PosY, " -------------------- "); PosY += FNTD(Font)+2;
   GetKey();
   CloseWin(pWin);
}

/* --------------------------------------------------------------------
   ShowStyle:
   Utility function - open a window with the given style and given
   caption (will simply describe the style)
   -------------------------------------------------------------------- */
void ShowStyle(WORD Style, char *Str)
{
   PALWIN *pWin;

   pWin = OpenWin(Style, 100, 50, 300, 120, Str);
   SetRule(XOR_RULE);
   Line(pWin->PosX, pWin->PosY,
        pWin->PosX+pWin->Width-1, pWin->PosY+pWin->Depth-1);
   Line(pWin->PosX+pWin->Width-1, pWin->PosY,
        pWin->PosX, pWin->PosY+pWin->Depth-1);
   GetKey();
   CloseWin(pWin);
}

/* --------------------------------------------------------------------
   ShowDynaMenu:
   Utility function to demonstrate dynamic construction of a menu.
   We create a scrolling menu that has four visible items, but
   more hidden ones that appear when the users scrolls.
   -------------------------------------------------------------------- */
void ShowDynaMenu(void)
{
   MENU DynaMenu;
   MENUITEM *pItems;
   int ItemCount = 10;
   char buf[40];
   int i;

   /* allocate storage for ItemCount items */
   pItems = malloc(ItemCount * sizeof(MENUITEM));

   /* now initialize the items */
   for(i = 0; i < ItemCount; i++) {
      sprintf(buf, "Item #%d", i+1001);
      pItems[i].Text       = strdup(buf);
      pItems[i].Flags      = MA_VAL;
      pItems[i].ActionPtr  = MENUVAL(i+1001);
   }

   DynaMenu.pMnuWin  = NULL;

   DynaMenu.Flags    = 0;
   DynaMenu.CurItem  = 0;
   DynaMenu.TopItem  = 0;
   DynaMenu.ItmDisp  = min(ItemCount, 4);
   DynaMenu.ItmCount = ItemCount;
   DynaMenu.Items    = pItems;
   ShowRetval(HandleMenu(&DynaMenu, 100, 50));

   /* Now free the item strings ... */
   for(i = 0; i < ItemCount; i++) free(pItems[i].Text);
   free(pItems); /* and the storage for the items */
}

void main(void)
{
   int RetVal;
   IMGHDR *pImg;

   if(!PalInit(1)) FatalExit("Init failed - CGAGRAPH not loaded ?", 1);

   /* Replace resident icon with a dynamically loaded one */
   if(!(pImg = LoadImg("VR.ICN"))) FatalExit("Load icon failed", 1);
   FileItems[2].Text = (char *)pImg;

   MessageBox("Welcome !", "... to the PAL test ", MEDIUM_FONT);
   do {
      RetVal =  HandleMenu(&BarMenu, 10, 10);
      ShowRetval(RetVal); /* this is for testing only */
      switch(RetVal) {
         case CMD_ABOUT:    AboutWindow();                   break;
         case CMD_WINHP100: ShowStyle(WS_HP100, "WS_HP100"); break;
         case CMD_WINHP200: ShowStyle(WS_HP200, "WS_HP200"); break;
         case CMD_WINHELP:  ShowStyle(WS_HELP,  "WS_HELP");  break;
         case CMD_WINTEST:  ShowStyle(WS_TEST,  "WS_TEST");  break;
         case CMD_TOGGLE:   BarMenu.Flags ^= MS_HORIZ;       break;
         case CMD_DYNAMENU: ShowDynaMenu();                  break;
      }
   } while(RetVal != CMD_QUIT);

   MessageBox(" Bye PAL ! ... ", " CMD_QUIT detected ", MEDIUM_FONT);
   PalDeInit(1);
}

