/* DIALOG RESOURCE FILE */

#include <stdlib.h>

#include "pal.h"
#include "pfers.h"

/*[PDE:SFont]*/

IHEDINIT EDFNAME = {
"",  80, MEDIUM_FONT
};

IHRAINIT RAHFNTYPE = {
MEDIUM_FONT, 1
};

DLGITEM SFontItems[] = {
/*X   Y   W   D   FLAGS     LABEL       INIT       TYPE           SIZE        PRIV */
{ 36, 11,390, 14,   0L, "&Enter filename:", &EDFNAME,   IhEdit  , sizeof(IHEDIT  ), NULL },
{ 56, 33,326, 14,   0L, "HFN Font &I.D.   (style):", NULL,       IhEdit  , sizeof(IHEDIT  ), NULL },
{ 42, 78,  0,  0,    1, "&HFN format", &RAHFNTYPE, IhRadio , sizeof(IHRADIO ), NULL },
{172, 78,  0,  0,    1, "&VFN format", NULL,       IhRadio , sizeof(IHRADIO ), NULL },
{299, 78,  0,  0,    1, "&FCL format", NULL,       IhRadio , sizeof(IHRADIO ), NULL },
{ 65,107,340, 10,   0L, "Press F10 to accept, F9 to cancel.", NULL,       IhStatic, sizeof(IHSTATIC), NULL },
{ 32, 57,386, 36,   0L, "File format", NULL,       IhGroup , sizeof(IHGROUP ), NULL }
};

DIALOG SFont = {
466, 135,
0L,
0,
7,
SFontItems,
DhStandard,
NULL,
NULL,
0
};

/*[PDE:OpenFont]*/

IHEDINIT EDOPNAME = {
"",  80, MEDIUM_FONT
};

DLGITEM OpenFontItems[] = {
/*X   Y   W   D   FLAGS     LABEL       INIT       TYPE           SIZE        PRIV */
{ 21, 28,412, 14,   0L, "Enter &filename:", &EDOPNAME,  IhEdit  , sizeof(IHEDIT  ), NULL },
{ 61, 68,340, 10,   0L, "Press F9 to cancel, F10 to accept.", NULL,       IhStatic, sizeof(IHSTATIC), NULL }
};

DIALOG OpenFont = {
458, 111,
0L,
0,
2,
OpenFontItems,
DhStandard,
NULL,
NULL,
0
};

/*[PDE:FontSize]*/

DLGITEM FontSizeItems[] = {
/*X   Y   W   D   FLAGS     LABEL       INIT       TYPE           SIZE        PRIV */
{ 16, 39,146, 14,   0L, "Font &width:", NULL,       IhEdit  , sizeof(IHEDIT  ), NULL },
{188, 39,156, 14,   0L, "Font &height:", NULL,       IhEdit  , sizeof(IHEDIT  ), NULL },
{ 16, 62,320, 10,   0L, "( 0 value keeps original W or H )", NULL,       IhStatic, sizeof(IHSTATIC), NULL },
{ 84, 15,190, 10,   0L, "(Enter values 4-16)", NULL,       IhStatic, sizeof(IHSTATIC), NULL },
{ 15, 85,340, 10,   0L, "Press F9 to cancel, F10 to accept.", NULL,       IhStatic, sizeof(IHSTATIC), NULL }
};

DIALOG FontSize = {
370, 119,
0L,
0,
5,
FontSizeItems,
DhStandard,
NULL,
NULL,
0
};

