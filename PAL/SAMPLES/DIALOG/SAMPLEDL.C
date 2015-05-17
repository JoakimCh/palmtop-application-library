/* DIALOG RESOURCE FILE */

#include <stdlib.h>

#include "pal.h"
#include "sampledlg.h"

/*[PDE:SampleDlg]*/

IHRAINIT RARABOTH = {
MEDIUM_FONT, 1
};


DLGITEM SampleDlgItems[] = {
/*X   Y   W   D   FLAGS     LABEL       INIT       TYPE           SIZE        PRIV */
{ 10, 10,152, 14,   0L, "&Name:",    NULL,       IhEdit  , sizeof(IHEDIT  ), NULL },
{ 20, 50, 15, 15,   0L, "&Turbo'ed", NULL,       IhCheck , sizeof(IHCHECK ), NULL },
{127, 45,139, 82,   0L, "Palmtop",   NULL,       IhListBox, sizeof(IHLISTBOX), NULL },
{ 11, 30,372,104,   0L, "Palmtop &Info", NULL,       IhGroup , sizeof(IHGROUP ), NULL },
{279, 63,  0,  0,   1L, "&Dos",      NULL,       IhRadio , sizeof(IHRADIO ), NULL },
{279, 80,  0,  0,   1L, "&Pim",      NULL,       IhRadio , sizeof(IHRADIO ), NULL },
{279, 96,  0,  0,   1L, "&Both",     &RARABOTH,  IhRadio , sizeof(IHRADIO ), NULL },
{275, 41,104, 86,   0L, "Usage",     NULL,       IhGroup , sizeof(IHGROUP ), NULL },
{ 19,100, 60, 25,   0L, "&Help",     NULL,       IhButton, sizeof(IHBUTTON), NULL },
{342,  5, 40, 25, IF_RET, "&OK",       NULL,       IhButton, sizeof(IHBUTTON), NULL }
};

DIALOG SampleDlg = {
394, 154,
0L,
0,
10,
SampleDlgItems,
DhStandard,
NULL,
NULL,
0
};

