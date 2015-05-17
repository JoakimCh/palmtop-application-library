/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  PALWIN.C
   Author:  Gilles Kohl
   Started: 10. Nov. 94
   Subject: Open and Close Window functions
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                          standard includes
   -------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* --------------------------------------------------------------------
                            msdos includes
   -------------------------------------------------------------------- */
#include <dos.h>

/* --------------------------------------------------------------------
                           local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"

/* --------------------------------------------------------------------
                           global variables
   -------------------------------------------------------------------- */

/* Fill mask definition (for window shadows) */
BYTE ShadowMask[]= { 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0 };

/* --------------------------------------------------------------------
                           Functions
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
   ShrinkClientArea:
   Reduce the client area (the non-system area of a window) in size
   both horizontally and vertically by a certain amount.
   (Internal utility function, not exported)
   -------------------------------------------------------------------- */

static void ShrinkClientArea(PALWIN *pWin, int Amount)
{
   pWin->PosX  += Amount;
   pWin->PosY  += Amount;
   pWin->Width -= 2*Amount;
   pWin->Depth -= 2*Amount;
}


/* --------------------------------------------------------------------
   OpenWin:
   Open a window, returning a pointer to a PALWIN descriptor structure.
   The window is given by upper left (x1,y1) and lower right (x2,y2)
   coordinates, and can have an optional Caption (or NULL for none)
   -------------------------------------------------------------------- */

PALWIN *OpenWin(WORD Style, int x1, int y1, int x2, int y2, char *Caption)
{
   int n;
   unsigned int buflen, horlen, verlen;
   PALWIN *pWin;

   if(!PalStateSaved++) SaveState(&PalState);

   /* allocate storage for window descriptor structure */
   pWin = malloc(sizeof(PALWIN));
   if(!pWin) return NULL;

   /* remember stuff about the window */
   pWin->Style = Style; /* collection of flags that determine appearance   */
   pWin->Font  = (Style & WS_SMLFONT) ?  SMALL_FONT : MEDIUM_FONT;

   pWin->x1    = x1;    /* physical boundaries of window - including frame */
   pWin->y1    = y1;    /* and border, but not shadow                      */
   pWin->x2    = x2;
   pWin->y2    = y2;

   pWin->PosX  = x1;    /* Client area starts as the full window, */
   pWin->PosY  = y1;    /* will be adjusted for border or frame   */

   pWin->Width = x2-x1+1; /* Width and Depth will be adjusted too */
   pWin->Depth = y2-y1+1;

   if(Style & WS_SAVEBG) { /* save the background ? */
      /* compute the REAL physical extent, to save in image buffer */
      horlen = x2-x1+1 + ((Style & WS_SHADOW) ? SHADOWWIDTH : 0);
      verlen = y2-y1+1 + ((Style & WS_SHADOW) ? SHADOWDEPTH : 0);
      buflen = 8+((horlen+7)/8) * verlen; /* compute storage needed */

      pWin->Buffer = malloc(buflen);
      if(!pWin->Buffer) FatalExit("Out of memory", 1);
      /* abort if not enough memory */

      /* save background of window to create */
      GetImg(x1, y1,
             min(x1+horlen-1, MAX_X-1), min(y1+verlen-1, MAX_Y-1),
             pWin->Buffer);
   }

   SetMask(ShadowMask);
   SetRule(FORCE_RULE);

   if(Style & WS_SHADOW) { /* draw a shadow ? */
      SetColor(WHITE_COLOR);
      Rectangle(x2+1, y1+4, x2+6, y2+4, SOLID_FILL);
      Rectangle(x1+5, y2+1, x2+6, y2+4, SOLID_FILL);

      SetColor(BLACK_COLOR);
      Rectangle(x2+1,             y1+SHADOWDEPTH,
                x2+SHADOWWIDTH,   y2+SHADOWDEPTH, PATTERN_FILL);
      Rectangle(x1+SHADOWWIDTH,   y2+1,
                x2+SHADOWWIDTH,   y2+SHADOWDEPTH, PATTERN_FILL);
   }

   /* Clear client area of window */
   SetColor(WHITE_COLOR);
   Rectangle(x1, y1, x2, y2, SOLID_FILL);

   /* Draw border */
   SetColor(BLACK_COLOR);
   for(n = (Style & WS_BMASK)-1; n >= 0; --n) {
      Rectangle(x1+n, y1+n, x2-n, y2-n, OUTLINE_FILL);
   }
   ShrinkClientArea(pWin, Style & WS_BMASK);

   if(Caption) {
      int x;
      int y;
      int FntWidth = FNTW(pWin->Font);
      int FntDepth = FNTD(pWin->Font);
      int TotDepth = max(FntDepth+1, Style & WS_BMASK);

      SelectFont(pWin->Font);
      x = x1 + (x2 - x1 - strlen(Caption)*FntWidth)/2;
      y = y1 + (TotDepth - FntDepth)/2+1;
      Rectangle(x1, y1, x2, y1+TotDepth-1, SOLID_FILL);
      pWin->PosY   = y1+TotDepth;
      pWin->Depth -= TotDepth-(Style & WS_BMASK);
      SetRule(XOR_RULE);
      WriteText(x, y, Caption);
   }
   if(!--PalStateSaved) RestoreState(&PalState);
   return pWin;
}


/* --------------------------------------------------------------------
   CloseWin:
   Close a window given by a pointer to a PALWIN descriptor structure.
   The background will only be restored if the corresponding style bit
   indicates that this is required.
   -------------------------------------------------------------------- */

void CloseWin(PALWIN *pWin)
{
   /* is there a saved background to restore ? */
   if(pWin->Style & WS_SAVEBG) {
      PutImg(pWin->x1, pWin->y1, FORCE_RULE, pWin->Buffer);
      free(pWin->Buffer);
   }
   free(pWin);
}


