/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  RESTORE.C
   Author:  Harry Konstas/Gilles Kohl
   Started: May 21 '95
   Subject: Restore graphics settings
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

void SaveState(SAVEDSTATE *pState)
{
   *pState = PalState;
}

void RestoreState(SAVEDSTATE *pState)
{
   SetPos(pState->sh_xpos, pState->sh_ypos);
   SelectFont(pState->sh_font);
   SetLineType(pState->sh_Ltype);
   SetColor(pState->sh_color);
   SetMask(pState->sh_mask);
   SetRule(pState->sh_rule);
}

