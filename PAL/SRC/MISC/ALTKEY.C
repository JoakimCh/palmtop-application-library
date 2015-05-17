/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  ALTKEY.C
   Author:  Harry Konstas/Gilles Kohl
   Started: 10. Nov. 94
   Subject: Return code for ALT-<Letter>, given letter
   Modified by A.Garzotto 11-NOV-95: digits added
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                            standard includes
   -------------------------------------------------------------------- */
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
                           constants
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                           global variables
   -------------------------------------------------------------------- */
static BYTE AltCodes[] = {
   0x1e, 0x30, 0x2e, 0x20, 0x12, 0x21, 0x22, 0x23, 0x17, 0x24,
   0x25, 0x26, 0x32, 0x31, 0x18, 0x19, 0x10, 0x13, 0x1f, 0x14,
   0x16, 0x2f, 0x11, 0x2d, 0x15, 0x2c };

/* --------------------------------------------------------------------
                           Functions
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
   AltKey:
   Subject: Return code for ALT-<Letter>, given letter
   Will return zero for characters outside the A-Z, 0-9 range
   ------------------------------------------------------------------- */
WORD AltKey(unsigned Letter)
{
   Letter = toupper(Letter);

   if (Letter == '0') return 0x8100;
   if((Letter >= '1') && (Letter <= '9'))
      return (WORD) (0x78 + Letter - '1') << 8;
   if(Letter < 'A' || Letter > 'Z') return 0;
   return (WORD)AltCodes[Letter-'A'] << 8;
}

