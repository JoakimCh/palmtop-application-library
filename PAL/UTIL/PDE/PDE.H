/* --------------------------------------------------------------------
   Project: PAL Dialog Editor  Vr. 1.2
   Module:  PDE.H
   Author:  Harry Konstas / Gilles Kohl
   Started: 29. Jan '95
   Subject: PDE Header file
   -------------------------------------------------------------------- */

#ifndef _PDE_H
#define _PDE_H


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
                         macro definitions
   -------------------------------------------------------------------- */

#define ABOUT 1
#define NORMAL 0
#define S_MODE 0
#define MS_MODE 1
#define SOLID 0xffff
#define DASHED 0x5555
#define HOFFSET_ADJ 1
#define VOFFSET_ADJ 10

#define SMUP 0x4838
#define SMDOWN 0x5032
#define SMLEFT 0x4b34
#define SMRIGHT 0x4d36
#define KEYENTER 0x1c0d
#define KEYESC 0x011b
#define TAB_KEY 0x0f09
#define STAB_KEY 0x0f00

#define NUM_OF_TYPES 11
#define NUM_OF_ELMNT 60


/* --------------------------------------------------------------------
                           type definitions
   -------------------------------------------------------------------- */

enum {X,TEXT,LABEL,BUTTON,CHECK,OPTION,GROUP,COMBO,BITMAP,LISTBOX,OTHER,XX};

typedef enum {
   CMD_ABOUT = 1, CMD_DEFDIAG, CMD_SAVE
} MENUCOMMANDS;


/* The item structure */
typedef struct {
   int  type;                    /* item type               */
   int  x1;                      /* X1: coordinates of item */
   int  y1;                      /* Y1: coordinates of item */
   int  x2;                      /* X2: coordinates of item */
   int  y2;                      /* Y2: coordinates of item */
   char flags[100];              /* flags of the item       */
   char name[40];                /* item name               */
   char label[100];              /* item label              */
   char TypeName[40];            /* user defined type name  */
   int  InitFlag;                /* Init values flag        */
   int  ChkFlag;                 /* Checkbox / radio flag   */
   int  font;                    /* item font ID            */
   int  color;                   /* item color              */
   int  lenght;                  /* real buffer lenght      */
   char dflt[100];               /* default data in buffer  */
   char List[255];               /* combo list data         */
} FIELDEF;


/* The dialog structure */
typedef struct {
   char flags[100];              /* flags of the dialog     */
   char name[40];                /* name of the dialog      */
   char fname[40];               /* filename for the dialog */
   int  x1;                      /* X1: coordinates         */
   int  y1;                      /* Y1: coordinates         */
   int  x2;                      /* X2: coordinates         */
   int  y2;                      /* Y2: coordinates         */
   int  focus;                   /* focus on startup        */
   char handler[40];             /* dialog handler name     */
   int  INum;                    /* number of items         */
   int  DiagFlag;                /* dialog defined flag     */
   int  Modified;                /* Dialog modified flag    */
   FIELDEF *item[NUM_OF_ELMNT];  /* Max. number of items    */
} DIAGDEF;


/* --------------------------------------------------------------------
                                prototypes
   -------------------------------------------------------------------- */

DIAGDEF *Initialize(void);
DIAGDEF *DefineDiag(DIAGDEF *pDialog);
void DisplayDlg(DIAGDEF *pDialog);

void ClrScrn(int type);
void About(void);
void Help(void);
void AddExt(char *Dest, char *Source, char *Ext);
void Interact(DIAGDEF *pDialog);

void DlgWinSize(DIAGDEF *pDialog);
void SortDlg(DIAGDEF *pDialog);
void ItemHandler(DIAGDEF *pDialog);
void NewItem(DIAGDEF *pDialog);
void EditItems(DIAGDEF *pDialog);
void GetItemSet(DIAGDEF *pDialog, int ItmNum);
void EditISetup(DIAGDEF *pDialog, int ItmNum);
void EditInit(DIAGDEF *pDialog, int ItmNum);
int  IhUEdit(int Msg, int n, void *p, DIALOG *pDlg, DLGITEM *pItm);

FILE *FindDLG(FILE *pDEF, char *dname);
int  LoadDlg(DIAGDEF *pDialog, char *file, char *name);
int  SaveHdr(char *cfile, char *hfile);
void Save(DIAGDEF *pDialog);
void SaveC(DIAGDEF *pDialog, char *cfile, char *hfile);
void SaveH(DIAGDEF *pDialog, char *hfile);
void SaveTest(char *cfile, char *hfile);
void SaveInit(DIAGDEF *pDialog, FILE *pDEF);
void SaveItem(DIAGDEF *pDialog, FILE *pDEF);

int  GetDlgHdr(char *dname, char *hfile, DIAGDEF *pLoad);
int  GetDlgSrc(char *dname, char *cfile, DIAGDEF *pLoad);
int  DeleteDlg(char *sfile, char *dname);
void PickDlg(char *hfile, char *dname);
int  PickItm(DIAGDEF *pDialog);
void ChangePos(DIAGDEF *pDialog, int src, int dst);
void Bubble(DIAGDEF *pDialog);

int  CreateRectan(DIAGDEF *pDialog, int mode, int *x1, int *y1, int *x2, int *y2);
void DispInfo(DIAGDEF *pDialog, int type, int x1, int y1, int x2, int y2);
void DisField(int type, int style, int x1, int y1,
              int x2, int y2, char *label);
void MoveSize(DIAGDEF *pDialog, int key, int mode, int type, int step, int *x1,
              int *y1, int *x2, int *y2);
int  PlaceItem(DIAGDEF *pDialog, int ItemNo, int step, int type,
               int x1, int y1, int x2, int y2, char *label);

#endif

