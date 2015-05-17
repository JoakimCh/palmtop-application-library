
/* --------------------------------------------------------------------
   Project: PAL Picklist function test
   Module:  Picklist.C
   Author:  Bob Chernow
   Started: August. 17, 1995
   Subject: Main picklist functions.
------------------------------------------------------------------------ */

/* --------------------------------------------------------------------
                       standard includes
   -------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------
                         local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"

/* --------------------------------------------------------------------
                        local prototypes
   -------------------------------------------------------------------- */
int MenuCallback(MENU *pMnu);

/* --------------------------------------------------------------------
                           Functions
   -------------------------------------------------------------------- */

/*-----------------------------------------------------------------------------
     Function: FindLastPick
       Syntax: cnt = FindLastPick(pPR);
   Parameters: pPR = (PICKRESULTS *): a PICKRESULTS linked list
      Returns: ptr to last entry in the list
        Notes:
  -----------------------------------------------------------------------------
     History
     Date     Who       What                                            Tag
     -------- --------- ----------------------------------------------- -------
     09/19/95 rlc       Creation                                        None
 ----------------------------------------------------------------------------*/
PICKRESULTS * FindLastPick(PICKRESULTS * pPR){
    PICKRESULTS   *pPRTemp = pPR;
    PICKRESULTS   *pPRCurr;

    while ( pPRTemp ) {
        pPRCurr = pPRTemp;
        pPRTemp = pPRTemp ->Next;
    }
    return( pPRCurr );
}

/*-----------------------------------------------------------------------------
     Function: MenuCallback
       Syntax: pItems[i].ActionPtr  = MENUVAL(MenuCallback);
   Parameters: None from user.
      Returns: MSC_STAY
        Notes: Provides picklist function.
  -----------------------------------------------------------------------------
     History
     Date     Who       What                                            Tag
     -------- --------- ----------------------------------------------- -------
     09/02/95 rlc       Creation                                        None
 ----------------------------------------------------------------------------*/
int MenuCallback(MENU *pMenu) {
    int iSel;
    char buf[80];
    iSel = pMenu->CurItem;

    /* if selected then deselect
    */
    if (pMenu->Items[iSel].Text[0] == '') {
        pMenu->Items[iSel].Text[0] = ' ';
    }
    /* else select
    */
    else {
        pMenu->Items[iSel].Text[0] = '';
    }
    return MSC_STAY;
}

/*-----------------------------------------------------------------------------
     Function: FreePickResults
       Syntax: FreePickResults( pPR );
   Parameters: pPR: (PICKRESULTS *) Pointer to a PICKRESULTS structure
      Returns: nothing
        Notes: Frees the Storage associated with the PICKRESULTS
  -----------------------------------------------------------------------------
     History
     Date     Who       What                                            Tag
     -------- --------- ----------------------------------------------- -------
     mm/dd/yy rlc       Creation                                        None
 ----------------------------------------------------------------------------*/
void FreePickResults(PICKRESULTS *pPR) {

    PICKRESULTS  *pPRTemp = pPR;

    while (pPRTemp) {
        /* Save next entry pointer
        */
        pPR = pPRTemp->Next;

        /* Free storage for string
        */
        free(pPRTemp->String);

        /* now free the entry itself
        */
        free(pPRTemp);

        /* Point to the next entry
        */
        pPRTemp = pPR;
    }
}


/*-----------------------------------------------------------------------------
     Function: PickList
       Syntax: ret = PickList(Strings, Count, FixedRows, PosX, PosY)
   Parameters: Strings: (char **) An array of poiners to strings
               Count: (int ) Number of Strings
               FixedRows: (int) Height of Picklist
                         This controls the height of the picklist.
                         If this Value is 0 the list will have as many rows
                         as there are entries up to a max of 8.
                         If this is non-zero it will be the Number of Rows
                         for the picklist up to the number of items.
               PosX: (int) X position for upper left corner.
               PosY: (int) Y position for upper left corner.
      Returns: (PICKRESULTS *) pointer to a structure containing the count
                of selected items, an array of item indeces and an array of
                pointers to pointers to strings.
  -----------------------------------------------------------------------------
     History
     Date     Who       What                                            Tag
     -------- --------- ----------------------------------------------- -------
     09/03/95 rlc       Creation                                        None
     09/04/95 rlc       Modified to use PICKRESULTS
 ----------------------------------------------------------------------------*/
PICKRESULTS *PickList(char **Strings,
                      int    Count,
                      int    FixedRows,
                      int    PosX,
                      int    PosY) {

    int         i,
                cnt;
    MENU        DynaMenu;
    MENUITEM    *pItems;
    char        buf[80];
    PICKRESULTS *pPR     = NULL,  *pPRTemp = NULL,  *pPRLast = NULL;


    /* allocate storage for Count items
    */
    pItems = malloc(Count * sizeof(MENUITEM));

    /* Adjust for FixedRows
    */
    if (FixedRows == 0) {
        FixedRows = 8;
    }else{
        FixedRows = min(FixedRows,Count);
    }

    /* now initialize the items */
    for(i = 0; i < Count; i++) {
       sprintf(buf,"  %s", Strings[i]);
       pItems[i].Text       = strdup(buf);
       pItems[i].Flags      = MA_FUN;
       pItems[i].ActionPtr  = MENUVAL(MenuCallback);
    }

    /* If we have some menu items run the menu
    */
    if (Count > 0) {

        /* Build the menu
        */
        DynaMenu.pMnuWin  = NULL;
        DynaMenu.Flags    = 0;
        DynaMenu.CurItem  = 0;
        DynaMenu.TopItem  = 0;
        DynaMenu.ItmDisp  = FixedRows;
        DynaMenu.ItmCount = Count;
        DynaMenu.Items    = pItems;

        /* Run the menu
        */
        HandleMenu(&DynaMenu, PosX, PosY);

        /* Build an array of strings of the selected files
        */
        for(i = 0; i < Count; i++) {
            if (pItems[i].Text[0] == '') {

                /* allocate a new entry
                */
                pPRTemp = (PICKRESULTS  *)malloc(sizeof(PICKRESULTS));
                pPRTemp->Next = NULL;

                /* See if this is the first entry. If so, start the linked
                *  list with this entry.
                */
                if (!pPR) {
                    pPR = pPRTemp;
                /* otherwise, use the end list poiner to link this new entry
                *  and then set this new entry as the last entry.
                */
                } else {
                    pPRLast->Next = pPRTemp;
                }

                /* fill in this entries data
                */
                pPRLast = pPRTemp;
                pPRLast->String = strdup((pItems[i].Text)+2);
                pPRLast->Index = i;
            }
        }

        /* Now free the item strings ...
        */
        for(i = 0; i < Count; i++) free(pItems[i].Text);
        free(pItems); /* and the storage for the items */

        /* return the array of strings
        */
        return(pPR);

    } else {
        Count = 0;
        return(NULL);
    }
}

