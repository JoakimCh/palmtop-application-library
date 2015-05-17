/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  ISAPPACT.C
   Author:  Harry Konstas
   Started: 17. Oct. 95
   Subject: Check if an application is active (by looking into TCB)
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                       standard includes
   -------------------------------------------------------------------- */

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

/* --------------------------------------------------------------------
                         local includes
   -------------------------------------------------------------------- */

#include "pal.h"
#include "palsmsvc.h"

/* --------------------------------------------------------------------
                       Get & display TCB entries
   -------------------------------------------------------------------- */

int IsAppActive(WORD KeyCode)
{
   unsigned int i, size;
   struct task far *pTCB;     /* TCB pointer */

   if(!CheckSysMgr()) return FALSE; /* SysMgr not loaded, abort */

   pTCB = m_get_TCB();        /* get TCB */
   size = (int)m_get_TCB_size();   /* get TCB size */

   /* check applications one by one in the TCB table */
   for(i = 0; i < size; i++) {
      /* check if app. hotkey matches KeyCode and if it's active */
      if((pTCB->t_hotkey == KeyCode) && (pTCB->t_state)) return TRUE;
      pTCB++;
   }
   return FALSE;
}

