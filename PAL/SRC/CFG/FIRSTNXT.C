/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  FIRSTNXT.C
   Author:  GK
   Started: 12/03/94
   Subject: Get first and subsequent entries from a CFG section
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                       standard includes
   -------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* --------------------------------------------------------------------
                         local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"

/* --------------------------------------------------------------------
                           functions
   -------------------------------------------------------------------- */

/* ----------------------------- GetFirstEntry ----------------------------- */
/* Used to search entire sections. Initiates search.                         */

char *GetFirstEntry(char *Section, char **pKey)
{
   CONFIGENTRY *pCfg;

   for(pCfg = CfgList; pCfg; pCfg = pCfg->Next) {
      if(strcmp(Section, pCfg->Section) == 0 &&
         (pKey) &&
         strlen(pCfg->Key)) {

         TmpEntry   = pCfg;
         TmpSection = Section;
         if(pKey) *pKey = pCfg->Key;
         return pCfg->Value;
      }
   }
   TmpEntry   = NULL;
   TmpSection = NULL;
   return NULL;
}

/* ----------------------------- GetNextEntry ----------------------------- */
/* Used to search entire sections. Continues search.                        */

char *GetNextEntry(char **pKey)
{

   do {
      /* Return NULL if the current entry, the current section, the next entry
      *  is NULL or the next section no longer matches
      */
      if(TmpEntry       == NULL ||
         TmpSection     == NULL ||
         TmpEntry->Next == NULL ||
         strcmp(TmpEntry->Next->Section, TmpSection) != 0) {

         TmpEntry   = NULL;
         TmpSection = NULL;
         return NULL;
      }

      /* Since we have a next entry point to it
      */
      TmpEntry = TmpEntry->Next;

      /* The section must match, but it might be a comment or blank line
      */
      if((pKey) && (strlen(TmpEntry->Key) != 0)) {
         *pKey = TmpEntry->Key;
         return TmpEntry->Value;
      }
   } while( strcmp(TmpEntry->Next->Section, TmpSection) == 0);

   /* if we get here the section changed.
   */
   return NULL;
}

/* ----------------------------- GetSectionEntry ----------------------------- */
/* Used to search entire config file for sections. Initiates search.                         */

char *GetFirstSection( void )
{
   CONFIGENTRY *pCfg;

   for(pCfg = CfgList; pCfg; pCfg = pCfg->Next) {
      /* New section
      */
      if(strcmp(pCfg->Section, pCfg->Value) == 0 &&
         strlen(pCfg->Key) == 0) {

         TmpSectionEntry   = pCfg;
         return pCfg->Section;
      }
   }
   TmpSectionEntry = NULL;
   return NULL;
}

/* ----------------------------- GetNextSection ----------------------------- */
/* Used to search entire config file for sections. Continues search.                        */

char *GetNextSection( void )
{
   CONFIGENTRY *pCfg;

   for(pCfg = TmpSectionEntry->Next; pCfg; pCfg = pCfg->Next) {
      /* New section
      */
      if(strcmp(pCfg->Section, pCfg->Value) == 0 &&
         strlen(pCfg->Key) == 0) {

         TmpSectionEntry   = pCfg;
         return pCfg->Section;
      }
   }
   TmpSectionEntry = NULL;
   return NULL;
}
