/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  GETKEY.C
   Author:  Harry Konstas/Gilles Kohl
   Started: 10. Nov. 94
   Subject: Get a key (with ASCII and Scan code)
   Modif:   GK941119: Added PushKeys function
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
                           constants
   -------------------------------------------------------------------- */
#define MAX_KEYSTACK 10

/* --------------------------------------------------------------------
                           global variables
   -------------------------------------------------------------------- */
static WORD *KStack[MAX_KEYSTACK];
static int  KStackPtr = -1;


/* --------------------------------------------------------------------
                           Functions
   -------------------------------------------------------------------- */


/* --------------------------------------------------------------------
   PushKeys
   Push a sequence of Keys which GetKey will deliver - parameter is
   a pointer to a 0-terminated array of WORDs.
   -------------------------------------------------------------------- */
void PushKeys(WORD *pKeys)
{
   if(KStackPtr >= MAX_KEYSTACK-1) exit(-1); /* panic */
   KStack[++KStackPtr] = pKeys;
}

/* --------------------------------------------------------------------
   KeyWaiting
   Returns TRUE if a key is waiting, either via PushKeys or in the
   hardware.
   -------------------------------------------------------------------- */
int KeyWaiting(void)
{
   int i;
   /* Vector is a far pointer to a function that has no parameters
      and returns an int */
   int (far *Vector)(void);

   /* Here we use a rather nasty device - since one of the supported
      compilers doesn't have inline assembly, we fill an array with
      a 'preassembled' piece of code where we will patch parameters
      into. This code is then called by a far call. */

   static BYTE kwait_asm[] = {
      0xB8, 0x00, 0x00,  /* 0: MOV  AX,Value */
      0xCD, 0x16,        /* 3: INT  16H      */
      0x9C,              /* 5: PUSHF         */
      0x58,              /* 6: POP AX        */
      0xCB               /* 7: RETF         */
   };

   if(KStackPtr > -1) {
      for(i = KStackPtr; i >= 0; --i) if(*KStack[i]) return TRUE;
   }
   /* Now patch the assembly code above to correctly set up register
      values. The PARAM macro is given the address of an instruction,
      and will yield a pointer to the parameter word immediately
      following the instruction byte - this is used for assignment
      here.  */
   PARAM(kwait_asm + 0) = Palmtop ? 0x1100 : 0x0100;

   /* Now, let 'Vector' point to our assembly language routine. We could
      also directly cast the pointer and call the routine with another
      macro, but TC 2.0 won't correctly compile such a construct. Hence
      the detour via another variable. */
   Vector = (int (far *)(void))kwait_asm;

   /* Finally, call the routine, indirectly via the vector */
   return ((*Vector)() & 64) == 0;
}


/* --------------------------------------------------------------------
   GetKey:
   Will wait for a keystroke and return it in both ASCII and scancode.
   The upper byte of the result will hold the scancode, the lower byte
   the ASCII code. This function can also detect the palmtop-specific
   keys like MENU.
   See also PushKeys above for a description of the pNextKey pointer.
   -------------------------------------------------------------------- */

WORD GetKey(void)
{
   union REGS inregs,outregs;
   struct SREGS segregs;
   WORD Key;

   if(KStackPtr > -1) {
      do {
         Key = *KStack[KStackPtr]++;
         if(Key == 0) --KStackPtr;
      } while(Key == 0 && KStackPtr > -1);
      if(Key) return Key;
   }

   inregs.x.ax = Palmtop ? 0x1000 : 0x0000;
   int86x(0x16, &inregs, &outregs, &segregs);
   RestoreState(&PalState);      /* restore graphics settings */
   return outregs.x.ax;
}

