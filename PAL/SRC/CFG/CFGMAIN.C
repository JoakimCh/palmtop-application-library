/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  CFGMAIN.C
   Author:  GK
   Started: 12/03/94
   Subject: Main configuration file management routines
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
                        global variables
   -------------------------------------------------------------------- */
CONFIGENTRY *CfgList  = NULL;
CONFIGENTRY *CfgLast  = NULL;
CONFIGENTRY *TmpEntry = NULL;
CONFIGENTRY *TmpSectionEntry = NULL;
char PrologSection[64];

char *TmpSection = NULL;
char *CurSection = "";

/* --------------------------------------------------------------------
                           functions
   -------------------------------------------------------------------- */

/* ------------------------ AddConfigEntry ---------------------------- */
/* Add an entry to the config list                                      */

static void AddConfigEntry(char *Section, char *Key, char *Value, int Line)
{
   CONFIGENTRY *pCfg;

   if((pCfg = malloc(sizeof(CONFIGENTRY))) == NULL) {
      fprintf(stderr, "out of mem in AddConfigEntry\n");
      exit(-1);
   }
   pCfg->Section = Section;
   pCfg->Key     = Key;
   pCfg->Value   = Value;
   pCfg->LineNbr = Line;
   pCfg->Next    = NULL;

   /* Is this the start of the list
   */
   if(CfgList == NULL) {
      CfgList = pCfg;
      CfgList->Prev = NULL;
   }
   /* Add a node, and link nodes.
   */
   else {
      CfgLast->Next = pCfg;
      pCfg->Prev = CfgLast;
   }

   /* Nice to have a pointer to the end of the list.
   */
   CfgLast = pCfg;
}

/* ------------------------------ StripSpace ------------------------------ */
/* Remove white space from start and end of string                          */
static char *StripSpace(unsigned char *s)
{
   unsigned char *e;

   for(; isspace(*s); ++s);
   if(!*s) return s;
   for(e = s + strlen(s)-1; e > s && isspace(*e); --e) *e = '\0';
   return s;
}

/* ------------------------------ StripQuotes ------------------------------ */
/* Remove quotes from string if present                                      */

static char *StripQuotes(char *s)
{
   char *q;
   if(strlen(s) < 2) return s;
   q = s + strlen(s)-1;
   if(*s == '\'' && *q == '\'') {
      ++s;
      *q = '\0';
   }
   return s;
}

/* ------------------------------ NewConfig ------------------------------- */
/* Initialize structures for a new in-memory config file                    */

void NewConfig(char *CfgName)
{
   /* Create Prolog Section Header
   */
   strcpy(PrologSection, CfgName);
   CurSection = strdup(PrologSection);
   AddConfigEntry(CurSection, strdup(""), strdup(CurSection), 0);
}

/* ------------------------------ ReadConfig ------------------------------ */
/* Read a configuration file into memory.                                   */

void ReadConfig(char *CfgName)
{
   FILE *f;
   int Line = 0;
   char buf[256];
   char *s, *p;

   if((f = fopen(CfgName, "r")) == NULL) return;

   /* Create Prolog Section Header
   */
   NewConfig(CfgName);

   while(s = fgets(buf, sizeof(buf), f)) {
      ++Line;

      s = StripSpace(s);

      /* New Section found
      */
      if(*s == '[' && s[strlen(s)-1] == ']') {
         ++s;
         s[strlen(s)-1] = '\0';
         CurSection = strdup(s);
      }

      if(p = strchr(s, '=')) { /* found assignment ? */
         *p++ = '\0'; /* insert separator */
         p = StripQuotes(StripSpace(p));
         s = StripSpace(s);
      }
      else
      {
         p = StripQuotes(StripSpace(s));
         s = "";
      }
      AddConfigEntry(CurSection, strdup(s), strdup(p), Line);
   }
   fclose(f);
   return;
}

/* ---------------------------- GetConfigString ---------------------------- */
/* Return pointer to value of give Config entry. If not found return def.    */

char *GetConfigString(char *Section, char *Key, char *Default)
{
   CONFIGENTRY *pCfg;

   for(pCfg = CfgList; pCfg; pCfg = pCfg->Next) {
      if(strcmp(Section, pCfg->Section) == 0  &&
         strcmp(Key, pCfg->Key) == 0) {
         return pCfg->Value;
      }
   }
   return Default;
}

/* ------------------------------ DiscardList ------------------------------ */
/* Free memory used by config list structures.                               */

void DiscardConfig(void)
{
   char *HSection;
   CONFIGENTRY *Ptr, *Ptr2;

   if(CfgList == NULL) return;

   Ptr = CfgList;
   HSection = Ptr->Section;
   free(HSection);


   do {
      if ( Ptr->Section != HSection ) {
         HSection = Ptr->Section;
         free(HSection);
      }
      free(Ptr->Key);
      free(Ptr->Value);
      Ptr2 = Ptr->Next;
      free(Ptr);
      Ptr = Ptr2;
   } while(Ptr != NULL);

   CfgList  = NULL;
   CfgLast  = NULL;
   TmpEntry = NULL;
}


