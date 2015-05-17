/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  IHCOMBO.C
   Author:  Gilles Kohl
   Started: 24. Jan 95
   Subject: Functions that implement the 'combo box' dialog item class
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
#include "palpriv.h"

/* --------------------------------------------------------------------
                         global data
   -------------------------------------------------------------------- */
unsigned char ArrowImg[] = {
   0x01, 0x00, 0x01, 0x00, 0x08, 0x00, 0x08, 0x00, 0xFF, 0xE7,
   0xE7, 0xE7, 0x81, 0xC3, 0xE7, 0xFF,
};

/* --------------------------------------------------------------------
                           functions
   -------------------------------------------------------------------- */

int IhCbInit(DIALOG *pDlg, DLGITEM *pItm, int GotFocus)
{
   IHCOMBO  *pDat;              /* points to private data of item */
   IHCBINIT *pIni = pItm->Init; /* points to eventual init data   */
   int n, i;
   char *s;

   /* an init structure is mandatory - fail if it is missing */
   if(!pIni) return FALSE;

   /* the combo item is derived from the edit item - initialize it first,
      fail if this doesn't work for some reason */
   if(!IhEdit(IM_INIT, GotFocus, NULL, pDlg, pItm)) return FALSE;

   /* point to what the edit item set up for us */
   pDat = pItm->Data;

   /* now, initialize the additional 'combo' stuff */
   /* copy the list string */
   pDat->List = strdup(pIni->List);

   /* loop over List, terminating strings and counting them */
   for(n = 0, s = pDat->List; *s; ++s) {
      if(*s == '|') {
         *s = '\0';
         n++;
      }
   }
   /* add one more if list not empty since
      last string doesn't have a | character */
   if(*pDat->List) ++n;

   /* initialize menu */
   pDat->CbMnu.pMnuWin  = NULL;
   pDat->CbMnu.Flags    = 0;
   pDat->CbMnu.CurItem  = 0;
   pDat->CbMnu.TopItem  = 0;
   pDat->CbMnu.ItmDisp  = min(5, n);
   pDat->CbMnu.ItmCount = n;

   /* alloc item space - add 2 in case n is zero */
   pDat->CbMnu.Items = malloc(n*sizeof(MENUITEM) + 2);

   /* initialize items */
   for(s = pDat->List, i = 0; i < n; i++, s += strlen(s)+1) {
      MENUITEM *pMnuItm = &pDat->CbMnu.Items[i];

      pMnuItm->Text      = s;
      pMnuItm->Flags     = MT_TXT|MA_VAL;
      pMnuItm->ActionPtr = MENUVAL(i);
   }

   /* store pointer to private data in item structure */
   pItm->Data = pDat;

   /* that's all folks */
   return TRUE;
}

int CbGetItem(DIALOG *pDlg, DLGITEM *pItm, int n, void *p)
{
   IHCOMBO *pDat     = pItm->Data; /* point to private data */
   MENUITEM *pMnuItm = pDat->CbMnu.Items;
   char *CurStr;
   char buf[512];
   int i;

   switch(n) { /* get what data ? */
      case CBGI_IDX: /* the index of the current string */
         /* first, get a pointer to the current string */
         IhEdit(IM_GETDATA, EDGI_PTR, &CurStr, pDlg, pItm);
         for(i = 0; pMnuItm[i].Text; i++) {
            StrNrml(buf, pMnuItm[i].Text); /* normalize */
            if(stricmp(buf, CurStr) == 0) break;
         }
         if(pMnuItm[i].Text == NULL) i = -1;
         *(int *)p = i;
         break;
      default:  return FALSE;
   }
   return TRUE;
}

int IhCbShow(DIALOG *pDlg, DLGITEM *pItm)
{
   int  x, y;
   int  h;
   int  WinPos;
   int  WinLen;
   IHEDIT *pDat = pItm->Data; /* a small lie, but makes it easier :-) */
   IMGHDR *pArrow = (IMGHDR *)ArrowImg;

   /* display edit control, give up if that fails */
   if(!IhEdit(IM_SHOW, 0, NULL, pDlg, pItm)) return FALSE;

   /* if list is empty, we're done here */
   if(!((IHCOMBO *)pDat)->CbMnu.ItmCount) return TRUE;

   /* set some local variables for speed */
   WinPos = pDat->WinPos;
   WinLen = pDat->WinLen;

   /* compute absolute position of bitmap */
   x = pDlg->pWin->PosX + pItm->PosX + WinPos+WinLen;
   y = pDlg->pWin->PosY + pItm->PosY - 2;

   /* compute height of bitmap */
   h = FontHeight(pDat->Font) + 4;

   /* display black rectangle */
   ClrBlock(x, y, x+h-1, y+h-1, BLACK_COLOR);

   /* center arrow bitmap in black rect */
   PutImg(x+(h-pArrow->Width)/2, y+(h-pArrow->Depth)/2,
          FORCE_RULE, pArrow);
   return TRUE;
}


int IhCbKey(DIALOG *pDlg, DLGITEM *pItm, int Key)
{
   int x;
   int y;
   int n;
   IHCOMBO *pDat = pItm->Data;
   MENU *pMnu = &pDat->CbMnu;
   char buf[512];

   /* all keys except cursor down are handled by the edit item */
   if((Key == DOWN_KEY) && !(pDat->CbMnu.ItmCount)) return TRUE;
   if((Key != DOWN_KEY) || !(pDat->CbMnu.ItmCount)) {
      return IhEdit(IM_KEY, Key, NULL, pDlg, pItm);
   }

   /* compute absolute position of menu */
   x = pDlg->pWin->PosX + pItm->PosX + pDat->Ed.WinPos;
   y = pDlg->pWin->PosY + pItm->PosY + FontHeight(pDat->Ed.Font);

   if((n = HandleMenu(pMnu, x, y)) >= 0) {
      int Sel = TRUE;

      /* if user selected an entry, update edit item */
      StrNrml(buf, pMnu->Items[n].Text); /* normalize */
      pDat->Ed.SelFlag = TRUE;
      IhEdit(IM_SETDATA, EDSI_TXT, buf,  pDlg, pItm);
   }

   return TRUE;
}

int IhCbKill(DIALOG *pDlg, DLGITEM *pItm)
{
   IHCOMBO *pDat = pItm->Data;
   MENU *pMnu = &pDat->CbMnu;

   /* free what we allocated ... */
   free(pDat->List);
   free(pMnu->Items);

   /* ... and let the edit item do the rest of the work */
   return IhEdit(IM_KILL, 0, NULL, pDlg, pItm);
}

int IhCombo(int Msg, int n, void *p, DIALOG *pDlg, DLGITEM *pItm)
{
   switch(Msg) {
      case IM_INIT:      return IhCbInit(pDlg, pItm, n);
      case IM_KEY:       return IhCbKey(pDlg, pItm, n);
      case IM_SHOW:      return IhCbShow(pDlg, pItm);
      case IM_KILL:      return IhCbKill(pDlg, pItm);

      /* the following cases are all handled by the edit item */
      case IM_LOSEFOCUS:
      case IM_SETDATA:   return IhEdit(Msg, n, p, pDlg, pItm);
      case IM_GETDATA:   return IhEdit(Msg, n, p, pDlg, pItm) ||
                                CbGetItem(pDlg, pItm, n, p);
      case IM_SETFOCUS:  return IhEdit(Msg, n, p, pDlg, pItm);

      default:           return FALSE;
   }
}
