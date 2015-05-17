/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  IHBITMAP.C
   Author:  Gilles Kohl
   Started: 4. Mar 95
   Subject: Functions that implement the 'Bitmap' dialog item class
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


int IhBmInit(DLGITEM *pItm)
{
   IHBITMAP *pDat;              /* points to private data of item */
   IHBMINIT *pIni = pItm->Init; /* points to init data   */

   /* try to allocate private data, return FALSE on failure */
   if(!(pDat = malloc(pItm->Size))) return FALSE;

   /* store pointer to private data in item structure */
   pItm->Data = pDat;

   /* copy bitmap pointer from INI structure, if any */
   pDat->pBmp = pIni ? pIni->pBmp : NULL;

   /* current bitmap was not loaded dynamically */
   pDat->Free = FALSE;

   /* that's all folks */
   return TRUE;
}

int IhBmSetItem(DIALOG *pDlg, DLGITEM *pItm, int n, void *p)
{
   IHBITMAP *pDat = pItm->Data; /* point to private data */

   switch(n) { /* set what data ? */
      case BMSI_PTR: /* set new bitmap pointer */
         if(pDat->Free) free(pDat->pBmp);
         pDat->pBmp = p;
         break;
      case BMSI_FIL: /* load a bitmap given by name */
         if(pDat->Free) free(pDat->pBmp);
         pDat->Free = !!(pDat->pBmp = LoadImg(p));
         break;
      default: return FALSE;
   }
   /* if dialog has been shown already, update item */
   if(pDlg->pWin) (pItm->ItmProc)(IM_SHOW, 0, NULL, pDlg, pItm);

   return TRUE;
}

int IhBmShow(DIALOG *pDlg, DLGITEM *pItm)
{
   IHBITMAP *pDat = pItm->Data;  /* point to private data of item */
   IMGHDR *pBmp = pDat->pBmp;
   int x, y;
   int xx, yy;

   /* compute absolute position of item */
   x = pDlg->pWin->PosX + pItm->PosX;
   y = pDlg->pWin->PosY + pItm->PosY;

   /* Clear area */
   ClrBlock(x, y, x+pItm->Width-1, y+pItm->Depth-1, WHITE_COLOR);

   if(pBmp) {
      /* center bitmap in area */
      xx = max(0, (pItm->Width - pBmp->Width)/2);
      yy = max(0, (pItm->Depth - pBmp->Depth)/2);

      /* output bitmap */
      PutImg(x+xx, y+yy, FORCE_RULE, pBmp);
   }
   return TRUE;
}

int IhBmKill(DLGITEM *pItm)
{
   IHBITMAP *pDat = pItm->Data;  /* point to private data of item */

   /* if we allocated bitmap, free it too */
   if(pDat->Free) free(pDat->pBmp);

   /* and free private data */
   free(pDat);

   return TRUE;
}


int IhBitmap(int Msg, int n, void *p, DIALOG *pDlg, DLGITEM *pItm)
{
   switch(Msg) {
      case IM_INIT:      return IhBmInit(pItm);
      case IM_LOSEFOCUS: return TRUE;
      case IM_KILL:      return IhBmKill(pItm);
      case IM_SHOW:      return IhBmShow(pDlg, pItm);
      case IM_SETDATA:   return IhBmSetItem(pDlg, pItm, n, p);

      case IM_GETDATA:
      case IM_SETFOCUS:
      case IM_KEY:
      default:           return FALSE;
   }
}

