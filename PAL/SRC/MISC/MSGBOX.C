/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  MSGBOX.C
   Author:  Gilles Kohl
   Started: 10. Apr. 94
   Subject: Display a message box dialog
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                            standard includes
   -------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


/* --------------------------------------------------------------------
                           local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"

/* --------------------------------------------------------------------
                            constants
   -------------------------------------------------------------------- */
#define TTLDIST 4
#define INPDIST 6
#define LINDIST 4
#define BTNDIST 12
#define XOFF    4
#define YOFF    4
#define BTNX    2
#define BTNY    4
#define BTNOFF  4

/* --------------------------------------------------------------------
                            globals
   -------------------------------------------------------------------- */
static DLGITEM MsgItm[] = {
/*X  Y  W  D   FLAGS LABEL  INIT  TYPE      SIZE              PRIV */
{ 0, 0, 0, 0,   0L,  NULL,  NULL, IhEdit,   sizeof(IHEDIT),   NULL },
{ 0, 0, 0, 0,   0L,  NULL,  NULL, IhButton, sizeof(IHBUTTON), NULL },
{ 0, 0, 0, 0,   0L,  NULL,  NULL, IhButton, sizeof(IHBUTTON), NULL },
{ 0, 0, 0, 0,   0L,  NULL,  NULL, IhButton, sizeof(IHBUTTON), NULL },
{ 0, 0, 0, 0,   0L,  NULL,  NULL, IhButton, sizeof(IHBUTTON), NULL },
{ 0, 0, 0, 0,   0L,  NULL,  NULL, IhButton, sizeof(IHBUTTON), NULL },
};

static DIALOG MsgDlg = {
   0, 0, 0L, 0, 0, &MsgItm[1], DhStandard, NULL, NULL, 0
};

static WORD FKeyCodes[] = { F1_KEY, F2_KEY, F3_KEY, F4_KEY, F5_KEY,
                           F6_KEY, F7_KEY, F8_KEY, F9_KEY, F10_KEY };

/* --------------------------------------------------------------------
                             Functions
   -------------------------------------------------------------------- */
int MsgBox(char *Title, char *Fmt, char *Inp, char *Btns, ...)
{
   va_list ArgPtr;        /* used for variable paramater list   */
   char *LinPtr[10];      /* array of pointers to line starts   */
   char *FnLbl[10];       /* array of pointers to f-key labels  */
   char FnTab[10];        /* button assigned to f-key           */
   char *BtnPtr[5];       /* array of pointers to button labels */
   char *Contents = NULL; /* pointer to contents line */
   char *Prompt   = NULL; /* pointer to prompt line */
   int FnActive  = FALSE; /* Flag: f-keys present */
   IMGHDR *FnBgnd = NULL; /* old f-key background (if any) */
   int LinCnt;            /* number of text lines */
   int BtnCnt;            /* number of buttons */
   int Focus  =-1;        /* button that gets the focus by default */
   int BtnSum = 0;        /* accumulated button width */
   int MaxLin = 0;        /* width of largest line */
   int MaxBtn = 0;        /* width of largest button */
   int Key;               /* Key hit in dialog */
   int i, x, y, w, d;     /* auxiliary variables */
   int fh = FontHeight(MEDIUM_FONT);
   char *Msg = NULL;

   va_start(ArgPtr, Btns);

   /* duplicate/allocate strings since StrSplit modifies them */
   if(Fmt)  Msg  = malloc(1024);
   if(Btns) Btns = strdup(Btns);

   if(Msg) vsprintf(Msg, Fmt, ArgPtr);

   /* Insert terminators, compute number of lines and buttons */
   LinCnt = StrSplit(LinPtr, Msg,  ELMS(LinPtr));
   BtnCnt = StrSplit(BtnPtr, Btns, ELMS(BtnPtr));

   /* if input line requested, find contents and prompt */
   if(Inp) {
      if(LinCnt) Contents = LinPtr[--LinCnt];
      if(LinCnt) Prompt   = LinPtr[--LinCnt];
   }

   /* find largest line */
   for(i = 0; i < LinCnt; i++) {
      int n = TextExt(MEDIUM_FONT, LinPtr[i]);
      if(n > MaxLin) MaxLin = n;
   }

   /* if we have an input field, adjust largest line */
   if(Inp) MaxLin = max(MaxLin,
                       (Contents ? TextExt(MEDIUM_FONT, Contents) : 0) +
                       (Prompt   ? TextExt(MEDIUM_FONT, Prompt)   : 0) +
                        48);

   /* init fkey labels and table */
   for(i = 0; i < 10; i++) {
      FnLbl[i] = NULL;
      FnTab[i] = -1;
   }
   FnLbl[8] = "Cancel";
   FnLbl[9] = "OK";

   if(BtnCnt) Focus = 0;
   /* find largest button, compute total button width,
      set fkey labels if any */
   for(i = 0; i < BtnCnt; i++) {
      int n, k;
      char *s, *p;

      if(*BtnPtr[i] == '!') {
         ++BtnPtr[i];
         Focus = i;
      }
      if(!!(s = strchr(BtnPtr[i], '[')) && s[1] == 'F') {
         FnActive = TRUE;
         *s++ = '\0';
         k = atoi(s+1)-1;
         if(k >= 0 && k <= 9) {
            FnTab[k] = i;
            p = strchr(s, ':');
            s = p ? p+1 : BtnPtr[i];
            if(p = strchr(p, ']')) *p = '\0';
            s += strspn(s, " ");
            FnLbl[k] = StrNrml(strdup(s), s);
         }
      }
      n = TextExt(MEDIUM_FONT, BtnPtr[i]);
      BtnSum += n+2*BTNX+BTNOFF;
      if(n > MaxBtn) MaxBtn = n;
   }

   /* Compute depth of dialog window */
   d =   LinCnt * (fh+LINDIST);
   if(Title)  d += fh+TTLDIST;
   if(Inp)    d += fh+INPDIST;
   if(BtnCnt) d += fh+BTNDIST;
   d += 2*YOFF;

   /* compute width of dialog window */
   w = max(BtnSum, Title ? TextExt(MEDIUM_FONT, Title) : 0);
   w = max(w, MaxLin+4) + 2*XOFF;

   /* store dialog data*/
   MsgDlg.Width = w;
   MsgDlg.Depth = d;
   MsgDlg.InitFocus = Inp ? 0 : Focus;
   MsgDlg.Items  = &MsgItm[!Inp];
   MsgDlg.ItmCnt = BtnCnt + !!Inp;

   /* compute vertical position of button/input line */
   y = YOFF+LinCnt*(fh+LINDIST);

   /* initialize Input field, if any */
   if(Inp) {
      MsgItm[0].PosX = XOFF;
      MsgItm[0].PosY = y;
      y += fh+INPDIST;
      MsgItm[0].Width = w-2*XOFF-4;
      MsgItm[0].Depth = fh + INPDIST;
      MsgItm[0].Text  = Prompt;
   }

   x  = (w - BtnSum)/2;  /* center group of buttons inside dialog */

   /* now store button data */
   for(i = 0; i < BtnCnt; i++) {
      if(i == Focus) MsgItm[i+1].Flags |= IF_RET; /* set default button */
      MsgItm[i+1].PosX = x;
      MsgItm[i+1].PosY = y;
      w = TextExt(MEDIUM_FONT, BtnPtr[i])+2*BTNX;
      MsgItm[i+1].Width = w;
      MsgItm[i+1].Depth = fh + 2*BTNY;
      MsgItm[i+1].Text  = BtnPtr[i];
      x += w+BTNOFF;
   }

   /* if we have fkeys to display, save their background */
   if(FnActive) {
      FnBgnd = GetImg(31, 190, 639, 199, NULL);

      /* show the new function key labels */
      ShowFKeys(FnLbl);
   }

   /* Center dialog */
   x = (MAX_X - MsgDlg.Width)/2;
   y = (MAX_Y - MsgDlg.Depth)/2;

   /* Initialize and show dialog */
   InitDialog(&MsgDlg);
   if(Contents) {
      char *s;

      /* suppress trailing blanks */
      for(s = Contents+strlen(Contents)-1;
          *s == ' ' && s > Contents; --s) *s = '\0';

      SetDlgItem(&MsgDlg, 0, EDSI_TXT, Contents);
   }
   ShowDialog(&MsgDlg, x, y, Title);

   /* Display strings */
   for(i = 0; i < LinCnt; i++) {
      WinText(MsgDlg.pWin, XOFF, YOFF+i*(fh+LINDIST), LinPtr[i]);
   }

   /* handle dialog */
   do {
      int j;
      i = HandleDialog(&MsgDlg, &Key);
      for(j = 0; j < 10; j++) {
         if(FnTab[j] >= 0 && Key == FKeyCodes[j]) i = FnTab[j]+!!Inp;
      }
   } while(i < 0 && i != DN_CANCEL && i != DN_OK);

   if(Inp) GetDlgItem(&MsgDlg, 0, EDGI_TXT, Inp);
   CloseDialog(&MsgDlg);

   /* restore fkeys background */
   if(FnBgnd) PutImg(31, 190, FORCE_RULE, FnBgnd);

   /* free storage for temporary strings */
   if(Msg)  free(Msg);
   if(Btns) free(Btns);
   for(d = 0; d < 10; d++) if(FnTab[d] >= 0) free(FnLbl[d]);
   va_end(ArgPtr);

   return (Inp && (i > 0)) ? i-1 : i;
}

