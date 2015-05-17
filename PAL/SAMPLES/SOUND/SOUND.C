/* --------------------------------------------------------------------
   Project: Sound test program
   Module:  sound.c
   Author:  Gilles Kohl
   Started: December 5, 1994
   Subject: Test program - access SysMgr sounds
   -------------------------------------------------------------------- */

#include <stdio.h>

#include "pal.h"
#include "palsmsvc.h"

/* an alarm sound - Beethoven's "Fate knocking at the door" with
   increasingly high notes */
char *Sound = "T150 O3 K1 L64 GGG L5E- P8 L64 FFF L5D P1,"
              "O4 K1 L64 GGG L5E- P8 L64 FFF L5D"
              "P1, O5 K1 L64 GGG L5E- P8 L64 FFF L5D";

void main (void)
{
   PalInit(0);
   if(!CheckSysMgr()) FatalExit("SysMgr not loaded.", 1);
   m_play(Sound);
   PalDeInit(0);
}

