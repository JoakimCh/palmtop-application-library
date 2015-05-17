/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  CLISTI.C
   Author:  Andreas Garzotto
   Started: Aug. 27 1995
   Subject: Enable / disable interrupts
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                          standard includes
   -------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdio.h>

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
      AsmSTI, AsmCLI: Dirty hack to get a portable implementation for
                      the STI and CLI opcodes. Used by some battery
                      functions.     A. Garzotto
   -------------------------------------------------------------------- */

void AsmSTI(void)
{
   static BYTE opcode[] = {0xFB, /* STI */
                           0xCB  /* RETF */ };
   void (far *Vector)(void);

   Vector = (void (far *)(void))opcode;
   (*Vector)();
}

void AsmCLI(void)
{
   static BYTE opcode[] = {0xFA, /* CLI */
                           0xCB  /* RETF */ };
   void (far *Vector)(void);

   Vector = (void (far *)(void))opcode;
   (*Vector)();
}

