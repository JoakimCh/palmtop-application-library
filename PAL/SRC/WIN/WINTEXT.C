/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  WINTEXT.C
   Author:  Gilles Kohl
   Started: 10. Nov. 94
   Subject: Output text in client area of a window
   -------------------------------------------------------------------- */

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
                           Functions
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
   WinText:
   Output a text string at position (x,y) in client area of a window
   given by a pointer to a PALWIN descriptor.
   -------------------------------------------------------------------- */

void WinText(PALWIN *pWin, int OffsetX, int OffsetY, char *Txt)
{
   WriteText(pWin->PosX+OffsetX, pWin->PosY+OffsetY, Txt);
}


