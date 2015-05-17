/* --------------------------------------------------------------------
   Project: PAL FONT EDITOR Vr. 1.02
   Module:  PFE.H
   Author:  Harry Konstas
   Started: 10 May '95
   Subject: PFE Header file
   -------------------------------------------------------------------- */

#ifndef _PFE_H
#define _PFE_H


/* --------------------------------------------------------------------
                       standard includes
   -------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* --------------------------------------------------------------------
                         constant definitions
   -------------------------------------------------------------------- */

#define IMGLENGTH(Width, Depth) ((((Width)+7)/8)*(Depth))

#define HOFS 20
#define VOFS 20
#define AHOFS 210
#define AVOFS 18
#define MARK 1
#define UNMARK 0
#define SOLID 0xffff
#define DASHED 0x5555
#define SPCBAR 0x3920
#define KEYENTER 0x1c0d
#define ESCKEY 0x011b

/* --------------------------------------------------------------------
                           type definitions
   -------------------------------------------------------------------- */

typedef enum {
   CMD_ABOUT = 1, CMD_QUIT
} MENUCOMMANDS;


typedef struct {
    int  Flag;         /* 'defined' flag */
    int  Bits;
    int  Width;
    int  Depth;
    BYTE Bitmap[32];
} CHARIMG;             /* 16x16 character image */

/* VR font header */
typedef struct {
   int maxwidth;
   int vbytes;
   int minchar;
   int maxchar;
   int isfixed;
} VFNHDR;

typedef struct FONTDESCTag {
   BYTE *pBits;
   VFNHDR h;
} VFNDESC;


/* --------------------------------------------------------------------
                                prototypes
   -------------------------------------------------------------------- */

void Initialize(void);
void Interact(char *filename);
void GraphByte(int col, int row, int value);
void BitCursor(int col, int row, int state);
void OnGrid(int ascii);
void DisplayFont(void);
void AscCursor(int ascii);
void EditChar(int ascii);
int  GrabFont(char *filename);
void Bit2Matrix(int ascii);
void Matrix2Bit(int ascii);
void SaveFont(char *filename);
void RotateFont(void);
void EditString(int ascii);
void Help(void);
void About(void);
void AddExt(char *Dest, char *Source, char *Ext);
void OpenFNT(void);
void WriteHFN(FILE *dst);
void WriteVFN(FILE *dst);
void WFCLchar(FILE *dst, int ascii, int type);
void WriteFCL(char *filename, FILE *dst, int type);
void ConvVFN(void);
void ClearFont(void);
void DelChar(int ascii);
void FromLatin(void);
int  LoadVfn(char *Name);
void DisplayFname(char *filename);
int  LoadFCL(char *filename);
void LoadByExt(char *filename);
void FontSetup(int clear);

#endif

