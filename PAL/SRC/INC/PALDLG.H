/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  PALDLG.H
   Author:  Harry Konstas/Gilles Kohl
   Started: 10. Nov. 94
   Subject: PAL Dialog include file
   -------------------------------------------------------------------- */

#ifndef _PALDLG_H
#define _PALDLG_H

/* --------------------------------------------------------------------
                           General Dialog Stuff
   -------------------------------------------------------------------- */

/* dialog flags */
#define DF_IDLERET 0x00000001L /* return IDLE_KEY when idle */

/* item flags (common to all items) */
#define IF_ESC 0x00010000L /* default ESCape item (generally button) */
#define IF_RET 0x00020000L /* default RETurn item (generally button) */
#define IF_FRT 0x00040000L /* item will get a SPACE after ALT-focus  */
#define IF_RBM 0x00000007L /* radio button mask                      */

/* NOTE: item-specific flags should be in the range defined by
         the mask 0x0000FFF8L since the first three bits are
         already used for radio buttons in a general way.
         (Any item that has one of these bits set is assumed a radio
         button - radio buttons are identified this way)
*/

/* Messages for dialog handler */
typedef enum {
   DM_INIT,     /* initialize dialog    */
   DM_SHOW,     /* show dialog          */
   DM_SETFOCUS, /* set focus to an item */
   DM_KEY,      /* process a key        */
   DM_KILL      /* destroy dialog       */
} DLGMSG;

/* Messages for item handler */
typedef enum {
   IM_INIT,      /* initialize item            */
   IM_SHOW,      /* display item               */
   IM_SETDATA,   /* set data in an item        */
   IM_KEY,       /* have item process a key    */
   IM_SETFOCUS,  /* set the focus to an item   */
   IM_LOSEFOCUS, /* remove focus from an item  */
   IM_GETDATA,   /* retrieve data from an item */
   IM_KILL       /* destroy an item            */
} ITMMSG;

/* Notification returns from items */
typedef enum {
   IN_NACK = 0, /* couldn't handle message                  */
   IN_ACK,      /* handled OK, nothing else to do           */
   IN_CHANGE,   /* item changed                             */
   IN_QUIT,     /* item requests that dialog loop be left   */
} ITMNOTIF;

/* Notification returns from dialog (on DM_KEY) */
typedef enum {
   DN_NACK = -4, /* couldn't handle message                   */
   DN_ACK,       /* handled OK, nothing else to do            */
   DN_CANCEL,    /* User hit F9 or ESC and no item handled it */
   DN_OK,        /* User hit F10 or CR and no item handled it */
} DLGNOTIF;

/* Define a type "pointer to dialog handler" */
typedef int (*DIALOGPROC)(int Msg, int n, void *p, struct SDIALOG *pDlg);

/* Define a type "pointer to item handler" */
typedef int (*ITEMPROC)(int Msg, int n, void *p,
            struct SDIALOG *pDlg, struct SDLGITEM *pItm);


/* --------------------------------------------------------------------
                      Dialog and Item structures
   -------------------------------------------------------------------- */
/* Define the item descriptor */
typedef struct SDLGITEM {
   int  PosX;        /* relative horz position of item in dialog        */
   int  PosY;        /* relative vert position of item in dialog        */
   int  Width;       /* width of item. Interpretation varies            */
   int  Depth;       /* depth of item. Interpretation varies            */
   long Flags;       /* various item style and status bits              */
   char *Text;       /* associated label or text string                 */
   void *Init;       /* pointer to initialization data if any           */
   ITEMPROC ItmProc; /* pointer to function that handles item           */
   int  Size;        /* size of private data - required for subclassing */
   void *Data;       /* pointer to private data managed by item         */
} DLGITEM;


/* Define the dialog descriptor */
typedef struct SDIALOG {
   int  Width;         /* width of dialog                           */
   int  Depth;         /* depth of dialog                           */
   long Flags;         /* various dialog style and status bits      */
   int  InitFocus;     /* item that first gets the focus            */
   int  ItmCnt;        /* number of items in following array        */
   DLGITEM *Items;     /* pointer to the item array                 */
   DIALOGPROC DlgProc; /* pointer to function that handles dialog   */
   PALWIN *pWin;       /* pointer to associated window              */
   void *Data;         /* pointer to private data managed by dialog */
   int  CurFocus;      /* index of item that actually has the focus */
} DIALOG;


/* --------------------------------------------------------------------
                         Item-specific stuff
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                              E D I T
   -------------------------------------------------------------------- */
/* private data of 'Edit' item */
typedef struct {
   char *Buf;    /* current input buffer                              */
   int  BufLen;  /* max characters allowed in buffer                  */
   int  CurPos;  /* current insertion point                           */
   int  WinChr;  /* max characters that fit in window                 */
   int  WinOff;  /* buffers first visible character in input window   */
   int  WinPos;  /* position of (actual) edit window relative to item */
   int  WinLen;  /* inner length of edit window in pixels             */
   int  Font;    /* font to use for label and window                  */
   int  Focus;   /* do we currently have the focus ?                  */
   int  SelFlag; /* selection indicator                               */
   char *Backup; /* backup copy of last input in case user presses ESC*/
} IHEDIT;

/* optional initialization data of edit item */
typedef struct {
   char *Contents; /* initial buffer contents */
   int  RealLen;   /* real length of buffer   */
   int  Font;      /* Id of font to be used   */
} IHEDINIT;

/* Edit item's GetDlgItem variants */
typedef enum {
   EDGI_TXT, /* get edit buffer as text (copy)  */
   EDGI_PTR, /* get pointer to actual buffer    */
   EDGI_INT  /* get buffer converted to integer */
   /* caution, modifiy CBGI_IDX if adding here */
} EDGI;

/* Edit item's SetDlgItem variants */
typedef enum {
   EDSI_TXT, /* set edit buffer as text (copy)  */
} EDSI;

/* --------------------------------------------------------------------
                              S T A T I C
   -------------------------------------------------------------------- */
/* private data of static text item */
typedef struct {
   int  BckGnd;  /* Color to use for background                      */
   int  Font;    /* Id of font to be used                            */
   char *Text;   /* current text (NULL means use item text property) */
} IHSTATIC;

/* optional initialization data of static text item */
typedef struct {
   int  BckGnd;   /* Color to use for background */
   int  Font;     /* Id of font to be used       */
} IHSTINIT;

/* Static item's SetDlgItem variants */
typedef enum {
   STSI_LBL, /* set label */
} STSI;

#define IF_STLFT 0x00000008L /* align static text left */

/* --------------------------------------------------------------------
                             B U T T O N
   -------------------------------------------------------------------- */
/* private data of button item */
typedef struct {
   int  Font;    /* Id of font to be used                            */
   char *Text;   /* current text (NULL means use item text property) */
   int  Focus;   /* do we currently have the focus ?                  */
} IHBUTTON;

/* optional initialization data of button item */
typedef struct {
   int  Font;     /* Id of font to be used       */
} IHBNINIT;

/* Button item's SetDlgItem variants */
typedef enum {
   BNSI_LBL, /* set label of button */
} BNSI;


/* --------------------------------------------------------------------
                              C O M B O
   -------------------------------------------------------------------- */
/* private data of combo box item */
typedef struct {
   IHEDIT Ed;
   MENU CbMnu;
   char *List;
} IHCOMBO;

/* mandatory initialization data of combo box item */
typedef struct {
   IHEDINIT Ei;
   char *List;
}  IHCBINIT;

/* Combo item's GetDlgItem variants */
typedef enum {
   /* get index into selection list, or -1 if not found */
   CBGI_IDX = EDGI_INT+1, /* start here because of subclassing */
} CBGI;

/* --------------------------------------------------------------------
                            C H E C K B O X
   -------------------------------------------------------------------- */
/* private data of checkbox item */
typedef struct {
   int  Font;    /* Id of font to be used                            */
   char *Text;   /* current text (NULL means use item text property) */
   int  Focus;   /* do we currently have the focus ?                 */
   int  Checked; /* checked or unchecked                             */
} IHCHECK;

/* optional initialization data of checkbox item */
typedef struct {
   int  Font;     /* Id of font to be used          */
   int  Checked;  /* initially checked or unchecked */
} IHCKINIT;

/* Check item's SetDlgItem variants */
typedef enum {
   CKSI_LBL,   /* set label of checkbox */
   CKSI_STATE, /* set state of checkbox */
} CKSI;

/* Check item's GetDlgItem variants */
typedef enum {
   CKGI_STATE, /* get state of checkbox */
} CKGI;


/* --------------------------------------------------------------------
                             B I T M A P
   -------------------------------------------------------------------- */
/* private data of bitmap item */
typedef struct {
   IMGHDR *pBmp;
   int Free;
} IHBITMAP;

/* mandatory initialization data of bitmap item */
typedef struct {
   IMGHDR *pBmp;
} IHBMINIT;

/* Bitmap item's SetDlgItem variants */
typedef enum {
   BMSI_PTR,
   BMSI_FIL
} BMSI;

/* --------------------------------------------------------------------
                              G R O U P
   -------------------------------------------------------------------- */
/* private data of group item */
typedef struct {
   int  BckGnd;  /* Color to use for background                      */
   int  Font;    /* Id of font to be used                            */
   char *Text;   /* current text (NULL means use item text property) */
} IHGROUP;

/* optional initialization data of group item */
typedef struct {
   int  BckGnd;   /* Color to use for background */
   int  Font;     /* Id of font to be used       */
} IHGRINIT;

/* Group item's SetDlgItem variants */
typedef enum {
   GRSI_LBL, /* set label */
} GRSI;

/* --------------------------------------------------------------------
                            R A D I O
   -------------------------------------------------------------------- */
/* private data of radio button item */
typedef struct {
   int  Font;    /* Id of font to be used                            */
   char *Text;   /* current text (NULL means use item text property) */
   int  Focus;   /* do we currently have the focus ?                 */
   int  Checked; /* checked or unchecked                             */
} IHRADIO;

/* optional initialization data of radio button item */
typedef struct {
   int  Font;     /* Id of font to be used          */
   int  Checked;  /* initially checked or unchecked */
} IHRAINIT;

/* Radio button item's SetDlgItem variants */
typedef enum {
   RASI_LBL,   /* set label of radio button */
   RASI_STATE, /* set state of radio button */
} RASI;

/* Radio button item's GetDlgItem variants */
typedef enum {
   RAGI_STATE, /* get state of checkbox */
} RAGI;

/* --------------------------------------------------------------------
                           L I S T B O X
   -------------------------------------------------------------------- */

/* private data of 'ListBox' item */
typedef struct {
   char *Text;       /* text to use for label            */
   int Font;         /* font to use for text             */
   int Focus;        /* do we currently have the focus ? */
   int CurStr;       /* currently selected string        */
   int TopStr;       /* first visible item               */
   int StrDisp;      /* number of visible strings        */
   int StrCount;     /* total number of strings          */
   int StrAlloc;     /* allocated string count           */
   int NcPaint;      /* bool: repaint frame              */
   int Upd0;         /* start of update region           */
   int Upd1;         /* end of update region             */
   int Upd2;         /* update line                      */
   int Scl0;         /* start of scroll region           */
   int Scl1;         /* end of scroll region             */
   int SclDir;       /* scroll direction                 */
   char **Strings;   /* pointer to string array          */
   char *(*Cb)(int n);  /* pointer to callback function  */
} IHLISTBOX;

/* optional initialization data of listbox item */
typedef struct {
   int  Font;     /* Id of font to be used          */
} IHLBINIT;

/* structure used in adding / modifying listbox strings */
typedef struct {
   int   StrIdx;
   char *Str;
} IHLBUPD;

/* constants used to identify listbox string positions */
#define LBPOS_FIRST  0  /* first string */
#define LBPOS_LAST  -1  /* last string  */
#define LBPOS_SORT  -2  /* sorted       */

/* ListBox item's GetDlgItem variants */
typedef enum {
   LBGI_STRTXT, /* get string text             */
   LBGI_STRPOS, /* get current selection index */
   LBGI_STRCNT, /* get string count            */
} LBGI;

/* ListBox item's SetDlgItem variants */
typedef enum {
   LBSI_STRSET, /* set a string                    */
   LBSI_STRADD, /* add a string                    */
   LBSI_STRDEL, /* remove a string                 */
   LBSI_STRCLR, /* remove all strings              */
   LBSI_STRCB,  /* set callback function           */
   LBSI_STRCNT, /* set string count (callback only)*/
   LBSI_STRPOS, /* set index                       */
} LBSI;


/* --------------------------------------------------------------------
                              Prototypes
   -------------------------------------------------------------------- */

/* ---------------
   Dialog handling
   --------------- */
int InitDialog(DIALOG *pDlg);
int SetDlgItem(DIALOG *pDlg, int ItmNbr, int n, void *pData);
int ShowDialog(DIALOG *pDlg, int x, int y, char *Caption);
int SetFocus(DIALOG *pDlg, int ItmNbr);
int HandleDialog(DIALOG *pDlg, int *pKey);
int GetDlgItem(DIALOG *pDlg, int ItmNbr, int n, void *Dest);
int CloseDialog(DIALOG *pDlg);
int GetFocus(DIALOG *pDlg);

int DhStandard(int Msg, int n, void *p, DIALOG *pDlg);

/* ------------
   Dialog items
   ------------ */
int IhEdit(int Msg, int n, void *p, DIALOG *pDlg, DLGITEM *pItm);
int IhStatic(int Msg, int n, void *p, DIALOG *pDlg, DLGITEM *pItm);
int IhButton(int Msg, int n, void *p, DIALOG *pDlg, DLGITEM *pItm);
int IhCombo(int Msg, int n, void *p, DIALOG *pDlg, DLGITEM *pItm);
int IhCheck(int Msg, int n, void *p, DIALOG *pDlg, DLGITEM *pItm);
int IhBitmap(int Msg, int n, void *p, DIALOG *pDlg, DLGITEM *pItm);
int IhGroup(int Msg, int n, void *p, DIALOG *pDlg, DLGITEM *pItm);
int IhRadio(int Msg, int n, void *p, DIALOG *pDlg, DLGITEM *pItm);
int IhListBox(int Msg, int n, void *p, DIALOG *pDlg, DLGITEM *pItm);

int LbAddString(DIALOG *pDlg, int Item, int InsBefore, char *Str);
char *LbGetString(DIALOG *pDlg, int Item, int Index);
int LbSetString(DIALOG *pDlg, int Item, int Index, char *Str);
int LbDelString(DIALOG *pDlg, int Item, int Index);
int LbGetCount(DIALOG *pDlg, int Item);
int LbGetPos(DIALOG *pDlg, int Item);
int LbSetPos(DIALOG *pDlg, int Item, int Index);
int LbClear(DIALOG *pDlg, int Item);
int LbSetCallBack(DIALOG *pDlg, int Item, char *(*pCb)(int n));
int LbSetCount(DIALOG *pDlg, int Item, int Count);

#endif

