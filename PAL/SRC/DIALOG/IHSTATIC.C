/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  IHSTATIC.C
   Author:  Gilles Kohl
   Started: 24. Jan 95
   Subject: Functions that implement the 'Static' dialog item class
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


void IhStInitText(IHSTATIC *pDat, char *s)
{
   /* if there's an old string, free it */
   if(pDat->Text) free(pDat->Text);

   /* copy new one */
   pDat->Text = strdup(s);
}

int IhStInit(DLGITEM *pItm, int GotFocus)
{
   IHSTATIC *pDat;              /* points to private data of item */
   IHSTINIT *pIni = pItm->Init; /* points to eventual init data   */

   /* try to allocate private data, return FALSE on failure */
   if(!(pDat = malloc(pItm->Size))) return FALSE;

   /* if Ini structure there, use its font, otherwise default font */
   pDat->Font = pIni ? pIni->Font : MEDIUM_FONT;

   /* if Ini structure there, use its background color, otherwise white */
   pDat->BckGnd = pIni ? pIni->BckGnd : WHITE_COLOR;

   /* store pointer to private data in item structure */
   pItm->Data = pDat;

   pDat->Text = NULL; /* NULL means use 'Text' from item descriptor */

   /* that's all folks */
   return TRUE;
}

int IhStSetItem(DIALOG *pDlg, DLGITEM *pItm, int n, void *p)
{
   IHSTATIC *pDat = pItm->Data; /* point to private data */

   switch(n) { /* set what data ? */
      case STSI_LBL: /* only settable value currently: the label */
         /* set new value */
         IhStInitText(pDat, p);
         break;
      default: return FALSE;
   }
   /* if dialog has been shown already, update item */
   if(pDlg->pWin) (pItm->ItmProc)(IM_SHOW, 0, NULL, pDlg, pItm);

   return TRUE;
}

int IhStShow(DIALOG *pDlg, DLGITEM *pItm)
{
   IHSTATIC *pDat = pItm->Data;  /* point to private data of item */
   int  x, y, w, h;
   char *s;

   /* compute absolute position of item */
   x = pDlg->pWin->PosX + pItm->PosX;
   y = pDlg->pWin->PosY + pItm->PosY;


   /* use pDat->Text if present, else Text from item itself */
   if(!(s = pDat->Text)) s = pItm->Text;

   if(s) { /* if there is a text at all, compute dimensions */
      /* compute width and height of text */
      w = TextExt(pDat->Font, s);
      h = FontHeight(pDat->Font);
   }

   /* Clear area */
   ClrBlock(x, y, x+pItm->Width-1, y+pItm->Depth-1, pDat->BckGnd);

   /* output text */
   if(s) {
      int xp;

      xp = (pItm->Flags & IF_STLFT) ? x :
                                      x+(pItm->Width-w)/2;

      TextOut(xp, y+(pItm->Depth-h)/2, XOR_RULE, pDat->Font, s);
   }

   return TRUE;
}

int IhStKill(DLGITEM *pItm)
{
   IHSTATIC *pDat = pItm->Data;  /* point to private data of item */

   /* if a text has been set, free it ! */
   if(pDat->Text) free(pDat->Text);

   /* and free private data */
   free(pDat);

   return TRUE;
}


int IhStatic(int Msg, int n, void *p, DIALOG *pDlg, DLGITEM *pItm)
{
   switch(Msg) {
      case IM_INIT:      return IhStInit(pItm, n);
      case IM_LOSEFOCUS: return TRUE;
      case IM_KILL:      return IhStKill(pItm);
      case IM_SHOW:      return IhStShow(pDlg, pItm);
      case IM_SETDATA:   return IhStSetItem(pDlg, pItm, n, p);

      case IM_GETDATA:
      case IM_SETFOCUS:
      case IM_KEY:
      default:           return FALSE;
   }
}

