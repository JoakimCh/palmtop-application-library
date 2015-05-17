/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  WRITECFG.C
   Author:  Bob Chernow
   Started: 10/11/95
   Subject: Writes config file
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

/* ------------------------------ WriteConfig ------------------------------ */
/* Writes current configuration from memory to file.                         */

int WriteConfig(char *CfgName)
{
   FILE *f;
   char buf[256];

   CONFIGENTRY *pCfg = CfgList;

   if((f = fopen(CfgName, "w")) == NULL) return 0;

   /* Skip the Prolog Record if it Exists.
   */
   if (strcmp(pCfg->Section, PrologSection) == 0) {
      pCfg = pCfg->Next;
   }
   while(pCfg) {

      if (strlen(pCfg->Key)) {
         /* Handle a valid Key
         */
         sprintf(buf,"%s=%s\n",pCfg->Key, pCfg->Value);
      }
      else if (strcmp(pCfg->Section, pCfg->Value) == 0) {
         /* New Section found
         */
         sprintf(buf,"[%s]\n",pCfg->Section);
      }
      else {
         /* Handle comments & blank lines
         */
         sprintf(buf,"%s\n",pCfg->Value);
      }
      if(fputs(buf, f) == EOF){
         return 0;
      }
      pCfg = pCfg->Next;
   }
   fclose(f);
   return 1;
}
