/* --------------------------------------------------------------------
   Project: PAL Picklist function test
   Module:  Picklist.C
   Author:  Bob Chernow
   Started: August. 17, 1995
   Subject: An easier version of picklist
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
                           Functions
   -------------------------------------------------------------------- */


/*-----------------------------------------------------------------------------
     Function: QuickPickList
       Syntax: ret = QuickPickList(Strings, FixedRows, PosX, PosY)
   Parameters: Strings: (char *) A | delimeted string where each item will
                        be an entry in the PickList.
               FixedRows: (int) Height of Picklist
                         This controls the height of the picklist.
                         If this Value is 0 the list will have as many rows
                         as there are entries up to a max of 8.
                         If this is non-zero it will be the Number of Rows
                         for the picklist up to the number of items.
               PosX: (int) X position for upper left corner.
               PosY: (int) Y position for upper left corner.
      Returns: (char **) Array of pointers to pointers to strings.
               It is the callers responsibility to free these.
        Notes: Display a list of files and let the user select multiple files.
  -----------------------------------------------------------------------------
     History
     Date     Who       What                                            Tag
     -------- --------- ----------------------------------------------- -------
     09/03/95 rlc       Creation                                        None
     09/04/95 rlc       Modified to return PICKRESULTS                  None
 ----------------------------------------------------------------------------*/
PICKRESULTS *QuickPickList(char *Strings,
                     int    FixedRows,
                     int    PosX,
                     int    PosY) {


    char        **ArrayIn;
    char        *s;
    int         n, NumItems;
    char        *List;
    PICKRESULTS *pPR;

    List = strdup(Strings);

    /* loop over list, terminating strings and counting them
    */
    for(n = 0, s = List; *s; ++s) {
        if(*s == '|') {
            *s = '\0';
            n++;
        }
    }
    ++n; /* add one more - last string doesn't have a | character */
    NumItems = n;

    /* We now know how many pointers we need, so get the storage
    */
    ArrayIn =(char **)malloc(n * sizeof(char *));


    /* loop over list, building the input array
    */
    ArrayIn[0] = List;
    for(n = 1, s = List; n < NumItems; ++s) {
        if(*s == '\0') {
            ArrayIn[n++] = ++s;
            s--;
        }
    }

    /* call the main Picklist function
    */
    pPR = PickList(ArrayIn,NumItems, FixedRows, PosX, PosY);

    /* Clean up
    */
    free(List);

    free(ArrayIn);

    return(pPR);
}
