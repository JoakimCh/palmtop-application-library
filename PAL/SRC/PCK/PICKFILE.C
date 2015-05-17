/* --------------------------------------------------------------------
   Project: PAL Picklist function test
   Module:  Picklist.C
   Author:  Bob Chernow
   Started: August. 17, 1995
   Subject: A picklist of files
------------------------------------------------------------------------ */

/* --------------------------------------------------------------------
                       standard includes
   -------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* --------------------------------------------------------------------
                         msdos includes
   -------------------------------------------------------------------- */

#if defined _MSC_VER || defined _QC
#include <dos.h>
#include <direct.h>
#define findfirst(a,b,c) _dos_findfirst(a,c,b)
#define findnext(a) _dos_findnext(a)
#define ffblk find_t
#define ff_name name
#else
#include <dir.h>
#endif

/* --------------------------------------------------------------------
                         local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"

/* --------------------------------------------------------------------
                         type definitions
   -------------------------------------------------------------------- */
#define MAXFILES 512

/* --------------------------------------------------------------------
                           Functions
   -------------------------------------------------------------------- */


/*-----------------------------------------------------------------------------
     Function: FindFiles
       Syntax: pFF = *FindFiles(char *FileSpec)
   Parameters: filespec = (char *) that points to a valid DOS filespec with
                          wildcards allowed.
      Returns: pFF = (*FOUNDFILES) : ptr to structures with file info
  -----------------------------------------------------------------------------
     History
     Date     Who       What                                            Tag
     -------- --------- ----------------------------------------------- -------
     09/02/95 rlc       Creation                                        None
 ----------------------------------------------------------------------------*/
FOUNDFILES *FindFiles(char *FileSpec)
{
    int i;
    struct ffblk ffb;
    char Path[512];
    char *s;
    FOUNDFILES *pFF;

    char Dir[64];

    /* Allocate a FOUNDFILES structure
    */
    pFF = (FOUNDFILES *)malloc(sizeof(FOUNDFILES));

    /* Allocate enough room for MAXFILES files
    */
    pFF->FileNames = (char **)malloc(MAXFILES * sizeof(char **));

    pFF->Count = 0;

    strcpy(Dir, FileSpec);

    /* scan all files matching filespec in this directory
    */
    if(findfirst(Dir, &ffb, 0) == 0) {
        do {
            char Name[256];

            if(pFF->Count >= MAXFILES) {
                continue; /* ignore if too many filess */
            }

            if(*ffb.ff_name == '.') {
                continue; /* skip . and .. */
            }

            strcpy(Name, Dir);
            strlwr(ffb.ff_name+1);
            strcpy(Name, ffb.ff_name);

            pFF->FileNames[pFF->Count++] = strdup(Name);
        } while(findnext(&ffb) == 0);
    }
    return(pFF);
}


/*-----------------------------------------------------------------------------
     Function: FreeFoundFiles
       Syntax: FreeFoundFiles( pFF );
   Parameters: pFF: (FOUNDFILES *) Pointer to a FOUNDFILES structure
      Returns: nothing
        Notes: Frees the Storage associated with the FILESFOUND
  -----------------------------------------------------------------------------
     History
     Date     Who       What                                            Tag
     -------- --------- ----------------------------------------------- -------
     mm/dd/yy rlc       Creation                                        None
 ----------------------------------------------------------------------------*/
void FreeFoundFiles(FOUNDFILES *pFF) {
    int i;

    for (i = 0;i < pFF->Count; i++) {
		free(pFF->FileNames[i]);
    }
    free(pFF);
}

/*-----------------------------------------------------------------------------
     Function: FilePickList
       Syntax: ret = FilePickList(Dir, FixedRows, PosX, PosY)
   Parameters: Dir: (char *) A valid filespec
               FixedRows: (int) Height of Picklist
                         This controls the height of the picklist.
                         If this Value is 0 the list will have as many rows
                         as there are entries up to a max of 8.
                         If this is non-zero it will be the Number of Rows
                         for the picklist up to the number of items.
               PosX: (int) X position for upper left corner.
               PosY: (int) Y position for upper left corner.
      Returns: (PICKRESULTS *) Array of pointers to pointers to strings.
        Notes: Display a list of files and let the user select multiple files.
  -----------------------------------------------------------------------------
     History
     Date     Who       What                                            Tag
     -------- --------- ----------------------------------------------- -------
     09/02/95 rlc       Creation                                        None
     09/03/95 rlc       Added Coordinates                               None
     09/04/95 rlc       Modified to return PICKRESULTS                  None
 ----------------------------------------------------------------------------*/
PICKRESULTS *FilePickList(char *Dir, int FixedRows, int PosX, int PosY) {

    PICKRESULTS *pPR;
    int         Count;
    FOUNDFILES  *pFF;

    pFF = FindFiles(Dir);

    pPR = PickList(pFF->FileNames, pFF->Count, FixedRows, PosX, PosY);

    FreeFoundFiles(pFF);

    return(pPR);
}
