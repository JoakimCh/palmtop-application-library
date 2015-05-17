/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  DELCFG.C
   Author:  Bob Chernow
   Started: 10/14/95
   Subject: Delete Key=Value from config file
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

/* ------------------------RenumberLines -------------------------- */
/* Since we may have to delete many entries it is not efficient to
*  renumber after each delete, so this is called after last delete.
*/
void RenumberLines(void)
{
   CONFIGENTRY *pCfg = CfgList;
   int i;

   for (i = 1; pCfg; pCfg = pCfg->Next, i++) {
      pCfg->LineNbr = i;
   }
}

/* ------------------------DeleteConfigEntry -------------------------- */
/* Deletes a config entry.
*/
void DeleteConfigEntry(CONFIGENTRY *pCfg)
{
   /* Adjust earlier entries Next Pointer
   */
   pCfg->Prev->Next = pCfg->Next;

   /* Adjust Next entries Prev Pointer if it exists
   */
   if (pCfg != CfgLast) {
      pCfg->Next->Prev = pCfg->Prev;
   }

   /* If we deleted the last entry, reset CfgLast
   */
   if (pCfg->Next == NULL) {
      CfgLast = pCfg;
   }

   /* free strings and structure. Be careful not to call this routine for
   *  the section entry before deleting ALL the entries in the section.
   */
   if (strcmp(pCfg->Section, pCfg->Value ) == 0) {
      free(pCfg->Section);
   }
   free(pCfg->Key);
   free(pCfg->Value);
   free(pCfg);
}

/* ---------------------------DeleteConfigString --------------------- */
/* Deletes a ConfigEntry
*  Note: Section must be provided and found or nothing is done.
*        If Section is found then the action depends on Key.
*        If Key is NULL then the complete section is removed.
*        If Key is provided and is not found nothing is done.
*        If Key is found then the action depends on Value.
*        If Value is NULL then all Matching keys are deleted.
*        If Value is provided and it is not found then nothing is done.
*        If Value is provided and found it is deleted.
*
*  Returns: Number of lines/entries deleted.
*
*/
int DeleteConfigString(char *Section, char *Key, char *Value)
{
   CONFIGENTRY *pCfg = CfgList;
   CONFIGENTRY *pTmp, *pSection;
   int fFound = FALSE;
   int iDeleted = 0;

   if (!Section) return(iDeleted);

   /* Find start of this section
   */
   while (pCfg) {
      if (strcmp(Section,pCfg->Section) == 0) {
         fFound = TRUE;
         break;
      }
      pCfg = pCfg->Next;
   }
   /* Section Not Found
   */
   if (!fFound) return(iDeleted);

   /* No Key so delete all entries in section
   */
   if (!Key) {
      /* Save the section entry for later.
      */
      pSection = pCfg;
      pCfg = pCfg->Next;

      while(pCfg && (strcmp(pCfg->Section, Section)==0)) {
         pTmp = pCfg;
         pCfg = pCfg->Next;
         DeleteConfigEntry(pTmp);
         iDeleted++;
      }

      /* Now delete the section entry
      */
      DeleteConfigEntry(pSection);
      RenumberLines();
      return(++iDeleted);
   }

   /* If we get here we have a section and a key
   */
   while( pCfg && strcmp( Section, pCfg->Section ) == 0 ) {

      if ( (strcmp( Key, pCfg->Key ) == 0) &&
           ((strcmp( Value, pCfg->Value ) == 0 ) || !Value)) {

         pTmp = pCfg;
         pCfg = pCfg->Next;
         DeleteConfigEntry(pTmp);
         iDeleted++;
      } else {
         pCfg = pCfg->Next;
      }
   }
   RenumberLines();
   return(iDeleted);
}
