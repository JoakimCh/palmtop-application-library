/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  ADDCFG.C
   Author:  Bob Chernow
   Started: 10/14/95
   Subject: Add Key=Value to config file
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
                           constant declarations
   -------------------------------------------------------------------- */
#define REPLACE 0x01
#define ADD     0x02

/* --------------------------------------------------------------------
                           functions
   -------------------------------------------------------------------- */

/* ------------------------InsertConfigEntry -------------------------- */
/* Builds a config entry, inserts it After pCfg, adjusts line numbers.
*/
void InsertConfigEntry(char *Section, char *Key, char *Value, CONFIGENTRY *pCfg)
{
  CONFIGENTRY *pNew;

  if ((pNew = (CONFIGENTRY *)malloc( sizeof(CONFIGENTRY))) == NULL ) {
      fprintf(stderr, "out of mem in InsertConfigEntry\n");
      exit(-1);
  }

  /* Slide this new entry after pCfg
  */
  pNew->Prev = pCfg;
  pNew->Next = pCfg->Next;

  if (pCfg->Next) {
   pCfg->Next->Prev = pNew;
  }
  pCfg->Next = pNew;

  /* Fill in its data
  */

  /* We only want to allocate a new section string if this is a new section
  *  entry. This is determined by comparing the section and the value. If they
  *  are the same we have a new section. If not, use the section of the
  *  entry passed in.
  */
  if (strcmp(Section,Value) == 0) {
    pNew->Section = strdup(Section);
  } else {
    pNew->Section = pCfg->Section;
  }

  if ((pNew->Key = strdup(Key))== NULL) {
      fprintf(stderr, "out of mem in InsertConfigEntry\n");
      exit(-1);
  }

  if ((pNew->Value = strdup(Value))== NULL) {
      fprintf(stderr, "out of mem in InsertConfigEntry\n");
      exit(-1);
  }

  pNew->LineNbr = pCfg->LineNbr + 1;

  /* Adjust last entry pointer
  */
  if (pCfg == CfgLast){
      CfgLast = pNew;
  }

  /* Update following line numbers
  */
  pNew = pNew->Next;
  while (pNew) {
      pNew->LineNbr++;
      pNew = pNew->Next;
  }
}

/* ----------------------------- _AddConfigString --------------------- */
/* Adds a Key=Value pair to the specified Section.
*  If flag = ADD then the pair is added as the last pair for that section.
*  If flag = REPLACE then all Keys for the section are searched and the first
*            match found is replaced.
*  In all cases if the section does not exist, it is created.
*/
void _AddConfigString(char *Section, char *Key, char *Value, int flag )
{
   CONFIGENTRY *pCfg = CfgList;
   CONFIGENTRY *pKeyCfg;
   int fFound = FALSE, fDone = FALSE;

   /* Find start of this section
   */
   while (pCfg) {
      if (strcmp(Section,pCfg->Section) == 0) {
         fFound = TRUE;
         break;
      }
      pCfg = pCfg->Next;
   }


   /* Section Found
   */
   if (fFound) {
      /* Find start of next section, if we pass by the key, and if
      *  we are in replace mode then replace it.
      */
      while (pCfg && !fDone) {
         if(strcmp(Section,pCfg->Section) != 0){
            break;
         }
         /* found the Key
         */
         if (strcmp(Key, pCfg->Key) == 0 && flag & REPLACE) {
            if(pCfg->Value) free(pCfg->Value);
            pCfg->Value = strdup(Value);
            fDone = TRUE;
         }
         pCfg = pCfg->Next;
      }
      /* We are either at the very end or the next section or we replaced
      *  the Value for a key. If we replaced a Key, we are done. If not,
      *  we need to back up to the last non-blank line, add the key.
      */
      if (!fDone) {
         /* if we are past the end back up
         */
         if (!pCfg) {
            pCfg = CfgLast;
         }

         /* Walk back to a non blank key
         */
         while (strlen(pCfg->Key) == 0) {
            pCfg = pCfg->Prev;
         }

         /* Insert the new Key
         */
         InsertConfigEntry(Section, Key, Value, pCfg);
      }
   }
   /* Need to create new section
   */
   else {
     InsertConfigEntry(Section, "", Section, CfgLast);
     InsertConfigEntry(Section, Key, Value, CfgLast);
   }
}

/* ------------------------------ AddConfigString --------------------- */

void AddConfigString(char *Section, char *Key, char *Value )
{
   _AddConfigString(Section, Key, Value, ADD );
}
/* ---------------------------ReplaceConfigString --------------------- */

void ReplaceConfigString(char *Section, char *Key, char *Value )
{
   _AddConfigString(Section, Key, Value, REPLACE );
}

/* ------------------------------ AddConfigInt -- --------------------- */

void AddConfigInt(char *Section, char *Key, int Value )
{
   char buf[20];
   _AddConfigString(Section, Key, itoa(Value,buf,10) , ADD );
}
/* ---------------------------ReplaceConfigInt ------------------------ */

void ReplaceConfigInt(char *Section, char *Key, int Value )
{
   char buf[20];
   _AddConfigString(Section, Key, itoa(Value,buf,10), REPLACE );
}

