/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  DHSTNDRD.C
   Author:  Gilles Kohl
   Started: 21. Jan 95
   Subject: Standard dialog handler functions
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

static WORD EnterKey[] = { ENTER_KEY, 0 };
static WORD SpaceKey[] = { SPACE_KEY, 0 };

/* --------------------------------------------------------------------
                           functions
   -------------------------------------------------------------------- */

int DhStdInit(DIALOG *pDlg)
{
   int i;

   /* setup current focus */
   pDlg->CurFocus = pDlg->InitFocus;

   /* send init messages to all items */
   for(i = 0; i < pDlg->ItmCnt; i++) {
      if(!(pDlg->Items[i].ItmProc)(IM_INIT, i == pDlg->CurFocus, NULL, pDlg,
                                   &pDlg->Items[i])) {
         return FALSE;
      }
   }
   return TRUE;
}

int DhStdShow(DIALOG *pDlg)
{
   int i;

   for(i = 0; i < pDlg->ItmCnt; i++) {
      if(!(pDlg->Items[i].ItmProc)(IM_SHOW, 0, NULL, pDlg, &pDlg->Items[i])) {
         return FALSE;
      }
   }
   if(pDlg->ItmCnt > 0 && pDlg->CurFocus >= 0) {
      (pDlg->Items[pDlg->CurFocus].ItmProc)(IM_SETFOCUS, 0, NULL,
                                         pDlg, &pDlg->Items[pDlg->CurFocus]);
   }
   return TRUE;
}

int DhStdKill(DIALOG *pDlg)
{
   int i;

   /* kill each item */
   for(i = 0; i < pDlg->ItmCnt; i++) {
      if(!(pDlg->Items[i].ItmProc)(IM_KILL, 0, NULL, pDlg, &pDlg->Items[i])) {
         free(pDlg->Data);
         return FALSE;
      }
   }
   free(pDlg->Data);
   return TRUE;
}

int DhStdSetFocus(DIALOG *pDlg, int ItmNbr)
{
   int i, n;
   int Ok;

   i = pDlg->CurFocus;
   n = ItmNbr;

   /* steal focus from current item, return if that doesn't work */
   if(i >= 0 && !(pDlg->Items[i].ItmProc)(IM_LOSEFOCUS, 0, NULL, pDlg,
           &pDlg->Items[i])) return FALSE;

   /* try to set focus to item requested by user */
   Ok = (pDlg->Items[n].ItmProc)(IM_SETFOCUS, 0, NULL, pDlg,
                                 &pDlg->Items[n]);
   /* if that didn't work and the focus WAS valid, set it back */
   if(!Ok && (i >= 0)) (pDlg->Items[i].ItmProc)(IM_SETFOCUS, 0, NULL, pDlg,
                                                &pDlg->Items[i]);
   /* if it worked fine, update current focus in pDlg */
   if(Ok) pDlg->CurFocus = n;
   return Ok;
}

static void NextFocus(DIALOG *pDlg, int Dir)
{
   int i;
   int Found = 0;

   if(!pDlg->ItmCnt) return; /* give up if empty dialog */
   i = pDlg->CurFocus;

   /* steal focus from current item, return if that doesn't work */
   if(i >= 0 && !(pDlg->Items[i].ItmProc)(IM_LOSEFOCUS, 0, NULL, pDlg,
               &pDlg->Items[i])) return;

   /* if focus not on an item, fake one for cyclic test to work */
   if(i < 0) pDlg->CurFocus = (Dir > 0) ? pDlg->ItmCnt-1 : 0;

   /* now, try to find an item that accepts the focus */
   do {
      i = i + Dir;
      if(i < 0) i = pDlg->ItmCnt-1;
      i = i % pDlg->ItmCnt;

      Found = (pDlg->Items[i].ItmProc)(IM_SETFOCUS, 0, NULL, pDlg,
                                       &pDlg->Items[i]);
   } while(!Found && (i != pDlg->CurFocus));

   /* OK, if that didn't set the focus, nobody wants it */
   pDlg->CurFocus = Found ? i : -1;
}

int DefButn(DIALOG *pDlg, int Key)
{
   int i;
   long Mask = (Key == ESC_KEY) ? IF_ESC : IF_RET;

   /* try to find an item with the correct mask */
   for(i = 0; i < pDlg->ItmCnt; i++) {
      if(pDlg->Items[i].Flags & Mask) break; /* stop at the first one */
   }
   if(i >= pDlg->ItmCnt) {
      /* we didn't find any - return DN_OK or DN_CANCEL */
      return (Key == ENTER_KEY) ? DN_OK : DN_CANCEL;
   }
   /* try to set focus to this item */
   if(DhStdSetFocus(pDlg, i)) {
      /* if that works, push RETURN */
      PushKeys(EnterKey);
      return DN_ACK; /* we handled it */
   }
   return DN_NACK;
}

void DhStdHandleRadio(DIALOG *pDlg, int n)
{
   int Group;
   int i;

   /* determine the radio button group number */
   Group = pDlg->Items[n].Flags & IF_RBM;

   if(!Group) return; /* no radio button at all */

   /* deselect all buttons of the same group except this one */
   for(i = 0; i < pDlg->ItmCnt; i++) {
      if((pDlg->Items[i].Flags & IF_RBM) == Group && i != n) {
         SetDlgItem(pDlg, i, RASI_STATE, VALUE(0));
      }
   }
   /* set current one to be sure */
   SetDlgItem(pDlg, n, RASI_STATE, VALUE(1));

}

int AltLetter(DIALOG *pDlg, int Key)
{
   int i;
   char *s;
   int AscKey;
   int ret;

   AscKey = (Key&0xff) ? toupper(Key&0xff) : Key;

   /* try to find an item with an Alt-Letter shortcut */
   for(i = 0; i < pDlg->ItmCnt; i++) {
      if(pDlg->Items[i].Text && (s = strchr(pDlg->Items[i].Text, '&'))) {
         if(AscKey == toupper(s[1]) || Key == AltKey(s[1])) break;
      }
   }
   if(i >= pDlg->ItmCnt) return FALSE;

   /* try to set focus to this item */
   ret = DhStdSetFocus(pDlg, i);
   if(ret && (pDlg->Items[i].Flags & IF_FRT)) {
      PushKeys(SpaceKey);
   }
   return ret;
}

int DhStdKey(DIALOG *pDlg, int Key)
{
   int i;
   int Notif = 0;

   i = pDlg->CurFocus;

   if(i >= 0) { /* if focus is on any item ... */
      /* ... send key to item, store notification it returns */
      Notif = (pDlg->Items[i].ItmProc)(IM_KEY, Key, NULL, pDlg, &pDlg->Items[i]);
   }

   if(i < 0 || Notif == IN_NACK) { /* no focus or item didn't recognize it */
      int RetVal;

      if(Key & 0xff) Key &= 0xff;
      switch(Key) {
         case ENTER_KEY: RetVal = DefButn(pDlg, Key);
            if(RetVal != DN_OK || pDlg->ItmCnt == 0) return RetVal;
            /* fall thru to TAB case on RETURN in DLG without defbutton */
         case DOWN_KEY:
         case RIGHT_KEY:
         case '\t':
            NextFocus(pDlg,  1);
            if(Key == DOWN_KEY || Key == RIGHT_KEY)
               DhStdHandleRadio(pDlg, pDlg->CurFocus);
            break;
         case UP_KEY:
         case LEFT_KEY:
         case STAB_KEY:
            NextFocus(pDlg, -1);
            if(Key == LEFT_KEY || Key == UP_KEY)
               DhStdHandleRadio(pDlg, pDlg->CurFocus);
            break;
         case ESC_KEY:   return DefButn(pDlg, Key);
         case F10_KEY:   return DN_OK;
         case F9_KEY:    return DN_CANCEL;
         default:        if(!AltLetter(pDlg, Key)) return DN_NACK;
            DhStdHandleRadio(pDlg, pDlg->CurFocus);
      }
   }
   else switch(Notif) {
      case IN_QUIT:    return i;
      case IN_CHANGE:
         /* handle radio buttons here */
         DhStdHandleRadio(pDlg, i);
         break;
   }
   return DN_ACK;
}

int DhStandard(int Msg, int n, void *p, DIALOG *pDlg)
{
   switch(Msg) {
      case DM_INIT:     return DhStdInit(pDlg);
      case DM_SHOW:     return DhStdShow(pDlg);
      case DM_SETFOCUS: return DhStdSetFocus(pDlg, n);
      case DM_KEY:      return DhStdKey(pDlg, n);
      case DM_KILL:     return DhStdKill(pDlg);
      default:          return FALSE;
   }
}

