/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  PAL_ERR.C
   Author:  Rob Koenis / Gilles Kohl
   Started: Aug. 17 1995
   Subject: Int 24 Error handler
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
                           Functions
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
      NoErr24Handler: Errorhandler to ignore errors
   -------------------------------------------------------------------- */
PALINTHANDLER(NoErr24Handler)
{
   if (pal_ax & 0x0800) pal_ax = 3;   /* Fail */
   else pal_ax = 0;                   /* Ignore error */
}

/* --------------------------------------------------------------------
      SetErrorHandler: Set int 24 handler to be used. HandlerType
                       can be DEF_ERRH or NOE_ERRH.
                       DEF_ERRH is the default DOS error handler.
                       NOE_ERRH will do nothing. If an error occurs
                       a failure will be indicated to the calling
                       routine instead.
   -------------------------------------------------------------------- */

int SetErrorHandler(int HandlerType)
{
   static void INTVECTOR(DefaultInt24Handler) = NULL;

   if(!DefaultInt24Handler && HandlerType != DEF_ERRH) {
      DefaultInt24Handler = getvect(0x24);
   }

   switch (HandlerType) {
      case DEF_ERRH:
         if(DefaultInt24Handler) {
            setvect(0x24, DefaultInt24Handler);
            DefaultInt24Handler = NULL;
         }
         break;
      case NOE_ERRH:
         setvect(0x24, NoErr24Handler);
         break;
      default:
         return 1;
   }
   return 0;
}

