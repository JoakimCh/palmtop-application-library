/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  csvc.c
   Author:  Ron Crain/Gilles Kohl
   Started: Tue 11-22-1994
   Subject: Allows calls to SysMgr in combination with c_service macro

   Thanks to OBA (our buddy Andy G) for coaching, and the original system
   manager authors, whose code is paraphrased here!

   --------------------------------------------------------------------*/

/* --------------------------------------------------------------------
   These routines provide access to the system manager.  Since
   they rely on interrupt 7Eh, they will only function when the
   application is launched from system manager, or under CPACK.

   INT 7E is used by other software packages.  Two that are known
   are DOS port of the GNU C/C++ compiler, and the LoadHI utility
   from Helix.
   --------------------------------------------------------------------*/

/* --------------------------------------------------------------------
                         local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"
#include "palsmsvc.h"

/* --------------------------------------------------------------------
                         global variables
   -------------------------------------------------------------------- */

/* The assembly routine that calls SysMgr, as an initialized array */
BYTE c_service_asm [] = {
  0x55,             /* PUSH BP           */
  0x89, 0xE5,       /* MOV BP,SP         */
  0x87, 0x7E, 0x06, /* XCHG DI,[BP+06]   */
  0x87, 0xC7,       /* XCHG AX,DI        */
  0x5D,             /* POP BP            */
  0xCD, 0x7E,       /* INT 7E            */
  0x55,             /* PUSH BP           */
  0x89, 0xE5,       /* MOV BP,SP         */
  0x87, 0x7E, 0x06, /* XCHG DI,[BP+06]   */
  0x5D,             /* POP BP            */
  0xCB              /* RETF              */
};

/* pSvc is a pointer into our assembly routine that calls SysMgr - we
   can't call c_service_asm directly because of a bug in TC 2.0 */
PCSVC    pCsvc    = (PCSVC)   c_service_asm;
PCSVC_FP pCsvc_fp = (PCSVC_FP)c_service_asm;


