/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  IHLISTBX.C
   Author:  Gilles Kohl
   Started: 13. Nov 95
   Subject: Functions that implement the 'ListBox' dialog item class
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

#define LB_CHUNK 50

/* --------------------------------------------------------------------
                           local prototypes
   -------------------------------------------------------------------- */
void IhLbSync(IHLISTBOX *pDat, int OldTop, int OldCur);

/* --------------------------------------------------------------------
                           functions
   -------------------------------------------------------------------- */

int IhLbInit(DLGITEM *pItm, int GotFocus)
{
   IHLISTBOX *pDat;              /* points to private data of item */
   IHLBINIT  *pIni = pItm->Init; /* points to eventual init data   */
   int h;

   /* try to allocate private data, return FALSE on failure */
   if(!(pDat = malloc(pItm->Size))) return FALSE;

   /* if Ini structure there, use its font, otherwise default font */
   pDat->Font = pIni ? pIni->Font : MEDIUM_FONT;
   h = FontHeight(pDat->Font);

   /* set focus information */
   pDat->Focus = GotFocus;

   pDat->Text = NULL; /* NULL means use 'Text' from item descriptor */

   /* initialize to empty listbox */
   pDat->CurStr   = -1;   /* current string   */
   pDat->TopStr   = 0;   /* top string       */
   pDat->StrDisp  = (pItm->Depth-h-4)/h;
   pDat->StrCount = 0;    /* no strings there yet  */
   pDat->StrAlloc = 0;    /* no empty slots either */
   pDat->Strings  = NULL;
   pDat->Cb       = NULL;
   pDat->NcPaint  = TRUE; /* indicate that frame needs painting */
   pDat->Upd0     = 0;               /* set update region ... */
   pDat->Upd1     = pDat->StrDisp-1; /* ... to full listbox   */
   pDat->Upd2     = -1; /* special line not needed  */
   pDat->SclDir   = -1; /* and don't scroll for now */

   /* store pointer to private data in item structure */
   pItm->Data = pDat;

   return TRUE;
}

int IhLbFindInsLoc(IHLISTBOX *pDat, char *s)
{
   int l = 0;
   int r = pDat->StrCount-1;
   int x = 0;
   int v = -1;

   while(l <= r) {
      x = (l+r)/2;
      if((v = strcmp(s, pDat->Strings[x])) < 0) r = x-1; else l = x+1;
   }
   return x + (v >= 0);
}

int IhLbAddString(DLGITEM *pItm, IHLISTBOX *pDat, IHLBUPD *pUpd, int Add, int Dsp)
{
   int  l;
   int  Upd = FALSE;
   int  n  = pUpd->StrIdx;    /* insert before this index */
   char *p = pUpd->Str;       /* the string to insert */
   int  Disp = pDat->StrDisp; /* easier to type ... */

   if(Add) {
      if(n == LBPOS_LAST) n = pDat->StrCount;
      if((n == LBPOS_SORT) && !pDat->Cb) n = IhLbFindInsLoc(pDat, p);
      if(n < 0 || n > pDat->StrCount) return FALSE;

      /* do we manage the strings ourselves ? */
      if(!pDat->Cb) { /* yes ... */
         /* do we need to realloc the item pointer array ? */
         if(pDat->StrCount+1 > pDat->StrAlloc) {
            pDat->StrAlloc += LB_CHUNK;
            pDat->Strings = realloc(pDat->Strings,
                                    pDat->StrAlloc*sizeof(char *));
         }
         if(!pDat->Strings) return FALSE;

         /* OK, now free up one slot if necessary */
         if(pDat->StrCount-n) {
            memmove(&pDat->Strings[n+1], &pDat->Strings[n],
                    (pDat->StrCount-n)*sizeof(char *));
         }
         pDat->Strings[n] = NULL;
      }
      pDat->StrCount++;
      if(n <=  pDat->TopStr && pDat->StrCount != 1) ++pDat->TopStr;
      if(n <= pDat->CurStr  || pDat->CurStr < 0) ++pDat->CurStr;

      /* if selection would move out of window, adjust top ... */
      if(pDat->CurStr >= pDat->TopStr+Disp) {
         Upd = TRUE;
         ++pDat->TopStr;
         if(Dsp) { /* but show only if window is already shown */
            pDat->Scl0 = 0; /* start with top */
            pDat->Scl1 = n-pDat->TopStr;
            pDat->SclDir = SCROLL_UP;
            pDat->Upd0 = pDat->Upd1 = pDat->Scl1;
         }
      }
      else if((pDat->TopStr <= n) && (n < pDat->TopStr+Disp) && Dsp) {
         Upd = TRUE;
         pDat->Scl0 = n-pDat->TopStr;
         pDat->Scl1 = Disp-1;
         pDat->SclDir = SCROLL_DOWN;
         pDat->Upd0 = pDat->Upd1 = pDat->Scl0;
      }
   }
   else {
      if(n < 0 || n > pDat->StrCount-1) return FALSE;
      if((pDat->TopStr <= n) && (n < pDat->TopStr+Disp) && Dsp) {
        Upd = TRUE;
        pDat->Upd0 = pDat->Upd1 = n-pDat->TopStr;
      }
   }
   /* do we manage the strings ourselves ? */
   if(!pDat->Cb) { /* yes ... */
      l = strlen(p);

      /* now (re)allocate storage for the new string */
      pDat->Strings[n] = realloc(pDat->Strings[n], l+1);
      if(!pDat->Strings[n]) return FALSE;
      strcpy(pDat->Strings[n], p);
   }

   return Upd;
}

int IhLbDelString(DLGITEM *pItm, IHLISTBOX *pDat, IHLBUPD *pUpd)
{
   int l;
   int n    = pUpd->StrIdx;
   int Upd  = FALSE;
   int Disp = pDat->StrDisp; /* easier to type ... */

   if(n == LBPOS_LAST) n = pDat->StrCount-1;
   if(n < 0 || n >= pDat->StrCount) return FALSE;

   --pDat->StrCount;
   /* do we manage the strings ourselves ? */
   if(!pDat->Cb) { /* yes ... */
      /* free the string itself */
      free(pDat->Strings[n]);

      /* move strings up one slot if necessary */
      if(pDat->StrCount-n) {
         memmove(&pDat->Strings[n], &pDat->Strings[n+1],
                 (pDat->StrCount-n)*sizeof(char *));
      }

      /* should we realloc the item pointer array ? */
      if(pDat->StrAlloc - pDat->StrCount >= LB_CHUNK) {
         pDat->StrAlloc -= LB_CHUNK;
         if(pDat->StrAlloc) {
            pDat->Strings = realloc(pDat->Strings, pDat->StrAlloc*sizeof(char *));
         }
         else {
            free(pDat->Strings);
            pDat->Strings = NULL;
         }
      }
   }
   if(n < pDat->TopStr) --pDat->TopStr;
   if(n < pDat->CurStr) --pDat->CurStr;
   if(pDat->CurStr >= pDat->StrCount) pDat->CurStr = pDat->StrCount-1;
   if(pDat->TopStr >= pDat->StrCount) pDat->TopStr = pDat->StrCount-1;
   if(pDat->StrCount) {
      if(n >= pDat->TopStr && pDat->TopStr+Disp > n) {
         Upd = TRUE;
         pDat->Scl0 = n-pDat->TopStr;
         pDat->Scl1 = Disp-1;
         pDat->Upd0 = max(n-pDat->TopStr-1, 0);
         pDat->Upd1 = n-pDat->TopStr;
         pDat->Upd2 = Disp-1;
         pDat->SclDir = SCROLL_UP;
      }
   }
   else {
      Upd = TRUE;
      pDat->CurStr = -1;
      pDat->TopStr =
      pDat->Upd0  =
      pDat->Upd1  = 0;
   }

   return Upd;
}

int IhLbClrStrings(DLGITEM *pItm, IHLISTBOX *pDat)
{
   int i;

   if(!pDat->Cb) {
      for(i = 0; i < pDat->StrCount; i++) free(pDat->Strings[i]);
      free(pDat->Strings);
      pDat->Strings  = NULL;
   }

   pDat->CurStr  =  -1;   /* current string   */
   pDat->TopStr  =  0;    /* top string       */
   pDat->StrCount = 0;    /* no strings there yet  */
   pDat->StrAlloc = 0;    /* no empty slots either */
   pDat->Upd0     = 0;               /* set update region ... */
   pDat->Upd1     = pDat->StrDisp-1; /* ... to full listbox   */
   pDat->Upd2     = -1;   /* special line not needed */
   pDat->SclDir   = -1;   /* don't scroll, though. */

   return TRUE;
}

int IhLbSetItem(DIALOG *pDlg, DLGITEM *pItm, int n, void *p)
{
   IHLISTBOX *pDat = pItm->Data; /* point to private data */
   int Upd = FALSE;
   int OldCur, OldTop;

   switch(n) { /* set what data ? */
      case LBSI_STRADD:
      case LBSI_STRSET:
         Upd = IhLbAddString(pItm, pDat, p, n == LBSI_STRADD, !!pDlg->pWin);
         break;
      case LBSI_STRDEL:
         Upd = IhLbDelString(pItm, pDat, p);
         break;
      case LBSI_STRCLR:
         Upd = IhLbClrStrings(pItm, pDat);
         break;
      case LBSI_STRCB:
         if(!pDat->StrCount) pDat->Cb = p;
         else return FALSE;
         break;
      case LBSI_STRCNT:
         if(!pDat->Cb) return FALSE;
         OldCur = (pDat->CurStr < 0) ? -999 : pDat->CurStr;
         OldTop = pDat->TopStr;
         pDat->StrCount = *((int *)p);
         IhLbSync(pDat, OldTop, OldCur);
         Upd = TRUE;
         break;
      case LBSI_STRPOS:
         if(!pDat->StrCount) break;
         OldCur = pDat->CurStr;
         OldTop = pDat->TopStr;
         pDat->CurStr = *((int *)p);
         if(pDat->CurStr == LBPOS_LAST) pDat->CurStr = pDat->StrCount-1;
         Upd = TRUE;
         IhLbSync(pDat, OldTop, OldCur);
         break;

      default: return FALSE;
   }
   /* if dialog has been shown already, update item */
   if(pDlg->pWin && Upd) (pItm->ItmProc)(IM_SHOW, 0, NULL, pDlg, pItm);

   return TRUE;
}

int IhLbGetItem(DLGITEM *pItm, int n, void *p)
{
   IHLISTBOX *pDat = pItm->Data; /* point to private data */
   IHLBUPD *pUpd;

   switch(n) { /* get what data ? */
      case LBGI_STRCNT:
         *((int *)p) = pDat->StrCount;
         break;
      case LBGI_STRPOS:
         *((int *)p) = pDat->CurStr;
         break;
      case LBGI_STRTXT:
         pUpd = p;
         pUpd->Str = NULL;
         if(pUpd->StrIdx >= 0 && pUpd->StrIdx < pDat->StrCount) {
            pUpd->Str = pDat->Cb ? (pDat->Cb)(pUpd->StrIdx)
                                 : pDat->Strings[pUpd->StrIdx];
         }
         break;
      default:
         return FALSE;
   }
   return TRUE;
}

int IhLbShow(DIALOG *pDlg, DLGITEM *pItm)
{
   IHLISTBOX *pDat = pItm->Data; /* point to private data */
   int Disp = pDat->StrDisp;     /* easier to type ... */
   int Done;
   int i, m;
   int x1, y1, x2, y2, h;
   int y;
   char *s;

   if(!pDlg->pWin) return FALSE;

   /* compute absolute position of item */
   x1 = pDlg->pWin->PosX + pItm->PosX;
   y1 = pDlg->pWin->PosY + pItm->PosY;
   x2 = x1+pItm->Width-1;
   y2 = y1+pItm->Depth-1;
   h = FontHeight(pDat->Font);

   /* output label */
   /* use pDat->Text if present, else Text from item itself */
   if(!(s = pDat->Text)) s = pItm->Text;

   SetRule(FORCE_RULE);
   SetColor(BLACK_COLOR);
   if(pDat->NcPaint) {
      ClrBlock(x1, y1, x2, y1+h+1, WHITE_COLOR);
      if(s) LabelOut(x1, y1, XOR_RULE, pDat->Font, s);
   }
   y1 += h+2;

   /* Draw enclosing rectangle */
   if(pDat->NcPaint) {
      SetRule(FORCE_RULE);
      Rectangle(x1, y1, x2, y2, OUTLINE_FILL);
   }
   ++x1; ++y1; --x2; --y2;
   SetClip(x1, y1, x2, y2);

   /* any scrolling to be done ? */
   if(pDat->SclDir >= 0) {
      if(pDat->Scl0 < pDat->Scl1) { /* scroll routine isn't robust ... */
         Scroll(pDat->SclDir, h, WHITE_COLOR, x1,
             y1+h*pDat->Scl0, x2, y1+h*pDat->Scl1+h-1);
      }
      pDat->SclDir = -1; /* scrolling done */
   }

   /* Show update region */
   do {
      for(i = pDat->Upd0; i <= pDat->Upd1; i++) {
         char *s;

         y = y1+h*i;
         m = pDat->TopStr+i;
         SetRule(FORCE_RULE);

         /* clear background */
         if(pDat->Focus) {
            ClrBlock(x1, y, x2, y+h-1,
                     m == pDat->CurStr ? BLACK_COLOR : WHITE_COLOR);
         }
         else {
            SetColor(WHITE_COLOR);
            ClrBlock(x1, y, x2, y+h-1, WHITE_COLOR);
            if(m == pDat->CurStr) {
               SetColor(BLACK_COLOR);
               SetLineType(0x5555);
               Rectangle(x1, y, x2, y+h-1, OUTLINE_FILL);
               SetLineType(0xffff);
            }
         }
         SetColor(BLACK_COLOR);

         /* call callback function to retrieve user string */
         if(m < pDat->StrCount) {
            s = pDat->Cb ? (pDat->Cb)(m)
                         : pDat->Strings[m];
            TextOut(x1, y, XOR_RULE, pDat->Font, s);
         }
      }
      if(!(Done = (pDat->Upd2 < 0))) {
         pDat->Upd0 = pDat->Upd1 = pDat->Upd2;
         pDat->Upd2 = -1;
      }
   } while(!Done);
   /* clear rest of box here */
   if(pDat->NcPaint) {
      /* xxx */
   }

   /* set update region back to full */
   pDat->Upd0 = 0;
   pDat->Upd1 = Disp-1;
   pDat->NcPaint = TRUE; /* paint all by default */

   SetClip(0, 0, MAX_X-1, MAX_Y-1);
   return TRUE;
}

void IhLbSync(IHLISTBOX *pDat, int OldTop, int OldCur)
{
   int Disp = pDat->StrDisp; /* easier to type ... */

   if(pDat->CurStr < 0)               pDat->CurStr = 0;
   if(pDat->CurStr >= pDat->StrCount) pDat->CurStr = pDat->StrCount-1;
   if(pDat->CurStr < pDat->TopStr)    pDat->TopStr = pDat->CurStr;
   if(pDat->CurStr >= pDat->TopStr+Disp) {
      pDat->TopStr = max(0, pDat->CurStr - Disp + 1);
   }
   pDat->Upd0 = 0;
   pDat->Upd1 = Disp-1;

   if(     pDat->TopStr - OldTop ==  1 && pDat->CurStr-OldCur == 1) {
      pDat->Scl0 = 0;
      pDat->Scl1 = Disp-2;
      pDat->SclDir = SCROLL_UP;
      pDat->Upd0 = Disp-2;
      pDat->Upd1 = Disp-1;
      pDat->NcPaint = FALSE;
   }
   else if(pDat->TopStr - OldTop == -1 && pDat->CurStr-OldCur == -1) {
      pDat->Scl0 = 1;
      pDat->Scl1 = Disp-1;
      pDat->SclDir = SCROLL_DOWN;
      pDat->Upd0 = 0;
      pDat->Upd1 = 1;
      pDat->NcPaint = FALSE;
   }
   else if(pDat->CurStr - OldCur == -1 && pDat->TopStr == OldTop) {
      pDat->Upd0 = OldCur - pDat->TopStr-1;
      pDat->Upd1 = pDat->Upd0+1;
      pDat->NcPaint = FALSE;
   }
   else if(pDat->CurStr - OldCur == 1 && pDat->TopStr == OldTop) {
      pDat->Upd0 = OldCur - pDat->TopStr;
      pDat->Upd1 = pDat->Upd0+1;
      pDat->NcPaint = FALSE;
   }
}

int IhLbSearch(IHLISTBOX *pDat, int Key)
{
   int n;
   char *s;

   if(Key & 0xff) Key &= 0xff;
   if((Key > 0xff) || !isalnum(Key)) return FALSE;

   if(pDat->StrCount <= 1 || pDat->CurStr < 0) return FALSE;

   for(n = (pDat->CurStr+1) % pDat->StrCount; n != pDat->CurStr;
       n = (n+1) % pDat->StrCount) {
      s = pDat->Cb ? (pDat->Cb)(n)
                   : pDat->Strings[n];
      if(toupper(*s) == toupper(Key)) {
         pDat->CurStr = n;
         break;
      }
   }
   return TRUE;
}

int IhLbKey(DIALOG *pDlg, DLGITEM *pItm, int Key)
{
   IHLISTBOX *pDat = pItm->Data; /* point to private data */
   int Disp = pDat->StrDisp; /* easier to type ... */
   int OldTop;
   int OldCur;

   /* if we don't have the focus or no strings, do nothing */
   if(!pDat->Focus || !pDat->StrCount) return FALSE;

   OldTop = pDat->TopStr;
   OldCur = pDat->CurStr;
   if(Key & 0xff) Key &= 0xff; /* if ASCII, reduce */

   switch(Key) {
      case HOME_KEY: pDat->TopStr = pDat->CurStr = 0; break;
      case END_KEY:  pDat->CurStr = pDat->StrCount-1; break;
      case DOWN_KEY: ++pDat->CurStr;                  break;
      case UP_KEY:   --pDat->CurStr;                  break;
      case PGUP_KEY: pDat->CurStr -= Disp-1; break;
      case PGDN_KEY: pDat->CurStr += Disp-1; break;
      case ENTER_KEY:return IN_QUIT;
      default:       if(!IhLbSearch(pDat, Key)) return FALSE;
   }
   IhLbSync(pDat, OldTop, OldCur);
   if(pDlg->pWin) (pItm->ItmProc)(IM_SHOW, 0, NULL, pDlg, pItm);

   return TRUE;
}


int IhLbFocus(DIALOG *pDlg, DLGITEM *pItm, int State)
{
   IHLISTBOX *pDat = pItm->Data; /* point to private data */

   /* empty dialog box should refuse the focus */
   if(!pDat->StrCount && State) return FALSE;
   if(pDat->Focus != State) {
      pDat->Focus = State;
      if(pDlg->pWin) {
         pDat->Upd0 = pDat->Upd1 = pDat->CurStr - pDat->TopStr;
         (pItm->ItmProc)(IM_SHOW, 0, NULL, pDlg, pItm);
      }
   }

   return TRUE;
}


int IhLbKill(DLGITEM *pItm)
{
   IHLISTBOX *pDat = pItm->Data; /* point to private data */
   int i;

   if(!pDat->Cb) {
      for(i = 0; i < pDat->StrCount; i++) free(pDat->Strings[i]);
   }
   free(pDat->Strings);
   free(pDat);

   return TRUE;
}


int IhListBox(int Msg, int n, void *p, DIALOG *pDlg, DLGITEM *pItm)
{
   switch(Msg) {
      case IM_INIT:      return IhLbInit(pItm, n);
      case IM_SHOW:      return IhLbShow(pDlg, pItm);
      case IM_SETDATA:   return IhLbSetItem(pDlg, pItm, n, p);
      case IM_GETDATA:   return IhLbGetItem(pItm, n, p);
      case IM_SETFOCUS:
      case IM_LOSEFOCUS: return IhLbFocus(pDlg, pItm, Msg == IM_SETFOCUS);
      case IM_KILL:      return IhLbKill(pItm);
      case IM_KEY:       return IhLbKey(pDlg, pItm, n);
      default:           return FALSE;
   }
}

/* --- Listbox utility functions --- */

int LbAddString(DIALOG *pDlg, int Item, int InsBefore, char *Str)
{
   IHLBUPD Upd;

   Upd.Str    = Str;
   Upd.StrIdx = InsBefore;

   return SetDlgItem(pDlg, Item, LBSI_STRADD, &Upd);
}

int LbSetString(DIALOG *pDlg, int Item, int Index, char *Str)
{
   IHLBUPD Upd;

   Upd.Str    = Str;
   Upd.StrIdx = Index;

   return SetDlgItem(pDlg, Item, LBSI_STRSET, &Upd);
}

char *LbGetString(DIALOG *pDlg, int Item, int Index)
{
   IHLBUPD Upd;

   Upd.StrIdx = Index;

   GetDlgItem(pDlg, Item, LBGI_STRTXT, &Upd);
   return Upd.Str;
}

int LbDelString(DIALOG *pDlg, int Item, int Index)
{
   IHLBUPD Upd;

   Upd.StrIdx = Index;
   return SetDlgItem(pDlg, Item, LBSI_STRDEL, &Upd);
}

int LbGetCount(DIALOG *pDlg, int Item)
{
   int Count = -1;

   GetDlgItem(pDlg, Item, LBGI_STRCNT, &Count);
   return Count;
}

int LbGetPos(DIALOG *pDlg, int Item)
{
   int Pos = -1;

   GetDlgItem(pDlg, Item, LBGI_STRPOS, &Pos);
   return Pos;
}

int LbSetPos(DIALOG *pDlg, int Item, int Index)
{
   return SetDlgItem(pDlg, Item, LBSI_STRPOS, &Index);
}

int LbClear(DIALOG *pDlg, int Item)
{
   return SetDlgItem(pDlg, Item, LBSI_STRCLR, NULL);
}

int LbSetCallBack(DIALOG *pDlg, int Item, char *(*pCb)(int n))
{
   return SetDlgItem(pDlg, Item, LBSI_STRCB, pCb);
}

int LbSetCount(DIALOG *pDlg, int Item, int Count)
{
   return SetDlgItem(pDlg, Item, LBSI_STRCNT, &Count);
}

