/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  PALINIT.C
   Author:  Gilles Kohl
   Started: 10. Nov. 94
   Subject: PAL Initialization and deinitialization
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                          standard includes
   -------------------------------------------------------------------- */
#include <stdlib.h>
#include <process.h>

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
                        global variables
   -------------------------------------------------------------------- */
int PalStateSaved = 0;
SAVEDSTATE PalState;

int Palmtop  = 0;
static int CgaGraph = 0;
static char Copyright[] = "Portions Copyright, 1995, The PAL Group";

/* --------------------------------------------------------------------
                           functions
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
   WhichHp:
   Returns the model number of the palmtop we're running on.
   Currently supported:  0 --> no palmtop, PC
                        95 --> HP95LX
                       100 --> HP100LX
   -------------------------------------------------------------------- */

int WhichHp(void)
{
   union REGS r;

   r.x.ax = 0x4dd4;

   int86(0x15, &r, &r);
   if(r.x.bx == 0x4850) switch(r.x.cx) {
      case 0x0101: return  95;
      case 0x0102: return r.h.dh ? 200 : 100;
      default: return 0; /* unknown - state 'no palmtop' */
   }
   return 0; /* no palmtop */
}

/* --------------------------------------------------------------------
   PalInit:
   Initialize PAL library.
   Sets 'Palmtop' variable to non-zero if running on an HP palmtop.
   Any other PAL 'init' stuff will go here. Calling this function is
   mandatory.
   -------------------------------------------------------------------- */

int PalInit(int Graphics)
{
   char *pCga;

   if(Graphics && !Check5f() && (pCga = getenv("CGAGRAPH"))) {
      if(sizeof(char *) == 2) { /* small model ? */
         spawnl(P_WAIT, pCga, pCga, NULL);
         CgaGraph = 1;
      }
   }
   Palmtop = WhichHp();
   if(Graphics && !GraphMode()) return FALSE;

   /* Set default graphics state */
   SetColor(BLACK_COLOR);
   SetRule(FORCE_RULE);
   SetLineType(0xffff);
   SelectFont(MEDIUM_FONT);
   SetPos(0,0);
   return TRUE;
}


/* --------------------------------------------------------------------
   PalDeInit:
   Shut down PAL library.
   Add more 'cleanup' type stuff here.
   -------------------------------------------------------------------- */
void PalDeInit(int Text)
{
   char *pCga;

   if(Text) TextMode();
   if(CgaGraph) {
      if(pCga = getenv("CGAGRAPH")) {
         spawnl(P_WAIT, pCga, pCga, NULL);
      }
   }
}

