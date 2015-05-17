/* --------------------------------------------------------------------
   Project: PAL Icon viewer
   Module:  ICNVU.C
   Author:  Gilles Kohl
   Started: Dec 26, 1994
   Subject: A sample PAL application: view icons
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                       standard includes
   -------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* --------------------------------------------------------------------
                         msdos includes
   -------------------------------------------------------------------- */

#if defined _MSC_VER || defined _QC
#include <dos.h>
#include <direct.h>
#define findfirst(a,b,c) _dos_findfirst(a,c,b)
#define findnext(a) _dos_findnext(a)
#define ffblk find_t
#define ff_name name
#else
#include <dir.h>
#endif
#include <io.h>
#include <process.h>

/* --------------------------------------------------------------------
                         local includes
   -------------------------------------------------------------------- */
#include "pal.h"

/* --------------------------------------------------------------------
                      constant definitions
   -------------------------------------------------------------------- */
#define MAXICONS 200  /* maximum number of icons to load             */
#define XOFF      3   /* horizontal position of entire icon array    */
#define YOFF     16   /* vertical position of entire icon array      */
#define ICNXOFF  16   /* horizontal position of icon inside box      */
#define ICNYOFF   6   /* vertical position of icon inside box        */
#define BOXWIDTH 76   /* width of icon box, with text and frame      */
#define BOXDEPTH 56   /* depth of icon box, with text and frame      */
#define ICNCOLS   8   /* total number of horizontal icons to display */
#define ICNROWS   3   /* number of rows of icons to display          */

/* --------------------------------------------------------------------
                        global variables
   -------------------------------------------------------------------- */
extern unsigned char ResidentFont[]; /* 'emergency' font */

int   IcnCount = 0;            /* number of icons loaded     */
int   CurIcn   = 0;            /* currently selected icon    */
int   TopIcn   = 0;            /* very first icon on screen  */
int   IconFont = SMALL_FONT;   /* font to use for legends    */
char *IcnNames[MAXICONS];      /* array of icon pathnames    */

/* define the function key labels */
char *KeyLabels[10] = {
   "Help", "New", "Edit", "Delete",  "Rescan",
    NULL,   NULL,  NULL,   NULL,     "Quit"
};

/* the 'File' pulldown: Items and Menu */
MENUITEM FileItems[] = {
   { " &About ... ",  MA_VAL,    MENUVAL(F1_KEY)  }, /* same as help */
   { " &New ",        MA_VAL,    MENUVAL(F2_KEY)  },
   { " &Edit ",       MA_VAL,    MENUVAL(F3_KEY)  },
   { " &Delete ... ", MA_VAL,    MENUVAL(F4_KEY)  },
   { " &Rescan ",     MA_VAL,    MENUVAL(F5_KEY)  },
   { " E&xit ",       MA_VAL,    MENUVAL(F10_KEY) },
};
MENU FileMenu = { NULL, MS_PULLDN, 0, 0, 6, 6, FileItems };

/* The menu bar itself, branching off to the submenues */
MENUITEM BarItems[] = {
   { "&File",         MA_MNU, &FileMenu        },
   { "&Quit",         MA_VAL, MENUVAL(F10_KEY) },
   { "&Help",         MA_VAL, MENUVAL(F1_KEY)  },
};
MENU BarMenu = { NULL, MS_HORIZ|MS_TOPLVL, 0, 0, 3, 3, BarItems };


/* --------------------------------------------------------------------
                           functions
   -------------------------------------------------------------------- */

/*
 * Load and display the help IMG file
 *
 */
void ShowHelp(void)
{
   int w, d, x1, y1, x2, y2;
   IMGHDR *pHelp;
   PALWIN *pWin;

   pHelp = LoadImg("HELP.IMG");
   if(!pHelp) return;
   w = pHelp->Width;
   d = pHelp->Depth;

   x1 = (MAX_X - w)/2;
   y1 = (MAX_Y - d)/2;
   x2 = x1 + w + 4;
   y2 = y1 + d + 12;
   pWin = OpenWin(WS_HP200, x1, y1, x2, y2, " ICNVU Help ");
   PutImg(pWin->PosX, pWin->PosY, FORCE_RULE, pHelp);
   GetKey();
   CloseWin(pWin);
   free(pHelp);
}

/*
 * Load the names of all the icons in the icon directories specified
 * in the config file
 *
 */
void FindIcons(void)
{
   int i;
   struct ffblk ffb;
   char Path[512];
   char *s;

   /* discard names eventually loaded */
   for(i = 0; i < IcnCount; i++) free(IcnNames[i]);
   IcnCount = 0;

   s = GetConfigString("Settings", "IconPath", NULL);
   if(!s) FatalExit("No IconPath under [Settings] in ICNVU.CFG", 1);
   strcpy(Path, s);

   /* scan all directories */
   for(s = strtok(Path, ";"); s; s = strtok(NULL, ";")) {
      char Dir[64];

      strcpy(Dir, s);
      if(Dir[strlen(Dir)-1] != '\\') strcat(Dir, "\\");
      strcat(Dir, "*.ICN");

      /* scan all *.ICN in this directory */
      if(findfirst(Dir, &ffb, 0) == 0) do {
         char Name[256];

         if(IcnCount >= MAXICONS) continue; /* ignore if too many icons */
         if(*ffb.ff_name == '.') continue; /* skip . and .. */
         strcpy(Name, Dir);
         strlwr(ffb.ff_name+1);
         strcpy(strrchr(Name, '\\')+1, ffb.ff_name);
         IcnNames[IcnCount++] = strdup(Name);
      } while(findnext(&ffb) == 0);
   }
   if(IcnCount == 0) FatalExit("No icons found", 1);
}

/*
 * Show Icon number n
 *
 */
void ShowIcon(int n)
{
   int r = (n-TopIcn)/ICNCOLS;
   int c = (n-TopIcn)%ICNCOLS;
   int x, y, l;
   char *s;
   IMGHDR *pIcon;

   SetColor(WHITE_COLOR);
   Rectangle(XOFF+BOXWIDTH*c,       YOFF+BOXDEPTH*r,
             XOFF+BOXWIDTH*(c+1)-1, YOFF+BOXDEPTH*(r+1)-1, SOLID_FILL);

   if(n >= IcnCount) return;   /* skip if we reached end of list */
   pIcon = LoadImg(IcnNames[n]); /* temporarily load icon ... */
   x = XOFF+BOXWIDTH*c;
   y = YOFF+BOXDEPTH*r;
   PutImg(x+ICNXOFF, y+ICNYOFF, FORCE_RULE, pIcon); /* display ... */
   free(pIcon);                  /* and discard icon again */

   /* now compute icon legend - drop the path part */
   s = strrchr(IcnNames[n], '\\');
   s = s ? s+1 : IcnNames[n]; /* if no backslash, use full name */

   l = TextExt(IconFont, s); /* get text extent for positionning */
   SetColor(BLACK_COLOR);
   TextOut(x+(BOXWIDTH-l)/2,  y+ICNYOFF+33, TXT_RULE, IconFont, s);
}

/*
 * Show a screenful of icons
 *
 */
void ShowIcons(void)
{
   int r, c;

   for(r = 0; r < ICNROWS; r++) {
      for(c = 0; c < ICNCOLS; c++) ShowIcon(TopIcn + r*ICNCOLS+c);
   }
}

/*
 * Frame the current icon
 *
 */
void ShowCurrent(void)
{
   int x1, y1, x2, y2;
   int r = (CurIcn-TopIcn)/ICNCOLS;
   int c = (CurIcn-TopIcn)%ICNCOLS;

   SetColor(BLACK_COLOR);
   SetRule(XOR_RULE);
   x1 = XOFF+BOXWIDTH*c;
   y1 = YOFF+BOXDEPTH*r;
   x2 = x1 + BOXWIDTH-1;
   y2 = y1 + BOXDEPTH-1;
   Rectangle(x1, y1, x2, y2, OUTLINE_FILL);
   Rectangle(x1+1, y1+1, x2-1, y2-1, OUTLINE_FILL);
}

/*
 * Call external program to edit current icon
 *
 */
void EditCurrent(int New)
{
   IMGHDR *pScreen;
   char *IcnEdit;
   int Size;

   /* compute size of full screen */
   Size = IMGSIZE(0, 0, MAX_X-1, MAX_Y-1);
   pScreen = malloc(Size);
   if(!pScreen) return; /* if that much mem not available, ignore request */

   /* get path of icon editor - default to ROM version of 200LX */
   IcnEdit = GetConfigString("Settings", "IconEdit", "d:\\bin\\icn200lx.exe");
   if(access(IcnEdit, 0)) return; /* if not available, bail out */

   GetImg(0, 0, MAX_X-1, MAX_Y-1, pScreen); /* save screen */

   /* shell to icon editor */
   spawnl(P_WAIT, IcnEdit, IcnEdit, New ? NULL : IcnNames[CurIcn], NULL);

   GraphMode(); /* switch back to graphics mode ... */
   PutImg(0, 0, FORCE_RULE, pScreen); /* restore saved screen ... */
   free(pScreen); /* and free memory that it used */
}

/*
 * Show current icon, wait for a keystroke and handle it
 *
 */
void Interact(void)
{
   char buf[80];
   WORD Key;

   do {
      sprintf(buf, "IcnVu: %s", IcnNames[CurIcn]);
      ShowTopTime(buf, 1); /* show title and current icon with full path */
      ShowCurrent();
      Key = GetKey();
      if(Key & 0xff) Key &= 0xff;
      ShowCurrent();

      if(Key == MENU_KEY || Key == AF10_KEY) {
         Key = HandleMenu(&BarMenu, 0, 10);
      }
      switch(Key) {
         case DOWN_KEY:
            if(CurIcn+ICNCOLS < IcnCount) CurIcn += ICNCOLS;
            break;
         case UP_KEY:
            if(CurIcn-ICNCOLS >= 0) CurIcn -= ICNCOLS;
            break;
         case LEFT_KEY:
            if(CurIcn > 0) --CurIcn;
            break;
         case RIGHT_KEY:
            if(CurIcn+1 < IcnCount) ++CurIcn;
            break;
         case F1_KEY:
            ShowHelp();
            break;
         case F2_KEY: /* F2 is a special case of F3 */
         case F3_KEY:
         case ENTER_KEY:
            EditCurrent(Key == F2_KEY);
            ShowIcon(CurIcn);
            break;
         case DEL_KEY:
         case F4_KEY:
            if(AskYesNo("Delete Icon ?")) unlink(IcnNames[CurIcn]);
            else break;
            /* if user chose to delete an icon, fall thru to rescan */
         case F5_KEY:
            FindIcons();
            if(CurIcn >= IcnCount) CurIcn = IcnCount-1;
            ShowIcons();
            break;
      }
      if(CurIcn >= TopIcn+ICNROWS*ICNCOLS) {
         TopIcn += ICNCOLS;
         ShowIcons();
      }
      if(CurIcn < TopIcn) {
         TopIcn -= ICNCOLS;
         ShowIcons();
      }
   } while(Key != ESC_KEY && Key != F10_KEY && toupper(Key) != 'Q');
}

/*
 * Load font to use for icon legends
 *
 */
int LoadIconFont(void)
{
   int Font;
   char *FontName;

   /* get name of font to load */
   FontName = GetConfigString("Settings", "IconFont", "SPAB0015.HFN");

   /* First try to get font specified in CFG file, or default font */
   Font = LoadFont(FontName, 0);

   /* If that failed, try to activate resident font */
   if(!Font) {
      Font = LoadFont(ResidentFont, 1);

      /* Panic if that fails too */
      if(!Font) FatalExit("Load resident font failed.", 1);
   }
   return Font;
}

void main(int argc, char *argv[])
{
   ReadMyConfig(argv[0]);
   if(!PalInit(1)) FatalExit("Init failed - CGAGRAPH not loaded ?", 1);

   ShowFKeys(KeyLabels);
   IconFont = LoadIconFont();
   FindIcons();
   ShowIcons();
   Interact();
   PalDeInit(1);
}

