/* DIALOG RESOURCE FILE */

#include <stdlib.h>
#include <stdio.h>
#include "pal.h"
#include "pdersrc.h"

/* special handler only in PDE, DONT remove */
int  IhUEdit(int Msg, int n, void *p, DIALOG *pDlg, DLGITEM *pItm);

/*[PDE:pick]*/
  
IHCBINIT CBPICK1 = {
{"",  50, MEDIUM_FONT },
""
};

DLGITEM pickItems[] = {
/*X   Y   W   D   FLAGS     LABEL       INIT       TYPE           SIZE        PRIV */
{ 57, 44,274, 14,   0L, "&Name:",     &CBPICK1,   IhCombo , sizeof(IHCOMBO ), NULL },
{ 37, 84,340, 10,   0L, "Press F9 to cancel, F10 to accept.", NULL,       IhStatic, sizeof(IHSTATIC), NULL },
{100, 14,220, 10,   0L, "Please select one.", NULL,       IhStatic, sizeof(IHSTATIC), NULL }
};

DIALOG pick = {
410, 122,
0L,
0,
3,
pickItems,
DhStandard,
NULL,
NULL,
0
};

/*[PDE:DlgSave]*/
 
DLGITEM DlgSaveItems[] = {
/*X   Y   W   D   FLAGS     LABEL       INIT       TYPE           SIZE        PRIV */
{ 19, 20,450, 14,   0L, "Enter &filename:", NULL,       IhEdit  , sizeof(IHEDIT  ), NULL },
{ 86, 48,340, 10,   0L, "Press F9 to cancel, F10 to accept.", NULL,       IhStatic, sizeof(IHSTATIC), NULL }
};

DIALOG DlgSave = {
514, 80,
0L,
0,
2,
DlgSaveItems,
DhStandard,
NULL,
NULL,
0
};

/*[PDE:edidlg]*/

IHCBINIT CBEDI1 = {
{"",  20, MEDIUM_FONT },
"Small|Medium |Large"
};

IHEDINIT EDEDI3 = {
"",  250, MEDIUM_FONT
};

DLGITEM edidlgItems[] = {
/*X   Y   W   D   FLAGS     LABEL   INIT      TYPE         SIZE        PRIV */
{ 17,  8,152, 14,    0L, "&Font:",          &CBEDI1, IhCombo , sizeof(IHCOMBO ), NULL },
{227,  8,200, 14,    0L, "&Buffer lenght:", NULL,       IhEdit  , sizeof(IHEDIT  ), NULL },
{ 17, 38,410, 14,    0L, "&Default text:",  &EDEDI3, IhEdit  , sizeof(IHEDIT  ), NULL },
{ 56, 74,340, 10,   0L, "Press F9 to cancel, F10 to accept.", NULL,       IhStatic, sizeof(IHSTATIC), NULL }
};

DIALOG edidlg = {
450, 114,
0L,
0,
4,
edidlgItems,
DhStandard,
NULL,
NULL,
0
};

/*[PDE:stidlg]*/

IHCBINIT CBSTI1 = {
{"",  20, MEDIUM_FONT },
"Small|Medium|Large"
};

IHCBINIT CBSTI2 = {
{"",  20, MEDIUM_FONT },
"Black|White"
};

DLGITEM stidlgItems[] = {
/*X   Y   W   D   FLAGS     LABEL   INIT      TYPE         SIZE        PRIV */
{ 17, 18,152, 14,    0L, "&Font:",         &CBSTI1, IhCombo , sizeof(IHCOMBO ), NULL },
{210, 18,210, 14,    0L, "&BckGnd color:", &CBSTI2, IhCombo  , sizeof(IHCOMBO ), NULL },
{ 64, 55,340, 10,   0L, "Press F9 to cancel, F10 to accept.", NULL,       IhStatic, sizeof(IHSTATIC), NULL }
};

DIALOG stidlg = {
450, 94,
0L,
0,
3,
stidlgItems,
DhStandard,
NULL,
NULL,
0
};

/*[PDE:btidlg]*/

IHCBINIT CBBTI1 = {
{"",  20, MEDIUM_FONT },
"Small|Medium|Large"
};

DLGITEM btidlgItems[] = {
/*X   Y   W   D   FLAGS     LABEL   INIT      TYPE         SIZE        PRIV */
{137, 18,152, 14,    0L, "&Font:",  &CBBTI1, IhCombo , sizeof(IHCOMBO ), NULL },
{ 54, 58,340, 10,   0L, "Press F9 to cancel, F10 to accept.", NULL,       IhStatic, sizeof(IHSTATIC), NULL }
};

DIALOG btidlg = {
450, 94,
0L,
0,
2,
btidlgItems,
DhStandard,
NULL,
NULL,
0
};

/*[PDE:chidlg]*/

IHCBINIT CBCHI2 = {
{"",  20, MEDIUM_FONT },
"Small|Medium|Large"
};

DLGITEM chidlgItems[] = {
/*X   Y   W   D   FLAGS     LABEL   INIT      TYPE         SIZE        PRIV */
{ 57, 18, 15, 14,    0L, "&Checked", NULL,      IhCheck,  sizeof(IHCHECK),  NULL },
{210, 18,152, 14,    0L, "&Font:",  &CBCHI2, IhCombo , sizeof(IHCOMBO ), NULL },
{ 52, 54,340, 10,   0L, "Press F9 to cancel, F10 to accept.", NULL,       IhStatic, sizeof(IHSTATIC), NULL }
};

DIALOG chidlg = {
450, 94,
0L,
0,
3,
chidlgItems,
DhStandard,
NULL,
NULL,
0
};

/*[PDE:cmidlg]*/
 
IHCBINIT CBCMI1 = {
{"",  20, MEDIUM_FONT },
"Small|Medium |Large"
};

IHEDINIT EDCMI3 = {
"",  250, MEDIUM_FONT
};

IHEDINIT EDCMI4 = {
"",  250, MEDIUM_FONT
};

DLGITEM cmidlgItems[] = {
/*X   Y   W   D   FLAGS     LABEL   INIT      TYPE         SIZE        PRIV */
{ 17,  8,152, 14,    0L, "&Font:",          &CBCMI1, IhCombo , sizeof(IHCOMBO ), NULL },
{227,  8,200, 14,    0L, "&Buffer lenght:", NULL,       IhEdit  , sizeof(IHEDIT  ), NULL },
{ 17, 38,410, 14,    0L, "&Default text:",  &EDCMI3, IhEdit  , sizeof(IHEDIT  ), NULL },
{ 17, 58,410, 14,    0L, "&Combo List  :",  &EDCMI4, IhEdit  , sizeof(IHEDIT  ), NULL },
{ 62, 77,340, 10,    0L, "Use '|' to separate combo items.",   NULL, IhStatic, sizeof(IHSTATIC), NULL },
{ 62, 97,340, 10,    0L, "Press F9 to cancel, F10 to accept.", NULL, IhStatic, sizeof(IHSTATIC), NULL }
};

DIALOG cmidlg = {
450, 124,
0L,
0,
6,
cmidlgItems,
DhStandard,
NULL,
NULL,
0
};

/*[PDE:bmidlg]*/
 
DLGITEM bmidlgItems[] = {
/*X   Y   W   D   FLAGS     LABEL       INIT       TYPE           SIZE        PRIV */
{ 19, 20,450, 14,   0L, "Enter &bitmap pointer:", NULL,       IhEdit  , sizeof(IHEDIT  ), NULL },
{ 86, 48,340, 10,   0L, "Press F9 to cancel, F10 to accept.", NULL,       IhStatic, sizeof(IHSTATIC), NULL }
};

DIALOG bmidlg = {
514, 80,
0L,
0,
2,
bmidlgItems,
DhStandard,
NULL,
NULL,
0
};

/*[PDE:DlgSetup]*/

IHEDINIT EDDLGFLAGS = {
"", 100, MEDIUM_FONT
};

DLGITEM DlgSetupItems[] = {
/*X   Y   W   D   FLAGS     LABEL       INIT       TYPE           SIZE        PRIV */
{ 12, 10,470, 14,   0L, "&Dialog file:", NULL,       IhEdit  , sizeof(IHEDIT  ), NULL },
{ 12, 30,470, 14,   0L, "Dialog &Name:", NULL,       IhEdit  , sizeof(IHEDIT  ), NULL },
{ 12, 50,470, 14,   0L, "Dialog &Handler:", NULL,       IhEdit  , sizeof(IHEDIT  ), NULL },
{ 84, 70,290, 14,   0L, "&Flags:",    &EDDLGFLAGS,IhUEdit  , sizeof(IHEDIT  ), NULL },
{ 94, 88,330, 10,   0L, "Press F9 to abort, F10 to accept.", NULL,       IhStatic, sizeof(IHSTATIC), NULL }
};

DIALOG DlgSetup = {
510, 130,
0L,
0,
5,
DlgSetupItems,
DhStandard,
NULL,
NULL,
0
};

/*[PDE:Itmset]*/

IHEDINIT EDITMLABEL = {
"",  78, MEDIUM_FONT
};

IHCBINIT CBITMTYPE = {
{"",  20, MEDIUM_FONT },
"Edit|Static|Button|Checkbox|Radio|Group|Combo|Bitmap|ListBox|Other"
};

IHEDINIT EDITMFLAGS = {
"", 100, MEDIUM_FONT
};

DLGITEM ItmsetItems[] = {
/*X   Y   W   D   FLAGS     LABEL       INIT       TYPE           SIZE        PRIV */
{ 20, 16,460, 14,   0L, "Item &Name:", NULL,       IhUEdit  , sizeof(IHEDIT  ), NULL },
{ 20, 36,460, 14,   0L, "Item &Label:", &EDITMLABEL,IhEdit  , sizeof(IHEDIT  ), NULL },
{ 20, 56,210, 14,   0L, "Item &Type:", &CBITMTYPE, IhCombo , sizeof(IHCOMBO ), NULL },
{260, 56,220, 14,   0L, "&Flags:",    &EDITMFLAGS,IhUEdit  , sizeof(IHEDIT  ), NULL },
{ 90, 86,340, 10,   0L, "Press F9 to cancel, F10 to accept.", NULL,       IhStatic, sizeof(IHSTATIC), NULL }
};

DIALOG Itmset = {
514, 120,
0L,
0,
5,
ItmsetItems,
DhStandard,
NULL,
NULL,
0
};

/*[PDE:gridlg]*/

IHCBINIT CBGRI1 = {
{"",  20, MEDIUM_FONT },
"Small|Medium|Large"
};

IHCBINIT CBGRI2 = {
{"",  20, MEDIUM_FONT },
"Black|White"
};

DLGITEM gridlgItems[] = {
/*X   Y   W   D   FLAGS     LABEL   INIT      TYPE         SIZE        PRIV */
{ 17, 18,152, 14,    0L, "&Font:",         &CBGRI1, IhCombo , sizeof(IHCOMBO ), NULL },
{210, 18,210, 14,    0L, "&BckGnd color:", &CBGRI2, IhCombo  , sizeof(IHCOMBO ), NULL },
{ 64, 55,340, 10,   0L, "Press F9 to cancel, F10 to accept.", NULL,       IhStatic, sizeof(IHSTATIC), NULL }
};

DIALOG gridlg = {
450, 94,
0L,
0,
3,
gridlgItems,
DhStandard,
NULL,
NULL,
0
};

/*[PDE:raidlg]*/

IHCBINIT CBRAI2 = {
{"",  20, MEDIUM_FONT },
"Small|Medium|Large"
};

DLGITEM raidlgItems[] = {
/*X   Y   W   D   FLAGS     LABEL   INIT      TYPE         SIZE        PRIV */
{ 57, 18, 15, 14,    0L, "&Marked", NULL,      IhCheck,  sizeof(IHCHECK),  NULL },
{210, 18,152, 14,    0L, "&Font:",  &CBRAI2, IhCombo , sizeof(IHCOMBO ), NULL },
{ 52, 54,340, 10,   0L, "Press F9 to cancel, F10 to accept.", NULL,       IhStatic, sizeof(IHSTATIC), NULL }
};

DIALOG raidlg = {
450, 94,
0L,
0,
3,
raidlgItems,
DhStandard,
NULL,
NULL,
0
};

/*[PDE:otheritem]*/

DLGITEM otheritemItems[] = {
/*X   Y   W   D   FLAGS     LABEL       INIT       TYPE           SIZE        PRIV */
{  9, 18,330, 14,   0L, "Handler name:", NULL,       IhEdit  , sizeof(IHEDIT  ), NULL },
{  7, 54,340, 10,   0L, "Press F9 to cancel, F10 to accept.", NULL,       IhStatic, sizeof(IHSTATIC), NULL }
};

DIALOG otheritem = {
354, 90,
0L,
0,
2,
otheritemItems,
DhStandard,
NULL,
NULL,
0
};

/*[PDE:lbidlg]*/

IHCBINIT LBBTI1 = {
{"",  20, MEDIUM_FONT },
"Small|Medium|Large"
};

DLGITEM lbidlgItems[] = {
/*X   Y   W   D   FLAGS     LABEL   INIT      TYPE         SIZE        PRIV */
{137, 18,152, 14,    0L, "&Font:",  &LBBTI1, IhCombo , sizeof(IHCOMBO ), NULL },
{ 54, 58,340, 10,   0L, "Press F9 to cancel, F10 to accept.", NULL,       IhStatic, sizeof(IHSTATIC), NULL }
};

DIALOG lbidlg = {
450, 94,
0L,
0,
2,
btidlgItems,
DhStandard,
NULL,
NULL,
0
};

