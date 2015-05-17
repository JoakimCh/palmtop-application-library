/* --------------------------------------------------------------------
   Project: PAL Picklist function test
   Module:  Picklist.C
   Author:  Bob Chernow
   Started: August. 17, 1995
   Subject: Find number of nodes
------------------------------------------------------------------------ */

/* --------------------------------------------------------------------
                         local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"

/* --------------------------------------------------------------------
                           Functions
   -------------------------------------------------------------------- */

/*-----------------------------------------------------------------------------
     Function: PickCount
       Syntax: cnt = PickCount(pPR);
   Parameters: pPR = (PICKRESULTS *): a PICKRESULTS linked list
      Returns: number of entries in the list
        Notes:
  -----------------------------------------------------------------------------
     History
     Date     Who       What                                            Tag
     -------- --------- ----------------------------------------------- -------
     09/19/95 rlc       Creation                                        None
 ----------------------------------------------------------------------------*/
int PickCount(PICKRESULTS * pPR){
    int cnt = 0;
    PICKRESULTS   *pPRTemp = pPR;

    while ( pPRTemp ) {
        cnt++;
        pPRTemp = pPRTemp ->Next;
    }
    return( cnt );
}

