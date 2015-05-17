/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  IHRADIO.C
   Author:  Gilles Kohl
   Started: 05. Feb 95
   Subject: Functions that implement the 'Radio' dialog item class
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
unsigned char Radio0[] = {
   0x01, 0x00, 0x01, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x1F, 0x00,
   0x20, 0x80, 0x40, 0x40, 0x80, 0x20, 0x80, 0x20, 0x80, 0x20,
   0x80, 0x20, 0x80, 0x20, 0x40, 0x40, 0x20, 0x80, 0x1F, 0x00,
   0x00, 0x00,
};

unsigned char Radio1[] = {
   0x01, 0x00, 0x01, 0x00, 0x0C, 0x00, 0x0C, 0x00, 0x1F, 0x00,
   0x20, 0x80, 0x4E, 0x40, 0x9F, 0x20, 0xBF, 0xA0, 0xBF, 0xA0,
   0xBF, 0xA0, 0x9F, 0x20, 0x4E, 0x40, 0x20, 0x80, 0x1F, 0x00,
   0x00, 0x00,
};



/* --------------------------------------------------------------------
                           functions
   -------------------------------------------------------------------- */

void IhRaInitText(IHRADIO *pDat, char *s)
{
   /* if there's an old string, free it */
   if(pDat->Text) free(pDat->Text);

   /* copy new one */
   pDat->Text = strdup(s);
}

int IhRaInit(DLGITEM *pItm, int GotFocus)
{
   IHRADIO  *pDat;              /* points to private data of item */
   IHRAINIT *pIni = pItm->Init; /* points to eventual init data   */

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

   /* that's all folks */
   return TRUE;
}

int IhRaSetItem(DIALOG *pDlg, DLGITEM *pItm, int n, void *p)
{
   IHRADIO *pDat = pItm->Data; /* point to private data */

   switch(n) { /* set what data ? */
      case RASI_LBL: /* The label */
         IhRaInitText(pDat, p);
         break;
      case RASI_STATE: /* the check state */
         /* set new value */
         pDat->Checked = (int)p;
         break;
      default: return FALSE;
   }
   /* if dialog has been shown already, update item */
   if(pDlg->pWin) (pItm->ItmProc)(IM_SHOW, 0, NULL, pDlg, pItm);

   return TRUE;
}

int IhRaGetItem(DLGITEM *pItm, int n, void *p)
{
   IHRADIO *pDat = pItm->Data; /* point to private data */

   switch(n) { /* get what data ? */
      case RAGI_STATE: /* the check state */
         *(int *)p = pDat->Checked;
         break;
      default:  return FALSE;
   }
   return TRUE;
}

int IhRaShow(DIALOG *pDlg, DLGITEM *pItm)
{
   static IMGHDR *pImg[] = { (IMGHDR*)Radio0, (IMGHDR*)Radio1 };
   IHRADIO *pDat = pItm->Data;  /* point to private data of item */
   int  x, y, w, h, fw, fh, ih, iw;
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
   ih = pImg[0]->Depth;
   iw = pImg[0]->Width;

   /* if there is a text at all, compute dimensions */
   fw = s ? LabelExt(pDat->Font, s) : 0;

   /* Clear entire item area to white */
   ClrBlock(x, y, x+w-1, y+h-1, WHITE_COLOR);

   /* compute bitmap coordinates */
   bx1 = x+1;
   by1 = y+(h-ih)/2;

   PutImg(bx1, by1, FORCE_RULE, pImg[!!pDat->Checked]);

   bx2 = bx1+iw+2; /* add some space between checkbox and text */
   by2 = y+(h+fh)/2;

   /* Clear font area */
   ClrBlock(bx2, by1, bx2+fw, by2+2,
            pDat->Focus ? BLACK_COLOR : WHITE_COLOR);

   /* output text */
   if(s) LabelOut(bx2, by1+1, XOR_RULE, pDat->Font, s);

   return TRUE;
}

int IhRaKey(DIALOG *pDlg, DLGITEM *pItm, int Key)
{
   IHRADIO *pDat = pItm->Data;

   if(Key & 0xff) Key &= 0xff;

   switch(Key) {
      case SPACE_KEY:
         pDat->Checked = TRUE;
         (pItm->ItmProc)(IM_SHOW, 0, NULL, pDlg, pItm);
         return IN_CHANGE;
   }
   return IN_NACK;
}


int IhRaFocus(DIALOG *pDlg, DLGITEM *pItm, int State)
{
   IHRADIO *pDat = pItm->Data;

   pDat->Focus = State;
   (pItm->ItmProc)(IM_SHOW, 0, NULL, pDlg, pItm);
   return TRUE;
}


int IhRaKill(DLGITEM *pItm)
{
   IHRADIO *pDat = pItm->Data;

   /* if there's an old string, free it */
   if(pDat->Text) free(pDat->Text);
   free(pDat);
   return TRUE;
}


int IhRadio(int Msg, int n, void *p, DIALOG *pDlg, DLGITEM *pItm)
{
   switch(Msg) {
      case IM_INIT:      return IhRaInit(pItm, n);
      case IM_SHOW:      return IhRaShow(pDlg, pItm);
      case IM_SETDATA:   return IhRaSetItem(pDlg, pItm, n, p);
      case IM_GETDATA:   return IhRaGetItem(pItm, n, p);
      case IM_SETFOCUS:
      case IM_LOSEFOCUS: return IhRaFocus(pDlg, pItm, Msg == IM_SETFOCUS);
      case IM_KILL:      return IhRaKill(pItm);
      case IM_KEY:       return IhRaKey(pDlg, pItm, n);
      default:           return FALSE;
   }
}

