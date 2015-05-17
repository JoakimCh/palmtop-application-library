/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  METER.C
   Author:  Harry Konstas
   Started: Aug. 3 1995
   Subject: Progress / Level meter
   Modified: Sept 4 by HK, fixed bug in UpdateMeter()
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                          standard includes
   -------------------------------------------------------------------- */
#include <stdlib.h>
#include <stdio.h>

#if defined _MSC_VER || defined _QC
#include <malloc.h>
#else
#include <alloc.h>
#endif

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
    OpenMeter: Opens (displays) a level meter. Type can be LEVEL_METER,
               or PROG_METER. Size can be 1-4.
   -------------------------------------------------------------------- */

METER *OpenMeter(int type, int Xpos, int Ypos, int size,
                 WORD BegVal, WORD EndVal, char *title)
{
   METER *pMtr;

   /* defaults on error */
   if((size>4)||(size<1)) size = 3;
   if(Ypos<12) Ypos = 12;
   if(EndVal <= BegVal) return NULL;

   pMtr = malloc(sizeof(METER));
   if(!pMtr) return NULL;

   /* copy args to METER structure */
   pMtr->Xpos   = Xpos;
   pMtr->Ypos   = Ypos;
   pMtr->Lenght = size * 100;
   pMtr->Height = 28;           /* fixed height */
   pMtr->type   = type;
   pMtr->BegVal = BegVal;
   pMtr->EndVal = EndVal;

   /* save background of meter */
   pMtr->Buffer = GetImg(Xpos, Ypos-8, Xpos+pMtr->Lenght+20,
                         Ypos+pMtr->Height+2, NULL);

   if(!pMtr->Buffer) {
      free(pMtr);
      return NULL;
   }

   /* clear screen area */
   SetColor(WHITE_COLOR);
   SetRule(FORCE_RULE);
   Rectangle(Xpos, Ypos, Xpos+pMtr->Lenght+18,
             Ypos+pMtr->Height, SOLID_FILL);
   SetColor(BLACK_COLOR);

   if(type==PROG_METER) {

      /* draw meter outline */
      SetColor(BLACK_COLOR);
      Rectangle(Xpos, Ypos, Xpos+pMtr->Lenght+18,
                Ypos+pMtr->Height, OUTLINE_FILL);
      Rectangle(Xpos+1, Ypos+1, Xpos+pMtr->Lenght+17,
                Ypos+pMtr->Height-1, OUTLINE_FILL);
   }

   /* write meter title */
   SelectFont(MEDIUM_FONT);
   SetRule(TXT_RULE);
   if(title) WriteText(Xpos+10, Ypos-5, title);
   SetRule(FORCE_RULE);

   /* draw progress outline */
   Rectangle(Xpos+8, Ypos+7, Xpos+pMtr->Lenght+10, Ypos+pMtr->Height-7, OUTLINE_FILL);
   return pMtr;
}


/* --------------------------------------------------------------------
    UpdateMeter: Update the level in the level meter. Percent can be
                 a value between BegVal & EndVal.
   -------------------------------------------------------------------- */

void UpdateMeter(METER *pMtr, int Level)
{
   char buffer[80];
   int Percent, level;

   /* argument checkpoint */
   if((Level<pMtr->BegVal)||(Level>pMtr->EndVal)) return;
   if(!pMtr) return;

   Percent = ((long)100 * (Level - pMtr->BegVal)) /
             (pMtr->EndVal - pMtr->BegVal);
   level = (pMtr->Lenght/100) * Percent;

   /* erase previous level */
   SetRule(FORCE_RULE);
   SetColor(WHITE_COLOR);
   Rectangle(pMtr->Xpos+9, pMtr->Ypos+8, pMtr->Xpos+pMtr->Lenght+9,
             pMtr->Ypos+pMtr->Height-8, SOLID_FILL);

   /* draw new level */
   SetColor(BLACK_COLOR);
   if(Percent) {
   Rectangle(pMtr->Xpos+9, pMtr->Ypos+8, pMtr->Xpos+9+level,
             pMtr->Ypos+pMtr->Height-8, SOLID_FILL);
   }

   if(pMtr->type==PROG_METER) {

      /* draw percentage */
      SetRule(XOR_RULE);
      sprintf(buffer,"%d%%", Percent);
      WriteText(pMtr->Xpos+(pMtr->Lenght/2), pMtr->Ypos+10, buffer);
      SetRule(FORCE_RULE);
   }
}


/* --------------------------------------------------------------------
    CloseMeter: closes down an active level meter.
   -------------------------------------------------------------------- */

void CloseMeter(METER *pMtr)
{

   /* argument checkpoint */
   if(!pMtr) return;

   /* restore screen contents */
   PutImg(pMtr->Xpos, pMtr->Ypos-8, FORCE_RULE, pMtr->Buffer);
   free(pMtr->Buffer);
   free(pMtr);
}

