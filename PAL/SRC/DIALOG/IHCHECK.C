/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  IHCHECK.C
   Author:  Gilles Kohl
   Started: 05. Feb 95
   Subject: Functions that implement the 'Check' dialog item class
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
                           functions
   -------------------------------------------------------------------- */

void IhCkInitText(IHCHECK *pDat, char *s)
{
   /* if there's an old string, free it */
   if(pDat->Text) free(pDat->Text);

   /* copy new one */
   pDat->Text = strdup(s);
}

int IhCkInit(DLGITEM *pItm, int GotFocus)
{
   IHCHECK  *pDat;              /* points to private data of item */
   IHCKINIT *pIni = pItm->Init; /* points to eventual init data   */

   /* try to allocate private data, return FALSE on failure */
   if(!(pDat = malloc(sizeof(*pDat)))) return FALSE;

   /* if Ini structure there, use its font, otherwise default font */
   pDat->Font = pIni ? pIni->Font : MEDIUM_FONT;

   /* if Ini structure there, use its check state, otherwise zero */
   pDat->Checked = pIni ? pIni->Checked : 0;

   /* set focus information */
   pDat->Focus = GotFocus;

   /* store pointer to private data in item structure */
   pItm->Data = pDat;

   pDat->Text = NULL; /* NULL means use 'Text' from item descriptor */

   /* set 'want SPACE when focus set via ALT' flag */
   pItm->Flags |= IF_FRT;

   /* that's all folks */
   return TRUE;
}

int IhCkSetItem(DIALOG *pDlg, DLGITEM *pItm, int n, void *p)
{
   IHCHECK *pDat = pItm->Data; /* point to private data */

   switch(n) { /* set what data ? */
      case CKSI_LBL: /* The label */
         IhCkInitText(pDat, p);
         break;
      case CKSI_STATE: /* the check state */
         /* set new value */
         pDat->Checked = (int)p;
         break;
      default: return FALSE;
   }
   /* if dialog has been shown already, update item */
   if(pDlg->pWin) (pItm->ItmProc)(IM_SHOW, 0, NULL, pDlg, pItm);

   return TRUE;
}

int IhCkGetItem(DLGITEM *pItm, int n, void *p)
{
   IHCHECK *pDat = pItm->Data; /* point to private data */

   switch(n) { /* get what data ? */
      case CKGI_STATE: /* the buffer per value */
         *(int *)p = pDat->Checked;
         break;
      default:  return FALSE;
   }
   return TRUE;
}

int IhCkShow(DIALOG *pDlg, DLGITEM *pItm)
{
   IHCHECK *pDat = pItm->Data;  /* point to private data of item */
   int  x, y, w, h, fw, fh;
   int  bx1, by1, bx2, by2;
   char *s;

   /* compute absolute position of item */
   x = pDlg->pWin->PosX + pItm->PosX;
   y = pDlg->pWin->PosY + pItm->PosY;
   w = pItm->Width;
   h = pItm->Depth;

   /* use pDat->Text if present, else Text from item itself */
   if(!(s = pDat->Text)) s = pItm->Text;

   fh = FontHeight(pDat->Font);

   /* if there is a text at all, compute dimensions */
   fw = s ? LabelExt(pDat->Font, s) : 0;

   /* Clear entire item area to white */
   ClrBlock(x, y, x+w-1, y+h-1, WHITE_COLOR);

   /* compute checkbox coordinates */
   bx1 = x+1;
   bx2 = x+h-1;
   by1 = y+(h-fh)/2;
   by2 = by1+fh-1;

   /* draw checkbox frame */
   SetColor(BLACK_COLOR);
   SetRule(FORCE_RULE);
   Rectangle(bx1, by1, bx2, by2, OUTLINE_FILL);

   if(pDat->Checked) {
      Line(bx1, by1, bx2, by2);
      Line(bx2, by1, bx1, by2);
   }

   bx2 += 2; /* add some space between checkbox and text */
   /* Clear font area */
   ClrBlock(bx2, by1, bx2+fw, by2+2,
            pDat->Focus ? BLACK_COLOR : WHITE_COLOR);

   /* output text */
   if(s) LabelOut(bx2, by1+1, XOR_RULE,
                 pDat->Font, s);

   return TRUE;
}

int IhCkKey(DIALOG *pDlg, DLGITEM *pItm, int Key)
{
   IHCHECK *pDat = pItm->Data;

   if(Key & 0xff) Key &= 0xff;

   switch(Key) {
      case SPACE_KEY:
         pDat->Checked = !pDat->Checked;
         (pItm->ItmProc)(IM_SHOW, 0, NULL, pDlg, pItm);
         return IN_CHANGE;
   }
   return IN_NACK;
}


int IhCkFocus(DIALOG *pDlg, DLGITEM *pItm, int State)
{
   IHCHECK *pDat = pItm->Data;

   pDat->Focus = State;
   (pItm->ItmProc)(IM_SHOW, 0, NULL, pDlg, pItm);
   return TRUE;
}


int IhCkKill(DLGITEM *pItm)
{
   IHCHECK *pDat = pItm->Data;

   /* if there's an old string, free it */
   if(pDat->Text) free(pDat->Text);
   free(pDat);
   return TRUE;
}


int IhCheck(int Msg, int n, void *p, DIALOG *pDlg, DLGITEM *pItm)
{
   switch(Msg) {
      case IM_INIT:      return IhCkInit(pItm, n);
      case IM_SHOW:      return IhCkShow(pDlg, pItm);
      case IM_SETDATA:   return IhCkSetItem(pDlg, pItm, n, p);
      case IM_GETDATA:   return IhCkGetItem(pItm, n, p);
      case IM_SETFOCUS:
      case IM_LOSEFOCUS: return IhCkFocus(pDlg, pItm, Msg == IM_SETFOCUS);
      case IM_KILL:      return IhCkKill(pItm);
      case IM_KEY:       return IhCkKey(pDlg, pItm, n);
      default:           return FALSE;
   }
}

