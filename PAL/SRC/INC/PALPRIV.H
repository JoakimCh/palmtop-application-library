/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  PALPRIV.H
   Author:  Harry Konstas/Gilles Kohl
   Started: 10. Nov. 94
   Subject: Private include file shared among PAL modules
   -------------------------------------------------------------------- */

#ifndef _PALPRIV_H
#define _PALPRIV_H

/* --------------------------------------------------------------------
                         msdos includes
   -------------------------------------------------------------------- */
#include <dos.h>

/* --------------------------------------------------------------------
                         local includes
   -------------------------------------------------------------------- */
#include "palmem.h"

/* --------------------------------------------------------------------
                      constant definitions
   -------------------------------------------------------------------- */
#define SHADOWWIDTH     6
#define SHADOWDEPTH     4

#define MAXFONTENTRIES 20

#define APPMAXAPPS       38
#define APPDEFSIZE     4374 /* base size of application definitions */
#define APPMGRFILE     "C:\\_DAT\\APPMGR.DAT"

/* --------------------------------------------------------------------
                          macro definitions
   -------------------------------------------------------------------- */

#define PARAM(ptr) (*((WORD *)((BYTE *)(ptr)+1)))

/* --------------------------------------------------------------------
                           type definitions
   -------------------------------------------------------------------- */
typedef struct ConfigEntryTag {
   struct ConfigEntryTag *Prev;
   struct ConfigEntryTag *Next;
   char *Section;
   char *Key;
   char *Value;
   int LineNbr;
} CONFIGENTRY;


/* --------------------------------------------------------------------
                             globals
   -------------------------------------------------------------------- */
extern int Palmtop;  /* which palmtop are we running on */

extern CONFIGENTRY *CfgList;
extern CONFIGENTRY *CfgLast;
extern CONFIGENTRY *TmpEntry;
extern CONFIGENTRY *TmpSectionEntry;

extern char *TmpSection;
extern char *CurSection;
extern char PrologSection[64];
extern FONTDESC *PalFontTab[];

/* Restore globals */
extern int PalStateSaved;
extern SAVEDSTATE PalState;

/* EMS global error */
extern int EmsErr;
extern void far *pEmsBase;

/* Out of memory handler */
extern int (*pPalMemFail)(size_t size);

/* --------------------------------------------------------------------
                           prototypes
   -------------------------------------------------------------------- */

void CallInt(int IntNo, WORD ax, WORD bx, WORD cx, WORD dx, union REGS *pr);
int TextOut0(int x, int y, int Style, int FntId, char *s);
int TextOut1(int x, int y, int Style, int FntId, char *s);
int TextExt0(int FntId, char *s);
int TextExt1(int FntId, char *s);
int FontHeight0(int FntId);
int FontHeight1(int FntId);
char *StrNrml(char *Dst, char *Src);
int LabelExt(int FontId, char *Lbl);
void LabelOut(int x, int y, int Style, int FontId, char *Lbl);
int StrSplit(char **pPtr, char *Txt, int Max);

#endif

