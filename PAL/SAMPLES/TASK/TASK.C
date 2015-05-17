/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  TASK.C
   Author:  HK
   Started: 17. Oct. 95
   Subject: TCB (Task Control Block) info retrieval sample

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

void main(void) {

   unsigned int i, size;
   struct task far *pTCB;     /* TCB pointer */

   if(!CheckSysMgr()) FatalExit("SysMgr not loaded.", 1);

   pTCB = m_get_TCB();        /* get TCB */
   size = (int)m_get_TCB_size();   /* get TCB size */

   /* Notify us if the phonebook application is active */
   if(IsAppActive(0xB400)==TRUE)
      printf("Phonebook Application is active.\n");

   printf("TCB = %08lX, size = %d\n", pTCB, size);
   for(i = 0; i < size; i++) {
      char far *s;

      printf("Index: %d  Hotkey: 0x%04X State: %d ",
               i, pTCB->t_hotkey, pTCB->t_state);

      printf("Name:");
      for(s = &pTCB->t_extname[0]; *s; ++s) putchar(*s);
      printf("\n");
      pTCB++;
   }
}
