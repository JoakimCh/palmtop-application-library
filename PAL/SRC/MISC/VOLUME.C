/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  VOLUME.C
   Author:  Harry Konstas / Andreas Garzotto.
   Started: 11/20/1994
   Subject: Set / Get the volume of the internal speaker
   A. Garzotto, 26-AUG-95: Get the volume
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                       standard includes
   -------------------------------------------------------------------- */
#include <stdio.h>
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
                           functions
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
   Volume:
   Sets the volume level of the built-in speaker
   available definitions: (VOL_OFF, VOL_LOW, VOL_MEDIUM, VOL_HI)
   -------------------------------------------------------------------- */

void Volume(int Level)
{
    union REGS regs;
    struct SREGS segregs;

    regs.h.al = Level;
    regs.h.ah = 0x48;
    int86x(0x15, &regs, &regs, &segregs);
}

/* --------------------------------------------------------------------
   GetVolume:
   Returns the volume setting level of the built-in speaker
   Return values: (VOL_OFF, VOL_LOW, VOL_MEDIUM, VOL_HI)
   -------------------------------------------------------------------- */

int GetVolume(void)
{
   unsigned int i;
   int vol;

   i = inportb(0x22);          /* save hornet index register */
   outportb(0x22, 0x1E);       /* select register holding volume setting */
   vol = inportb(0x23);        /* and read setting */
   outportb(0x22, i);          /* restore previous index register setting */
   return (vol >> 6); 
}

