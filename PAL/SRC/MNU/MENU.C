/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  MENU.C
   Author:  Gilles Kohl/Andreas Garzotto
   Started: 10. Nov. 94
   Subject: Menu routines
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
#include "pal.h"
#include "palpriv.h"

#define ARROW_W 8
#define ARROW_H 8

unsigned char ArrowDn[] = {
   0x01, 0x00, 0x01, 0x00, 0x08, 0x00, 0x08, 0x00, 0xFF, 0xE7,
   0xE7, 0xE7, 0x81, 0xC3, 0xE7, 0xFF,
};

unsigned char ArrowUp[] = {
   0x01, 0x00, 0x01, 0x00, 0x08, 0x00, 0x08, 0x00, 0xFF, 0xE7,
   0xC3, 0x81, 0xE7, 0xE7, 0xE7, 0xFF,
};

/* --------------------------------------------------------------------
                           Functions
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
   GetItemExt:
   Will return the width and depth of a menu item given a Menu and
   an item number.
   (Internal utility, not exported)
   -------------------------------------------------------------------- */
static void GetItemExt(MENU *pMenu, int Item, int *pWidth, int *pDepth)
{
   MENUITEM *pMnuItm;
   PALWIN *pMnuWin   = pMenu->pMnuWin;

   int FontId   = pMnuWin ? pMnuWin->Font : MEDIUM_FONT;
   int FntWidth;
   int FntDepth;
   int w, d;

   FntWidth = FNTW(FontId);
   FntDepth = FNTD(FontId);
   pMnuItm = &pMenu->Items[Item];

   switch(pMnuItm->Flags & MI_TYPMSK) {
      case MT_TXT:
         w = LabelExt(FontId, pMnuItm->Text)+4;
         d = FntDepth+6;
         if(pMenu->Flags & MS_HORIZ) w += 20;
         else                        w += 40;
         break;
      case MT_SEP:
         w = (pMenu->Flags & MS_HORIZ) ? 5 : 0;
         d = (pMenu->Flags & MS_HORIZ) ? 0 : 5;
         break;
      case MT_BMP:
         w = ((IMGHDR *)pMnuItm->Text)->Width;
         d = ((IMGHDR *)pMnuItm->Text)->Depth;
         break;
   }

   *pWidth = w;
   *pDepth = d;
}

/* --------------------------------------------------------------------
   CreateMenuWin:
   Create the window for a menu, will store the window pointer into
   the menu structure. The window will be created at PosX, PosY.
   Computing the extent of the window is the main work of this routine.
   (Internal utility, not exported)
   -------------------------------------------------------------------- */
void CreateMenuWin(MENU *pMenu, int PosX, int PosY)
{
   int i;
   int Depth, Width;
   int d, w;

   pMenu->pMnuWin = NULL;
   /* compute Depth and Width of the Menu */
   for(Depth = 0, Width = 0, i = 0; i < pMenu->ItmCount; i++) {
      GetItemExt(pMenu, i, &w, &d);
      if(pMenu->Flags & MS_HORIZ) {
         if(i < pMenu->ItmDisp) Width += w;
         if(Depth < d) Depth =  d;
      }
      else {
         if(i < pMenu->ItmDisp) Depth += d;
         if(Width < w) Width = w;
      }
   }

   /* add space for scrollbar if needed */

   if (!(pMenu->Flags & MS_HORIZ) &&
        ((pMenu->TopItem) || (pMenu->ItmDisp != pMenu->ItmCount)))
       Width += ARROW_W + 1;

   if(pMenu->Flags & MS_TOPLVL) {
      if(pMenu->Flags & MS_HORIZ) Width = MAX_X-2-SHADOWWIDTH-PosX;
      else                        Depth = MAX_Y-2-SHADOWDEPTH-PosY;
   }

   /* reposition window if required and possible */
   if(PosX+Width+1+SHADOWWIDTH >= MAX_X) {
      PosX = MAX_X-SHADOWWIDTH-2 - Width;
      if(PosX < 0) PosX = 0;
   }
   if(PosY+Depth+1+SHADOWDEPTH >= MAX_Y) {
      PosY = MAX_Y-SHADOWDEPTH-2 - Depth;
      if(PosY < 0) PosY = 0;
   }

   /* Now open a window for our menu */
   pMenu->pMnuWin = OpenWin(WS_MENU, PosX, PosY,
                            PosX+Width+1, PosY+Depth+1, NULL);
}


/* --------------------------------------------------------------------
   GetItemDim:
   Will return the coordinates of a menu item (Upper left/lower right)
   given a Menu and an item number.
   (Internal utility, not exported)
   -------------------------------------------------------------------- */
static void GetItemDim(MENU *pMenu, int Item,
                       int *px1, int *py1, int *px2, int *py2)
{
   PALWIN *pMnuWin   = pMenu->pMnuWin;
   int x1, y1, x2, y2;
   int i, d, w;

   x1 = pMnuWin->PosX;
   y1 = pMnuWin->PosY;

   /* determine position of item */
   for(i = pMenu->TopItem; i < Item; i++) {
      GetItemExt(pMenu, i, &w, &d);

      /* we have to differ between vertical and horizontal menus */
      if(pMenu->Flags & MS_HORIZ) x1 += w;
      else                        y1 += d;
   }
   GetItemExt(pMenu, i, &w, &d);
   if(pMenu->Flags & MS_HORIZ) {
      x2 = x1 + w-1;
      y2 = y1 + pMnuWin->Depth-1;
   }
   else {
      x2 = x1 + pMnuWin->Width-1;
      y2 = y1 + d-1;
   }
   *px1 = x1;
   *px2 = x2;
   *py1 = y1;
   *py2 = y2;
}


/* --------------------------------------------------------------------
   ShowMenuItem
   Will display a menu item in normal or highlighted form, given a
   pointer to a menu and the item number.
   (Internal utility, not exported)
   -------------------------------------------------------------------- */
void ShowMenuItem(MENU *pMenu, int Item, int Hilite)
{
   int  Pos = -1;
   MENUITEM *pMnuItm;
   int x1, y1, x2, y2;
   int offx;

   pMnuItm = &pMenu->Items[Item];
   GetItemDim(pMenu, Item, &x1, &y1, &x2, &y2);
   SetColor(Hilite ? BLACK_COLOR : WHITE_COLOR);
   SetRule(FORCE_RULE);
   Rectangle(x1, y1, x2, y2, SOLID_FILL);
   SetRule(XOR_RULE);
   SetColor(BLACK_COLOR);

   switch(pMnuItm->Flags & MI_TYPMSK) {
      case MT_TXT:
         offx = (pMenu->Flags & MS_HORIZ) ? 10 : 20;
         LabelOut(x1+offx, y1+2, XOR_RULE, pMenu->pMnuWin->Font, pMnuItm->Text);
         break;
      case MT_SEP:
         if(pMenu->Flags & MS_HORIZ) {
            Line(x1+2, y1, x1+2, y1+pMenu->pMnuWin->Depth-1);
         }
         else Line(x1, y1+2, x1+pMenu->pMnuWin->Width-1, y1+2);
         break;
      case MT_BMP:
         PutImg(x1, y1, XOR_RULE, (char *)pMnuItm->Text);
         break;
   }
   if(pMnuItm->Flags & MI_GRY) {
      SetColor(Hilite ? BLACK_COLOR : WHITE_COLOR);
      SetRule(Hilite ? OR_RULE : AND_RULE);
      Rectangle(x1, y1, x2, y2, PATTERN_FILL);
   }

   /* draw scroll bar if needed */

   if(!(pMenu->Flags & MS_HORIZ) &&
        ((pMenu->TopItem) || (pMenu->ItmDisp != pMenu->ItmCount))) {
      x2 -= ARROW_W;
      if(Item && (Item == pMenu->TopItem)) {
         PutImg(x2 + 1, y1, FORCE_RULE, ArrowUp);
      }
      if((Item == pMenu->TopItem + pMenu->ItmDisp - 1) &&
          (Item != pMenu->ItmCount - 1)) {
         PutImg(x2 + 1, y2 - ARROW_H + 1, FORCE_RULE, ArrowDn);
      }
      Line(x2, y1, x2, y2);
   }
}

void ShowMenu(MENU *pMenu)
{
   int i;

   for(i = pMenu->TopItem; i < pMenu->TopItem+pMenu->ItmDisp; i++) {
      ShowMenuItem(pMenu, i, i == pMenu->CurItem);
   }
}

/* --------------------------------------------------------------------
   MenuAction:
   Will carry out the action associated with the current Menu item -
   either return an integer value to the caller, call a function,
   or call another menu.
   (Internal utility, not exported)
   -------------------------------------------------------------------- */

int MenuAction(MENU *pMenu)
{
   int Ret;
   int x1, y1, x2, y2;
   int px, py;
   MNUCALLBACK pCallback;
   MENUITEM *pMnuItm;
   PALWIN *pMnuWin = pMenu->pMnuWin;

   pMnuItm = &pMenu->Items[pMenu->CurItem];
   switch(pMnuItm->Flags & MI_ACTMSK) {
      case MA_VAL:
         return (int)pMnuItm->ActionPtr;
      case MA_MNU:
         GetItemDim(pMenu, pMenu->CurItem, &x1, &y1, &x2, &y2);
         px = (pMenu->Flags & MS_HORIZ) ? x1-1 : x2+1;
         py = (pMenu->Flags & MS_HORIZ) ? y2+1 : y1-1;
         Ret = HandleMenu((MENU *)pMnuItm->ActionPtr, px, py);
         if(Ret != MSC_ESC) return Ret;
         break;
      case MA_FUN:
         pCallback = (MNUCALLBACK)pMnuItm->ActionPtr;
         if(sizeof(MNUCALLBACK) == sizeof(void *)) {
            /* Small or Large model required for MNUCALLBACK to work */
            Ret = (*pCallback)(pMenu);
         }
         return Ret;
   }
   return MSC_STAY;
}


/* --------------------------------------------------------------------
   HorzKey:
   Translates a keypress in a horizontal menu so that it can be treated
   as in a vertical menu.
   (Internal utility, not exported)
   -------------------------------------------------------------------- */

int HorzKey(int Key)
{
   switch(Key) {
      case LEFT_KEY:  return UP_KEY;
      case RIGHT_KEY: return DOWN_KEY;
      case DOWN_KEY:  return SPACE_KEY;
   }
   return Key;
}


/* --------------------------------------------------------------------
   HandleMenuKey:
   Handles keys in a menu, given the menu and the key.
   Will set a flag if the entire menu needs updating
   (Internal utility, not exported)
   -------------------------------------------------------------------- */

int HandleMenuKey(MENU *pMenu, int Key, int *pUpd)
{
   static WORD MoveLeft[]  = { ESC_KEY, LEFT_KEY,  SPACE_KEY, 0 };
   static WORD MoveRight[] = { ESC_KEY, RIGHT_KEY, SPACE_KEY, 0 };
   static WORD DownKey[]   = { DOWN_KEY, 0 };
   static WORD UpKey[]     = { UP_KEY, 0 };
   static WORD LeftKey[]   = { LEFT_KEY, 0 };
   static WORD RightKey[]  = { RIGHT_KEY, 0 };
   static WORD EnterKey[]  = { ENTER_KEY, 0 };
   int i;

   *pUpd = FALSE; /* no updating necessary _yet_ */
   if(Key & 0xff) Key &= 0xff; /* if ASCII, reduce */

   /* search for hotkey */
   for(i = 0; i < pMenu->ItmCount; i++) {
      char *s;

      if(((pMenu->Items[i].Flags & MI_TYPMSK) == MT_TXT) &&
         (s = strchr(pMenu->Items[i].Text, '&')) &&
         (toupper(Key) == toupper(s[1])) &&
         !(pMenu->Items[i].Flags & MI_SKIP) ) break;
   }
   if(i < pMenu->ItmCount) { /* found hotkey */
      pMenu->CurItem = i;
      PushKeys(EnterKey);
   }
   if(pMenu->Flags & MS_HORIZ) Key = HorzKey(Key);
   switch(Key) {
      case HOME_KEY:
         pMenu->CurItem = pMenu->ItmCount;
         break;
      case END_KEY:
         pMenu->CurItem = -1;
         break;
      case DOWN_KEY:
         ++pMenu->CurItem;
         break;
      case UP_KEY:
         --pMenu->CurItem;
         break;
      case PGUP_KEY:
         pMenu->CurItem -= pMenu->ItmDisp-1;
         break;
      case PGDN_KEY:
         pMenu->CurItem += pMenu->ItmDisp-1;
         break;
      case ESC_KEY:
         return MSC_ESC;
      case SPACE_KEY:
         if((pMenu->Items[pMenu->CurItem].Flags & MI_ACTMSK) != MA_MNU) break;
      case ENTER_KEY:
         return MenuAction(pMenu);
      case LEFT_KEY:
         if(pMenu->Flags & MS_PULLDN) PushKeys(MoveLeft);
         break;
      case RIGHT_KEY:
         if(pMenu->Flags & MS_PULLDN) PushKeys(MoveRight);
         break;
      default:
         break;
   }
   if(pMenu->CurItem < 0) pMenu->CurItem = pMenu->ItmCount-1;
   if(pMenu->CurItem >= pMenu->ItmCount) pMenu->CurItem = 0;
   if(pMenu->CurItem < pMenu->TopItem) {
      pMenu->TopItem = pMenu->CurItem;
      *pUpd = TRUE;
   }
   if(pMenu->CurItem >= pMenu->TopItem+pMenu->ItmDisp) {
      pMenu->TopItem = max(0, pMenu->CurItem - pMenu->ItmDisp + 1);
      *pUpd = TRUE;
   }

   if(pMenu->Items[pMenu->CurItem].Flags & MI_SKIP) {
      PushKeys(   (Key == DOWN_KEY)
                ? ((pMenu->Flags & MS_HORIZ) ? RightKey : DownKey)
                : ((pMenu->Flags & MS_HORIZ) ? LeftKey  : UpKey)
              );
   }
   return MSC_STAY;
}


/* --------------------------------------------------------------------
   HandleMenu:
   The menu working horse - given a pointer to a menu structure and
   a position, creates a window for the menu, displays it, and handles
   keystrokes. Will return a non-zero id for a selected menu entry,
   or -1 if the menu was aborted.
   -------------------------------------------------------------------- */

int HandleMenu(MENU *pMenu, int PosX, int PosY)
{
   int Key;
   int Ret = 0;
   int OwnWin = 0;
   int Upd = TRUE;
   int LastItem;

   if(!PalStateSaved++) SaveState(&PalState);

   /* Create a window for the menu if none present */
   if(OwnWin = !pMenu->pMnuWin) CreateMenuWin(pMenu, PosX, PosY);

   /* Now loop */
   do {
      if(Upd) ShowMenu(pMenu);
      else {
         ShowMenuItem(pMenu, LastItem, 0);       /* UnHilite last item  */
         ShowMenuItem(pMenu, pMenu->CurItem, 1); /* Hilite current item */
      }
      LastItem = pMenu->CurItem;
      Key = GetKey();                         /* get a key      */
      Ret = HandleMenuKey(pMenu, Key, &Upd);  /* handle our key */
   } while(Ret == MSC_STAY); /* as long as nothing is to be returned */

   /* if we created the window ourselves, close it */
   if(OwnWin) {
      CloseWin(pMenu->pMnuWin);
      pMenu->pMnuWin = NULL; /* zero pointer for next pass */
   }

   if(!--PalStateSaved) RestoreState(&PalState);
   return Ret;
}

