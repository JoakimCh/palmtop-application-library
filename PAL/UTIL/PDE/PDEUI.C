/* --------------------------------------------------------------------
   Project: PAL Dialog Editor  Vr. 1.2
   Module:  PDEUI.C
   Author:  Harry Konstas / Gilles Kohl
   Started: 29. Jan '95
   Subject: User Interface module for PDE.C
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                       standard includes
   -------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <conio.h>


/* --------------------------------------------------------------------
                         local includes
   -------------------------------------------------------------------- */

#include "pal.h"
#include "pde.h"
#include "pdersrc.h"    /* PDE Dialog resource */


/* --------------------------------------------------------------------
                             variables
   -------------------------------------------------------------------- */

extern char *KeyLabels[10];
extern char *KeyLabels2[10];
extern char *OKCANLabels[10];
extern char *EditLabels[10];
extern IHCBINIT CBPICK1;

char *itypes[] = {"Edit","Static","Button","Checkbox",
                  "Radio","Group","Combo","Bitmap","ListBox","Other"};
char *fonts[]  = {"Small","Medium", "Large"};
char *colors[] = {"Black","White"};


/* --------------------------------------------------------------------
                           UCASE Edit handler
   -------------------------------------------------------------------- */

int IhUEdit(int Msg, int n, void *p, DIALOG *pDlg, DLGITEM *pItm)
{
   if(Msg == IM_KEY && (n & 0xff) != 0)
      n = (n & 0xff00) | toupper(n & 0xff);
   return IhEdit(Msg, n, p, pDlg, pItm);
 }


/* --------------------------------------------------------------------
                      DIALOG WINDOW SIZE SETUP
   -------------------------------------------------------------------- */

void DlgWinSize(DIAGDEF *pDialog)
{
   PALWIN *pWin;
   int f, type, x1, x2, y1, y2;

   if(!pDialog->DiagFlag) return; /* not defined */

   ClrScrn(NORMAL);

   /* display only items */
   for(f=1;f<pDialog->INum+1;f++) {

      x1 = pDialog->item[f]->x1;
      x2 = pDialog->item[f]->x2;
      y1 = pDialog->item[f]->y1;
      y2 = pDialog->item[f]->y2;
      type = pDialog->item[f]->type;

      DisField(type, SOLID, x1, y1, x2, y2, pDialog->item[f]->label);
   }

   x1=pDialog->x1;
   y1=pDialog->y1;
   x2=pDialog->x2;
   y2=pDialog->y2;

   if(CreateRectan(pDialog, S_MODE, &x1, &y1, &x2, &y2)) {
      pDialog->x1=x1;
      pDialog->y1=y1;
      pDialog->x2=x2;
      pDialog->y2=y2;
   }

   else {

      x1=pDialog->x1;
      y1=pDialog->y1;
      x2=pDialog->x2;
      y2=pDialog->y2;
   }

   ClrScrn(NORMAL);
   ShowFKeys(KeyLabels2);

   if(strlen(pDialog->name)<2)
      strcpy(pDialog->name,"untitled");

   pWin  = OpenWin(WS_HP200, x1, y1, x2, y2, pDialog->name);
   /* static window, free memory */
   free(pWin->Buffer);
   free(pWin);

   /* display items again */
   for(f=1;f<pDialog->INum+1;f++) {

      x1 = pDialog->item[f]->x1;
      x2 = pDialog->item[f]->x2;
      y1 = pDialog->item[f]->y1;
      y2 = pDialog->item[f]->y2;
      type = pDialog->item[f]->type;

      DisField(type, SOLID, x1, y1, x2, y2, pDialog->item[f]->label);
      pDialog->Modified = TRUE;
   }

}


/* --------------------------------------------------------------------
                              DIALOG SETUP
   -------------------------------------------------------------------- */

DIAGDEF *DefineDiag(DIAGDEF *pDialog)
{
   PALWIN *pWin;
   int  f, istat, x1, y1, x2, y2;

   if(!pDialog->DiagFlag)
      ClrScrn(ABOUT);

   ShowFKeys(KeyLabels2);
   pDialog->Modified = FALSE;

   InitDialog(&DlgSetup);

   SetDlgItem(&DlgSetup, DLGHANDLER, EDSI_TXT, "DhStandard");

   if(strlen(pDialog->fname)<1)
      strcpy(pDialog->fname,"resource");  /* default file */

   SetDlgItem(&DlgSetup, DLGFNAME, EDSI_TXT, pDialog->fname);
   SetDlgItem(&DlgSetup, DLGFLAGS, EDSI_TXT, "0L");

   ShowDialog(&DlgSetup, 60, 30, "New Dialog Setup");
   ShowFKeys(OKCANLabels);

   istat = HandleDialog(&DlgSetup, NULL);

   if(istat==DN_CANCEL) {
      CloseDialog(&DlgSetup);
      if(pDialog->DiagFlag)
         ShowFKeys(KeyLabels2);
      else
         ShowFKeys(KeyLabels);
      return pDialog;
   }

   /* release allocated storage */
   for(f=0;f<NUM_OF_ELMNT;f++) {
      free(pDialog->item[f]);
      pDialog->item[f] = NULL;
   }

   pDialog->INum = 0;
   pDialog->Modified = FALSE;

   GetDlgItem(&DlgSetup, DLGNAME, EDGI_TXT, pDialog->name);
   GetDlgItem(&DlgSetup, DLGFNAME, EDGI_TXT, pDialog->fname);
   GetDlgItem(&DlgSetup, DLGHANDLER, EDGI_TXT, pDialog->handler);

   pDialog->focus=0;      /* default focus = 0 */

   GetDlgItem(&DlgSetup, DLGFLAGS, EDGI_TXT, pDialog->flags);
   CloseDialog(&DlgSetup);

   if(!strlen(pDialog->name)) {
      if(pDialog->DiagFlag)
         ShowFKeys(KeyLabels2);
      else
         ShowFKeys(KeyLabels);
   }

   if(!strlen(pDialog->fname))
      strcpy(pDialog->fname,"resource");

   /* Try to load file if available */
   ClrScrn(ABOUT);
   if(LoadDlg(pDialog, pDialog->fname, pDialog->name)) return pDialog;

   ClrScrn(NORMAL);
   WriteText(180,80, " Use arrows to size window.");
   WriteText(180,100,"(SHIFT-ARROWS for precision)");

   x1=40;y1=20;x2=594;y2=174;
   CreateRectan(pDialog, S_MODE, &x1, &y1, &x2, &y2);

   pDialog->x1=x1;
   pDialog->y1=y1;
   pDialog->x2=x2;
   pDialog->y2=y2;

   ClrScrn(ABOUT);
   ShowFKeys(KeyLabels2);

   if(strlen(pDialog->name)<2)
      strcpy(pDialog->name,"untitled");

   if(x2!=0) {

     pWin  = OpenWin(WS_HP200, x1, y1, x2, y2, pDialog->name);
     /* static window, free memory */
     free(pWin->Buffer);
     free(pWin);

     pDialog->x1=x1;
     pDialog->y1=y1;
     pDialog->x2=x2;
     pDialog->y2=y2;
     pDialog->DiagFlag=1; /* dialog window defined */
     pDialog->INum=0;     /* no items yet */

   }

   ShowFKeys(KeyLabels2);
   return pDialog;
}


/* --------------------------------------------------------------------
                           Save Resource screen
   -------------------------------------------------------------------- */

void Save(DIAGDEF *pDialog)
{
   PALWIN *pWin;
   int istat;
   char cfile[100];       /* .C filename */
   char hfile[100];       /* .H filename */

   if((!pDialog->DiagFlag)||(!pDialog->INum)) return;  /* nothing to save */

   if(pDialog->Modified) {
      InitDialog(&DlgSave);
      SetDlgItem(&DlgSave, DLGFILE, EDSI_TXT, pDialog->fname);

      ShowDialog(&DlgSave, 55, 60, "Save Dialog?");
      ShowFKeys(OKCANLabels);

      istat = HandleDialog(&DlgSave, NULL);

      if(istat==DN_CANCEL) {
         CloseDialog(&DlgSave);
         ShowFKeys(KeyLabels2);
         return;
      }

      GetDlgItem(&DlgSave, DLGFILE, EDGI_TXT, pDialog->fname);
      CloseDialog(&DlgSave);
   }

   AddExt(cfile, pDialog->fname, ".c");
   AddExt(hfile, pDialog->fname, ".h");

   pWin = OpenWin(WS_HP200, 140, 50, 470, 110, "Attention.");
   if(!pDialog->Modified) {
      WriteText(175,80,  " Saving TESTDLG files...");
      MsDelay(800);
   }

   if(pDialog->Modified) {
      WriteText(175,80,  " Saving resource files...");
      SaveC(pDialog, cfile, hfile);   /* save .C resource */
      SaveH(pDialog, hfile);          /* save .H resource */
   }

   SaveTest(cfile, hfile);    /* save TESTDLG.C file */

   pDialog->Modified = FALSE;
   CloseWin(pWin);
   ShowFKeys(KeyLabels2);
}


/* --------------------------------------------------------------------
                           Get Item setup
   -------------------------------------------------------------------- */


void GetItemSet(DIAGDEF *pDialog, int ItmNum)

{
   int type;
   char buffer[100];

   GetDlgItem(&Itmset, ITMNAME, EDGI_TXT, pDialog->item[ItmNum]->name);

   /* if no name, create default */
   if(strlen(pDialog->item[ItmNum]->name) < 2) {
      itoa(ItmNum, buffer, 10);
      strcpy(pDialog->item[ItmNum]->name, pDialog->name);
      strupr(pDialog->item[ItmNum]->name);
      strcat(pDialog->item[ItmNum]->name, buffer);
   }

   GetDlgItem(&Itmset, ITMLABEL, EDGI_TXT, pDialog->item[ItmNum]->label);
   GetDlgItem(&Itmset, ITMFLAGS, EDGI_TXT, pDialog->item[ItmNum]->flags);

   GetDlgItem(&Itmset, ITMTYPE, CBGI_IDX, &type);
   type++;

   if((type==0)||(type==NUM_OF_TYPES)) {
      type=NUM_OF_TYPES-1;
      strcpy(pDialog->item[ItmNum]->TypeName,buffer); /* user defined handler */
   }

   pDialog->item[ItmNum]->type=type;
}


/* --------------------------------------------------------------------
                             New Item Entry
   -------------------------------------------------------------------- */

void NewItem(DIAGDEF *pDialog)

{
   int istat;

   if(!pDialog->DiagFlag) return;  /* dialog not defined */

   InitDialog(&Itmset);
   SetDlgItem(&Itmset, ITMFLAGS, EDSI_TXT, "0L");
   SetDlgItem(&Itmset, ITMTYPE , EDSI_TXT, "Edit");

   ShowDialog(&Itmset, 60, 40, "New Item");
   ShowFKeys(OKCANLabels);

   istat = HandleDialog(&Itmset, NULL);

   if(istat==DN_CANCEL) {
      CloseDialog(&Itmset);
      ShowFKeys(KeyLabels2);
      return;
   }

   /* Allocate storage for new item */
   pDialog->INum++;
   pDialog->item[pDialog->INum]=malloc(sizeof(FIELDEF));
   pDialog->Modified = TRUE;

   pDialog->item[pDialog->INum]->InitFlag = 0; /* no init data yet */
   pDialog->item[pDialog->INum]->ChkFlag = 0;  /* not Checked */
   pDialog->item[pDialog->INum]->List[0] = 0;  /* no init list */
   pDialog->item[pDialog->INum]->dflt[0] = 0;  /* no dflt data */
   pDialog->item[pDialog->INum]->TypeName[0]=0;
   pDialog->item[pDialog->INum]->font = 2;     /* Medium by default */
   pDialog->item[pDialog->INum]->color= 1;     /* Black by default  */
   pDialog->item[pDialog->INum]->lenght= 20;   /* buffer lenght */

   GetItemSet(pDialog, pDialog->INum);         /* get mandatory values */

   CloseDialog(&Itmset);

   /* Mandatory INIT types */
   switch(pDialog->item[pDialog->INum]->type) {

      case COMBO:
         pDialog->item[pDialog->INum]->InitFlag = 1;
         EditInit(pDialog, pDialog->INum);        /* INIT data */
         break;

      case BITMAP:
         pDialog->item[pDialog->INum]->InitFlag = 1;
         EditInit(pDialog, pDialog->INum);        /* INIT data */
         break;

      case OTHER:
         EditInit(pDialog, pDialog->INum);
         break;
   }

   ItemHandler(pDialog);           /* place/size item */
   ShowFKeys(KeyLabels2);
}


/* --------------------------------------------------------------------
                              Edit items
   -------------------------------------------------------------------- */

void EditItems(DIAGDEF *pDialog)

{
   PALWIN *pMsg;
   int x1, x2, y1, y2, type, istat, f, c, step=2;
   int ix, typeNum, TabPos;
   char label[50];
   static int EditFocus;

   if(EditFocus==0) EditFocus=1;

   if(!pDialog->INum) return;  /* nothing to do */

   typeNum=EditFocus;

   ShowFKeys(EditLabels);

   do {
      x1=pDialog->item[typeNum]->x1;
      y1=pDialog->item[typeNum]->y1;
      x2=pDialog->item[typeNum]->x2;
      y2=pDialog->item[typeNum]->y2;
      type=pDialog->item[typeNum]->type;
      strcpy(label,pDialog->item[typeNum]->label);

      DispInfo(pDialog, type, x1, y1, x2, y2);

      /* Erase current display */
      DisField(type, SOLID, x1, y1, x2, y2, label);

      /* Display it dashed */
      DisField(type, DASHED, x1, y1, x2, y2, label);

      istat=PlaceItem(pDialog, typeNum, step, type, x1, y1, x2, y2, label);

      if(((istat==DEL_KEY)||(istat==F4_KEY))&&(pDialog->INum>1)) {

         pMsg = OpenWin(WS_HP200, 120, 50, 470, 110, "Attention!");
         WriteText(200,70, " Remove this item?");
         WriteText(200,90, "Hit [Y] to confirm.");
         printf("\a");  /* bell */
         c=getch();
         CloseWin(pMsg);

         if((c==89)||(c==121)) {   /* [Y] key */
            pDialog->Modified = TRUE;
            DisField(type, SOLID, x1, y1, x2, y2, label);

            /* delete item from structure */
            for(f=typeNum;f<pDialog->INum;f++) {
               pDialog->item[f]->type     = pDialog->item[f+1]->type;
               pDialog->item[f]->x1       = pDialog->item[f+1]->x1;
               pDialog->item[f]->y1       = pDialog->item[f+1]->y1;
               pDialog->item[f]->x2       = pDialog->item[f+1]->x2;
               pDialog->item[f]->y2       = pDialog->item[f+1]->y2;
               pDialog->item[f]->font     = pDialog->item[f+1]->font;
               pDialog->item[f]->color    = pDialog->item[f+1]->color;
               pDialog->item[f]->lenght   = pDialog->item[f+1]->lenght;
               pDialog->item[f]->InitFlag = pDialog->item[f+1]->InitFlag;
               pDialog->item[f]->ChkFlag  = pDialog->item[f+1]->ChkFlag;

               strcpy(pDialog->item[f]->flags,    pDialog->item[f+1]->flags);
               strcpy(pDialog->item[f]->name,     pDialog->item[f+1]->name);
               strcpy(pDialog->item[f]->label,    pDialog->item[f+1]->label);
               strcpy(pDialog->item[f]->TypeName, pDialog->item[f+1]->TypeName);
               strcpy(pDialog->item[f]->dflt,     pDialog->item[f+1]->dflt);
               strcpy(pDialog->item[f]->List,     pDialog->item[f+1]->List );
            }
            free(pDialog->item[pDialog->INum]);   /* free item memory */
            pDialog->item[pDialog->INum] = NULL;
            pDialog->INum--;
         }
      istat = F10_KEY;
      }

      if((istat==F3_KEY)&&(typeNum>0)) {

        pDialog->Modified = TRUE;
        x1 = pDialog->item[typeNum]->x1;
        y1 = pDialog->item[typeNum]->y1;
        x2 = pDialog->item[typeNum]->x2;
        y2 = pDialog->item[typeNum]->y2;
        strcpy(label,pDialog->item[typeNum]->label);
        DisField(type, SOLID, x1, y1, x2, y2, label);

        /* edit values */
        EditISetup(pDialog, typeNum);   /* Edit Item */
        type = pDialog->item[typeNum]->type;
        strcpy(label,pDialog->item[typeNum]->label);
        DisField(type, SOLID, x1, y1, x2, y2, label);
        ShowFKeys(KeyLabels2);
        EditFocus=typeNum;
        return;
      }

      if(istat==F6_KEY) {   /* edit INIT data */
        x1 = pDialog->item[typeNum]->x1;
        y1 = pDialog->item[typeNum]->y1;
        x2 = pDialog->item[typeNum]->x2;
        y2 = pDialog->item[typeNum]->y2;
        strcpy(label,pDialog->item[typeNum]->label);
        DisField(type, SOLID, x1, y1, x2, y2, label);

        EditInit(pDialog, typeNum);
        pDialog->Modified = TRUE;
        type = pDialog->item[typeNum]->type;
        DisField(type, SOLID, x1, y1, x2, y2, label);
        ShowFKeys(KeyLabels2);
        EditFocus=typeNum;
        return;
      }

      if(istat==F7_KEY) {
         pDialog->Modified = TRUE;
         pDialog->focus=typeNum-1;  /* F7: sets focus */
         pMsg = OpenWin(WS_HP200, 140, 40, 470, 120, "Attention.");
         WriteText(180,70,  "Initial focus is set to:");
         TabPos=((33-strlen(pDialog->item[typeNum]->name))*5);
         WriteText(140 + TabPos, 90, pDialog->item[typeNum]->name);
         GetKey();
         CloseWin(pMsg);
      }

      if((istat==F5_KEY)||(istat==TAB_KEY)) {
         typeNum++;
         if(typeNum>pDialog->INum) typeNum=1;
      }

      if(istat==F8_KEY) {
         ix = PickItm(pDialog);
         ChangePos(pDialog, typeNum, ix);
         istat=F10_KEY;
      }

      if((istat==STAB_KEY)) {
         typeNum--;
         if(typeNum==0) typeNum=pDialog->INum;
      }

      if(!istat) {
         DisField(type, SOLID, x1, y1, x2, y2, label);
         ShowFKeys(KeyLabels2);
         EditFocus=typeNum;
         return;
      }

   } while((istat!=F10_KEY)&&(istat!=KEYENTER));
   pDialog->Modified=TRUE;
   ShowFKeys(KeyLabels2);
   EditFocus=typeNum;
}


/* --------------------------------------------------------------------
                           Edit Item setup
   -------------------------------------------------------------------- */

void EditISetup(DIAGDEF *pDialog, int ItmNum)
{
   int istat;

   InitDialog(&Itmset);

   /* Get old values */
   SetDlgItem(&Itmset, ITMFLAGS, EDSI_TXT, pDialog->item[ItmNum]->flags);

   SetDlgItem(&Itmset, ITMTYPE , EDSI_TXT, itypes[pDialog->item[ItmNum]->type-1]);
   SetDlgItem(&Itmset, ITMLABEL, EDSI_TXT, pDialog->item[ItmNum]->label);
   SetDlgItem(&Itmset, ITMNAME,  EDSI_TXT, pDialog->item[ItmNum]->name);

   ShowDialog(&Itmset, 60, 40, "Edit Item setup");
   ShowFKeys(OKCANLabels);

   istat = HandleDialog(&Itmset, NULL);

   if(istat==DN_CANCEL) {
     CloseDialog(&Itmset);
     ShowFKeys(EditLabels);
     return;
   }

   pDialog->Modified = TRUE;
   GetItemSet(pDialog, ItmNum);  /* get new values */

   CloseDialog(&Itmset);
   ShowFKeys(EditLabels);

}


/* --------------------------------------------------------------------
                         Edit Item INIT structure
   -------------------------------------------------------------------- */

void EditInit(DIAGDEF *pDialog, int ItmNum)
{
   int istat, indx;
   char buffer[20];

   switch(pDialog->item[ItmNum]->type) {

      case TEXT:

         InitDialog(&edidlg);

         /* Get old values */
         SetDlgItem(&edidlg, EDI1, EDSI_TXT, fonts[pDialog->item[ItmNum]->font-1]);
         itoa(pDialog->item[ItmNum]->lenght, buffer, 10);
         SetDlgItem(&edidlg, EDI2, EDSI_TXT, buffer);
         SetDlgItem(&edidlg, EDI3, EDSI_TXT, pDialog->item[ItmNum]->dflt);

         ShowDialog(&edidlg, 90, 40, "Edit IhEdit INIT data");
         ShowFKeys(OKCANLabels);

         istat = HandleDialog(&edidlg, NULL);

         if(istat==DN_CANCEL) {
            CloseDialog(&edidlg);
            ShowFKeys(EditLabels);
            return;
         }

         pDialog->Modified = TRUE;
         pDialog->item[ItmNum]->InitFlag = 1;
         GetDlgItem(&edidlg, EDI1, CBGI_IDX, &indx);
         pDialog->item[ItmNum]->font = indx + 1;
         GetDlgItem(&edidlg, EDI2, EDGI_INT, &pDialog->item[ItmNum]->lenght);
         GetDlgItem(&edidlg, EDI3, EDGI_TXT, pDialog->item[ItmNum]->dflt);

         CloseDialog(&edidlg);
         ShowFKeys(EditLabels);
         break;


      case LABEL:

         InitDialog(&stidlg);

         /* Get old values */
         SetDlgItem(&stidlg, STI1, EDSI_TXT, fonts[pDialog->item[ItmNum]->font-1]);
         SetDlgItem(&stidlg, STI2, EDSI_TXT, colors[pDialog->item[ItmNum]->color-1]);

         ShowDialog(&stidlg, 90, 40, "Edit IhStatic INIT data");
         ShowFKeys(OKCANLabels);

         istat = HandleDialog(&stidlg, NULL);

         if(istat==DN_CANCEL) {
            CloseDialog(&stidlg);
            ShowFKeys(EditLabels);
            return;
         }

         pDialog->Modified = TRUE;
         pDialog->item[ItmNum]->InitFlag = 1;
         GetDlgItem(&stidlg, STI1, CBGI_IDX, &indx);
         pDialog->item[ItmNum]->font = indx+1;
         GetDlgItem(&stidlg, STI2, CBGI_IDX, &indx);
         pDialog->item[ItmNum]->color = indx+1;

         CloseDialog(&stidlg);
         ShowFKeys(EditLabels);
         break;


      case BUTTON:

         InitDialog(&btidlg);

         /* Get old values */
         SetDlgItem(&btidlg, BTI1, EDSI_TXT, fonts[pDialog->item[ItmNum]->font-1]);

         ShowDialog(&btidlg, 70, 40, "Edit IhButton INIT data");
         ShowFKeys(OKCANLabels);

         istat = HandleDialog(&btidlg, NULL);

         if(istat==DN_CANCEL) {
            CloseDialog(&btidlg);
            ShowFKeys(EditLabels);
            return;
         }

         pDialog->Modified = TRUE;
         pDialog->item[ItmNum]->InitFlag = 1;
         GetDlgItem(&btidlg, BTI1, CBGI_IDX, &indx);
         pDialog->item[ItmNum]->font = indx + 1;

         CloseDialog(&btidlg);
         ShowFKeys(EditLabels);
         break;


      case COMBO:

         InitDialog(&cmidlg);

         /* Get old values */
         SetDlgItem(&cmidlg, CMI1, EDSI_TXT, fonts[pDialog->item[ItmNum]->font-1]);
         itoa(pDialog->item[ItmNum]->lenght, buffer, 10);
         SetDlgItem(&cmidlg, CMI2, EDSI_TXT, buffer);
         SetDlgItem(&cmidlg, CMI3, EDSI_TXT, pDialog->item[ItmNum]->dflt);
         SetDlgItem(&cmidlg, CMI4, EDSI_TXT, pDialog->item[ItmNum]->List);

         ShowDialog(&cmidlg, 90, 40, "Edit IhCombo INIT data");
         ShowFKeys(OKCANLabels);

         istat = HandleDialog(&cmidlg, NULL);

         if(istat==DN_CANCEL) {
            CloseDialog(&cmidlg);
            ShowFKeys(EditLabels);
            return;
         }

         pDialog->Modified = TRUE;
         pDialog->item[ItmNum]->InitFlag = 1;
         GetDlgItem(&cmidlg, CMI1, CBGI_IDX, &indx);
         pDialog->item[ItmNum]->font = indx+1;
         GetDlgItem(&cmidlg, CMI2, EDGI_INT, &pDialog->item[ItmNum]->lenght);
         GetDlgItem(&cmidlg, CMI3, EDGI_TXT, pDialog->item[ItmNum]->dflt);
         GetDlgItem(&cmidlg, CMI4, EDGI_TXT, pDialog->item[ItmNum]->List);

         CloseDialog(&cmidlg);
         ShowFKeys(EditLabels);
         break;


      case CHECK:

         InitDialog(&chidlg);

         /* Get old values */
         if(pDialog->item[ItmNum]->ChkFlag)
            SetDlgItem(&chidlg, CHI1, CKSI_STATE, (void *)1);

         SetDlgItem(&chidlg, CHI2, EDSI_TXT, fonts[pDialog->item[ItmNum]->font-1]);

         ShowDialog(&chidlg, 90, 40, "Edit IhCheck INIT data");
         ShowFKeys(OKCANLabels);

         istat = HandleDialog(&chidlg, NULL);

         if(istat==DN_CANCEL) {
            CloseDialog(&chidlg);
            ShowFKeys(EditLabels);
            return;
         }

         pDialog->Modified = TRUE;
         pDialog->item[ItmNum]->InitFlag = 1;
         GetDlgItem(&chidlg, CHI1, CKGI_STATE,
            &pDialog->item[ItmNum]->ChkFlag);

         GetDlgItem(&chidlg, CHI2, CBGI_IDX, &indx);
         pDialog->item[ItmNum]->font = indx+1;

         CloseDialog(&chidlg);
         ShowFKeys(EditLabels);
         break;

      case OPTION:
         InitDialog(&raidlg);

         /* Get old values */
         if(pDialog->item[ItmNum]->ChkFlag)
            SetDlgItem(&raidlg, RAI1, CKSI_STATE, (void *)1);

         SetDlgItem(&raidlg, RAI2, EDSI_TXT, fonts[pDialog->item[ItmNum]->font-1]);

         ShowDialog(&raidlg, 90, 40, "Edit IhRadio INIT data");
         ShowFKeys(OKCANLabels);

         istat = HandleDialog(&raidlg, NULL);

         if(istat==DN_CANCEL) {
            CloseDialog(&raidlg);
            ShowFKeys(EditLabels);
            return;
         }

         pDialog->Modified = TRUE;
         pDialog->item[ItmNum]->InitFlag = 1;
         GetDlgItem(&raidlg, RAI1, CKGI_STATE,
            &pDialog->item[ItmNum]->ChkFlag);

         GetDlgItem(&raidlg, RAI2, CBGI_IDX, &indx);
         pDialog->item[ItmNum]->font = indx+1;

         CloseDialog(&raidlg);
         ShowFKeys(EditLabels);
         break;


      case GROUP:
         InitDialog(&gridlg);

         /* Get old values */
         SetDlgItem(&gridlg, GRI1, EDSI_TXT, fonts[pDialog->item[ItmNum]->font-1]);
         SetDlgItem(&gridlg, GRI2, EDSI_TXT, colors[pDialog->item[ItmNum]->color-1]);

         ShowDialog(&gridlg, 90, 40, "Edit IhGroup INIT data");
         ShowFKeys(OKCANLabels);

         istat = HandleDialog(&gridlg, NULL);

         if(istat==DN_CANCEL) {
            CloseDialog(&gridlg);
            ShowFKeys(EditLabels);
            return;
         }

         pDialog->Modified = TRUE;
         pDialog->item[ItmNum]->InitFlag = 1;
         GetDlgItem(&gridlg, GRI1, CBGI_IDX, &indx);
         pDialog->item[ItmNum]->font = indx+1;
         GetDlgItem(&gridlg, GRI2, CBGI_IDX, &indx);
         pDialog->item[ItmNum]->color = indx+1;

         CloseDialog(&gridlg);
         ShowFKeys(EditLabels);
         break;


      case BITMAP:
         InitDialog(&bmidlg);

         /* Get old values */
         if(strlen(pDialog->item[ItmNum]->TypeName)<2)
            strcpy(pDialog->item[ItmNum]->TypeName, "NULL");

         SetDlgItem(&bmidlg, BMI1, EDSI_TXT, pDialog->item[ItmNum]->TypeName);

         ShowDialog(&bmidlg, 70, 40, "Edit IhBitmap INIT data");
         ShowFKeys(OKCANLabels);

         istat = HandleDialog(&bmidlg, NULL);

         if(istat==DN_CANCEL) {
            CloseDialog(&bmidlg);
            ShowFKeys(EditLabels);
            return;
         }

         pDialog->Modified = TRUE;
         pDialog->item[ItmNum]->InitFlag = 1;
         GetDlgItem(&bmidlg, BMI1, EDGI_TXT, pDialog->item[ItmNum]->TypeName);

         CloseDialog(&bmidlg);
         ShowFKeys(EditLabels);
         break;


      case LISTBOX:
         InitDialog(&lbidlg);

         /* Get old values */
         SetDlgItem(&lbidlg, LBI1, EDSI_TXT, fonts[pDialog->item[ItmNum]->font-1]);

         ShowDialog(&lbidlg, 70, 40, "Edit IhListBox INIT data");
         ShowFKeys(OKCANLabels);

         istat = HandleDialog(&lbidlg, NULL);

         if(istat==DN_CANCEL) {
            CloseDialog(&lbidlg);
            ShowFKeys(EditLabels);
            return;
         }

         pDialog->Modified = TRUE;
         pDialog->item[ItmNum]->InitFlag = 1;
         GetDlgItem(&lbidlg, LBI1, CBGI_IDX, &indx);
         pDialog->item[ItmNum]->font = indx + 1;

         CloseDialog(&lbidlg);
         ShowFKeys(EditLabels);
         break;


      case OTHER:
         InitDialog(&otheritem);

         /* Get old values */
         if(strlen(pDialog->item[ItmNum]->TypeName)<2)
            strcpy(pDialog->item[ItmNum]->TypeName, "MyHandler");

         SetDlgItem(&otheritem, OTHERITEM1, EDSI_TXT, pDialog->item[ItmNum]->TypeName);

         ShowDialog(&otheritem, 140, 50, "Edit OTHER item data");
         ShowFKeys(OKCANLabels);

         istat = HandleDialog(&otheritem, NULL);

         if(istat==DN_CANCEL) {
            CloseDialog(&otheritem);
            ShowFKeys(EditLabels);
            return;
         }

         pDialog->Modified = TRUE;
         GetDlgItem(&otheritem, OTHERITEM1, EDGI_TXT, pDialog->item[ItmNum]->TypeName);

         CloseDialog(&otheritem);
         ShowFKeys(EditLabels);
         break;

      default:
      return;
   }
}

/* --------------------------------------------------------------------
                       Create Dialog name pick list
   -------------------------------------------------------------------- */

void PickDlg(char *hfile, char *dname)
{
   FILE *pDEF;

   int  n=0;          /* number of elements in pick list */
   char *pSAVE;       /* save combo's initial pointer */
   char *Plst;        /* new combo pointer */
   char sname[80];

   char wname[80];
   char buffer[110];

   if(!(pDEF = fopen(hfile,"r"))) return;

   /* create list */

   Plst=malloc(2000); /* temporary combo list storage */
   Plst[0] = 0;

   while(fgets(buffer, 100, pDEF)) {
      if(strstr(buffer, "/*[PDE:")) {
         sscanf(buffer, "/*[PDE:%[^]]", dname);

         /* add new item to list */
         if(strlen(Plst)) strcat(Plst,"|");
         strcat(Plst, dname);
         n++;
      }

   }
   fclose(pDEF);

   /* transfer pointer address to Combo */
   pSAVE = CBPICK1.List;
   CBPICK1.List = Plst;

   if(n==1) {               /* if only 1 item, no picklist */
      CBPICK1.List = pSAVE;
      ShowFKeys(EditLabels);
      free(Plst);
      return;
   }

   /* show picklist */
   InitDialog(&pick);

   SetDlgItem(&pick, PICK1, EDSI_TXT, dname);
   strcpy(wname, "Available dialogs in: ");
   AddExt(sname, hfile, " ");
   strcat(wname, sname);

   ShowDialog(&pick, 90, 40, wname);
   ShowFKeys(OKCANLabels);

   HandleDialog(&pick, NULL);

   GetDlgItem(&pick, PICK1, EDGI_TXT, dname);
   CBPICK1.List = pSAVE;
   CloseDialog(&pick);
   ShowFKeys(EditLabels);
   free(Plst);
}


/* --------------------------------------------------------------------
                       Create item name pick list
   -------------------------------------------------------------------- */

int PickItm(DIAGDEF *pDialog)
{
   int f, istat, indx=0;
   char *pSAVE;       /* save combo's initial pointer */
   char *Plst;        /* new combo pointer */
   char buffer[80];

   if(pDialog->INum<2) return 0; /* nothing to do */

   /* create list */

   Plst=malloc(2000); /* temporary combo list storage */
   Plst[0] = 0;

   for(f=1;f<pDialog->INum;f++) {
      if(strlen(Plst)) strcat(Plst,"|");
      itoa(f, buffer, 10);
      strcat(Plst, buffer);
      strcat(Plst, " ");
      strcat(Plst, pDialog->item[f]->name);
   }

   /* transfer pointer address to Combo */
   pSAVE = CBPICK1.List;
   CBPICK1.List = Plst;

   /* show picklist */
   InitDialog(&pick);

   strcpy(buffer,"1 ");
   strcat(buffer, pDialog->item[1]->name);
   SetDlgItem(&pick, PICK1, EDSI_TXT, buffer);
   ShowDialog(&pick, 90, 40, "Select new focus position:");
   ShowFKeys(OKCANLabels);

   istat=HandleDialog(&pick, NULL);

   GetDlgItem(&pick, PICK1, CBGI_IDX, &indx);
   CBPICK1.List = pSAVE;
   CloseDialog(&pick);
   ShowFKeys(EditLabels);
   free(Plst);
   if(istat==DN_CANCEL) return 0;
   return indx+1;
}


/* --------------------------------------------------------------------
                  Sort dialog items (focus) screen
   -------------------------------------------------------------------- */

void SortDlg(DIAGDEF *pDialog)
{

   PALWIN *pMsg;
   int c;

   if(pDialog->INum < 2) return;   /* nothing to do */

   pMsg = OpenWin(WS_HP200, 120, 50, 470, 110, "Attention!");
   WriteText(200,70, "Sort dialog items?");
   WriteText(200,90, "Hit [Y] to confirm.");
   c=getch();
   CloseWin(pMsg);

   if((c==89)||(c==121)) {   /* [Y] key */
      pMsg = OpenWin(WS_HP200, 120, 50, 470, 90, "Please wait...");
      WriteText(200,70, "Sorting dialog items");
      Bubble(pDialog);
      pDialog->Modified = TRUE;
      CloseWin(pMsg);
   }
}

