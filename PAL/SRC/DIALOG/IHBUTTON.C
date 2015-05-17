/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  IHBUTTON.C
   Author:  Gilles Kohl
   Started: 26. Jan 95
   Subject: Functions that implement the 'Button' dialog item class
   Modified:Aug. 4, 1995 by HK, added curved corners.
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

void IhBnInitText(IHBUTTON *pDat, char *s)
{
   /* if there's an old string, free it */
   if(pDat->Text) free(pDat->Text);

   /* copy new one */
   pDat->Text = strdup(s);
}

int IhBnInit(DLGITEM *pItm, int GotFocus)
{
   IHBUTTON *pDat;              /* points to private data of item */
   IHBNINIT *pIni = pItm->Init; /* points to eventual init data   */

   /* try to allocate private data, return FALSE on failure */
   if(!(pDat = malloc(sizeof(*pDat)))) return FALSE;

   /* if Ini structure there, use its font, otherwise default font */
   pDat->Font = pIni ? pIni->Font : MEDIUM_FONT;

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

int IhBnSetItem(DIALOG *pDlg, DLGITEM *pItm, int n, void *p)
{
   IHBUTTON *pDat = pItm->Data; /* point to private data */

   switch(n) { /* set what data ? */
      case BNSI_LBL: /* only settable value currently: the label */
         /* set new value */
         IhBnInitText(pDat, p);
         break;
      default: return FALSE;
   }
   /* if dialog has been shown already, update item */
   if(pDlg->pWin) (pItm->ItmProc)(IM_SHOW, 0, NULL, pDlg, pItm);

   return TRUE;
}

int IhBnShow(DIALOG *pDlg, DLGITEM *pItm)
{
   IHBUTTON *pDat = pItm->Data;  /* point to private data of item */
   int  x, y, w, h;
   int  i, xx, yy;
   int  d;
   int  focusColor = pDat->Focus ? BLACK_COLOR : WHITE_COLOR;
   char *s;

   /* compute absolute position of item */
   x = pDlg->pWin->PosX + pItm->PosX;
   y = pDlg->pWin->PosY + pItm->PosY;

   /* draw frame */
   SetColor(BLACK_COLOR);
   SetRule(FORCE_RULE);

   xx = x + pItm->Width+1;
   yy = y + pItm->Depth+2;

   /* create frame with extra wide right and bottom */
   for (i = 0; i < 3; i++) {

      d = (pItm->Flags&(long)IF_RET) ? i : 0;
      Rectangle(x+d, y+d, xx-i, yy-i, OUTLINE_FILL);
   }

   /* Add additional diagonal elements */
   Line(x+d,   y+2+d, x+2+d, y+d);
   Line(x+1+d, y+2+d, x+3+d, y+d);

   PlotDot(x+1+d, yy-3);
   PlotDot(xx-3,yy-3);
   PlotDot(xx-3,y+1+d);

   /* Eat away extra pixels */
   SetColor(WHITE_COLOR);
   PlotDot(x,y);
   PlotDot(xx,y);
   PlotDot(x,yy);
   PlotDot(xx,yy);

   Line(x,y+1,x+1,y);
   Line(xx-1,y,xx,y+1);
   Line(xx-1,yy,xx,yy-1);
   Line(x,yy-1,x+1,yy);

   SetColor(BLACK_COLOR);


   /* use pDat->Text if present, else Text from item itself */
   if(!(s = pDat->Text)) s = pItm->Text;

   if(s) { /* if there is a text at all, compute dimensions */
      /* compute width and height of text */
      w = LabelExt(pDat->Font, s);
      h = FontHeight(pDat->Font);
   }

   /* Clear area */
   ClrBlock(x+2+d, y+2+d, xx-4, yy-4, focusColor);  /* Main Text Area */
   ClrBlock(x+1+d, y+3+d, x+1+d, yy-4, focusColor);  /* Left   stripe  */
   ClrBlock(x+3+d, y+1+d, xx-4, y+1+d, focusColor);  /* Top    stripe  */
   ClrBlock(x+2+d,  yy-3, xx-4, yy-3, focusColor);  /* Bottom stripe  */
   ClrBlock( xx-3, y+2+d, xx-3, yy-4, focusColor);  /* Right  stripe  */

   /* output text */
   if(s) LabelOut(x+(pItm->Width-w)/2, y+(pItm->Depth-h)/2+1, XOR_RULE,
                  pDat->Font, s);

   return TRUE;
}

int IhBnKey(DIALOG *pDlg, DLGITEM *pItm, int Key)
{
   IHBUTTON *pDat = pItm->Data;

   if(Key & 0xff) Key &= 0xff;

   switch(Key) {
      case ENTER_KEY:
      case SPACE_KEY: return IN_QUIT;
      default:        return IN_NACK;
   }
}


int IhBnFocus(DIALOG *pDlg, DLGITEM *pItm, int State)
{
   IHBUTTON *pDat = pItm->Data;

   pDat->Focus = State;
   (pItm->ItmProc)(IM_SHOW, 0, NULL, pDlg, pItm);
   return TRUE;
}


int IhBnKill(DLGITEM *pItm)
{
   IHBUTTON *pDat = pItm->Data;

   /* if there's an old string, free it */
   if(pDat->Text) free(pDat->Text);
   free(pDat);
   return TRUE;
}


int IhButton(int Msg, int n, void *p, DIALOG *pDlg, DLGITEM *pItm)
{
   switch(Msg) {
      case IM_INIT:      return IhBnInit(pItm, n);
      case IM_SHOW:      return IhBnShow(pDlg, pItm);
      case IM_SETDATA:   return IhBnSetItem(pDlg, pItm, n, p);
      case IM_GETDATA:   return FALSE;
      case IM_SETFOCUS:
      case IM_LOSEFOCUS: return IhBnFocus(pDlg, pItm, Msg == IM_SETFOCUS);
      case IM_KILL:      return IhBnKill(pItm);
      case IM_KEY:       return IhBnKey(pDlg, pItm, n);
      default:          return FALSE;
   }
}

