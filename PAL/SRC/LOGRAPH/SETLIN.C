/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  SETLIN.C
   Author:  Harry Konstas/Gilles Kohl
   Started: 10. Nov. 94
   Subject: Set line type
   Modified:21 May 1995 by HK
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                          standard includes
   -------------------------------------------------------------------- */
#include <stdlib.h>

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
   SetLineType:
   Sets the graphics line type. The line_type argument
   may be an integer value between [0-65535]. The bit pattern
   representing this line will repetitively been used to draw the line.
   -------------------------------------------------------------------- */

void SetLineType(unsigned int LineType)
{
   PalState.sh_Ltype = LineType;    /* save current state */
   CallInt(0x5f, 0x0b00, 0, LineType, 0, NULL);
}


