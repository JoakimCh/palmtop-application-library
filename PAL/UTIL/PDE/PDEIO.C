/* --------------------------------------------------------------------
   Project: PAL Dialog Editor  Vr. 1.2
   Module:  PDEIO.C
   Author:  Harry Konstas / Gilles Kohl
   Started: 29. Jan '95
   Subject: Resource file I/O module for PDE.C
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                       standard includes
   -------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined _MSC_VER || defined _QC
#include <direct.h>
#else
#include <dir.h>
#endif

/* --------------------------------------------------------------------
                         local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "pde.h"


/* --------------------------------------------------------------------
                               variables
   -------------------------------------------------------------------- */

extern char *KeyLabels2[10];

char *types[]      = { "Window", "IhEdit", "IhStatic", "IhButton", "IhCheck",
                       "IhRadio", "IhGroup", "IhCombo", "IhBitmap",
                       "IhListBox","Other"};

char *initypes[]   = {"X", "IHEDINIT", "IHSTINIT", "IHBNINIT", "IHCKINIT",
                      "IHRAINIT","IHGRINIT","IHCBINIT","IHBMINIT",
                      "IHLBINIT","USERINIT"};

char *ininames[]   = {"X","ED","ST","BN","CK","RA","GR","CB","BM","LB","US"};
char *colornames[] = {"BLACK_COLOR", "WHITE_COLOR"};
char *fontnames[]  = {"SMALL_FONT", "MEDIUM_FONT", "LARGE_FONT"};


/* --------------------------------------------------------------------
                       Change extension of a filename
   -------------------------------------------------------------------- */

void AddExt(char *Dest, char *Source, char *Ext)
{
    char *s;
    strcpy(Dest, Source);
    s = strrchr(Dest, '.');
    if(s == NULL) s = Dest+strlen(Dest);
    strcpy(s, Ext);
}


/* --------------------------------------------------------------------
                        Delete dialog from resource
   -------------------------------------------------------------------- */

int DeleteDlg(char *sfile, char *dname)

{
   FILE *pSRC;
   FILE *pDST;
   char *stat;

   int  blen=0;
   int  fflag=FALSE;
   char buffer[255];
   char dlgname[80];
   char tmpfile[]="pdetemp.$$$";

   strcpy(dlgname, "/*[PDE:");
   strcat(dlgname, dname);
   strcat(dlgname, "]*/");

   if(!(pSRC = fopen(sfile,"r"))) return FALSE;
   if(!(pDST = fopen(tmpfile,"w"))) return FALSE;

   while(stat=fgets(buffer, 250, pSRC)) {

      if(strstr(buffer, dlgname)) {
         fflag=TRUE;
         /* skip dialog */
         while(stat=fgets(buffer, 250, pSRC)) {
            /* check for next dialog */
            if(strstr(buffer, "/*[PDE:")) {
               fputs(buffer, pDST);
               break;
            }
         }
         if(stat) strcpy(buffer," "); /* EOF */
      }

      if(!((strlen(buffer)==blen) && (blen<2))) {
         fputs(buffer, pDST); /* copy to new file */
         blen=strlen(buffer);
      }

   }

   fclose(pSRC);
   fclose(pDST);

   /* delete old */
   remove(sfile);
   rename(tmpfile,sfile);
   return fflag;
}


/* --------------------------------------------------------------------
                            Find dialog in file
   -------------------------------------------------------------------- */

FILE *FindDLG(FILE *pDEF, char *dname)

{
   char *stat;
   char dlgname[80];
   char buffer[255];

   strcpy(dlgname, "/*[PDE:");
   strcat(dlgname, dname);
   strcat(dlgname, "]*/");

   if(strlen(dname)) {
      while(stat=fgets(buffer, 250, pDEF)) {
         if(strstr(buffer, dlgname)) break;
      }
      if(!stat) return NULL;  /* not found */
   }

   if(!strlen(dname)) {  /* when no name is specified */
      while(fgets(buffer, 250, pDEF)) {
         if(stat=strstr(buffer, "/*[PDE:")) break;
      }
      if(!stat) return NULL;  /* nothing found */
   }

   return pDEF;
}


/* --------------------------------------------------------------------
                              Save .C Header
   -------------------------------------------------------------------- */

int SaveHdr(char *cfile, char *hfile)

{
   FILE *pDEF;

   if(!(pDEF=fopen(cfile,"r"))) {

      if(!(pDEF = fopen(cfile,"w"))) return FALSE;
      fprintf(pDEF, "/* DIALOG RESOURCE FILE */\n\n");
      fprintf(pDEF, "#include <stdlib.h>\n\n");
      fprintf(pDEF, "#include \"pal.h\"\n");
      fprintf(pDEF, "#include \"%s\"\n\n", hfile);
      fclose(pDEF);
      return FALSE;     /* new file */
   }
   fclose(pDEF);
   return TRUE;    /* file already exists */
}


/* --------------------------------------------------------------------
                            Save INIT structures
   -------------------------------------------------------------------- */

void SaveInit(DIAGDEF *pDialog, FILE *pDEF)
{
   int f, type;
   char buffer[60];

   for(f=1;f<=pDialog->INum;f++) {
      if(pDialog->item[f]->InitFlag) {
         type = pDialog->item[f]->type;

         fprintf(pDEF, "\n%s ", initypes[type]);
         strcpy(buffer,pDialog->item[f]->name);
         fprintf(pDEF, "%s%s", ininames[type],buffer);
         fprintf(pDEF, " = {\n");

         switch(type) {

            case TEXT:
              fprintf(pDEF, "\"%s\", ", pDialog->item[f]->dflt);
              fprintf(pDEF, "%3d, ", pDialog->item[f]->lenght);
              fprintf(pDEF, "%s\n};\n", fontnames[pDialog->item[f]->font-1]);
              break;

            case GROUP:
            case LABEL:
              fprintf(pDEF, "%s, ", colornames[pDialog->item[f]->color-1]);
              fprintf(pDEF, "%s \n};\n", fontnames[pDialog->item[f]->font-1]);
              break;

            case LISTBOX:
            case BUTTON:
              fprintf(pDEF, "%s\n};\n", fontnames[pDialog->item[f]->font-1]);
              break;

            case OPTION:  /* Radio */
            case CHECK:
              fprintf(pDEF, "%s, ", fontnames[pDialog->item[f]->font-1]);
              if(pDialog->item[f]->ChkFlag)
                 fprintf(pDEF, "1");  /* marked   */
              else
                 fprintf(pDEF, "0");  /* unmarked */
              fprintf(pDEF, "\n};\n");
              break;

            case COMBO:
              fprintf(pDEF, "{\"%s\", ", pDialog->item[f]->dflt);
              fprintf(pDEF, "%3d, ", pDialog->item[f]->lenght);
              fprintf(pDEF, "%s },\n", fontnames[pDialog->item[f]->font-1]);
              fprintf(pDEF, "\"%s\"\n};\n", pDialog->item[f]->List);
              break;

            case BITMAP:
              if(strstr(pDialog->item[f]->TypeName,"NULL"))
                 fprintf(pDEF, "NULL\n};\n");
              else
                 fprintf(pDEF, "(IMGHDR *)%s\n};\n", pDialog->item[f]->TypeName);
              break;

            case OTHER:
              fprintf(pDEF," 0 };\n");
              break;
         }

      fprintf(pDEF,"\n");

      }
   }
}


/* --------------------------------------------------------------------
                            Save ITEM structure
   -------------------------------------------------------------------- */

void SaveItem(DIAGDEF *pDialog, FILE *pDEF)

{
   int f, x, y, w, d, type;
   char buffer[80];

   fprintf(pDEF, "\nDLGITEM %sItems[] = {\n",pDialog->name);
   fprintf(pDEF, "/*X   Y   W   D   FLAGS     LABEL       INIT       TYPE");
   fprintf(pDEF, "           SIZE        PRIV */\n");

   for(f=1;f<=pDialog->INum;f++) {

      x=(pDialog->item[f]->x1)-(pDialog->x1)-HOFFSET_ADJ; /* horiz offset */
      y=(pDialog->item[f]->y1)-(pDialog->y1)-VOFFSET_ADJ; /* vert offset  */
      w=(pDialog->item[f]->x2)-(pDialog->item[f]->x1);      /* width */
      d=(pDialog->item[f]->y2)-(pDialog->item[f]->y1);      /* depth */
      type = pDialog->item[f]->type;

      /* Checkbox is always square */
      if(type==CHECK) {
         w=15; d=15;
      }

      fprintf(pDEF, "{%3d,%3d,%3d,%3d,", x, y, w, d);

      fprintf(pDEF, " %4s, \"", pDialog->item[f]->flags);

      /* LABEL OUTPUT */
      strcpy(buffer,pDialog->item[f]->label);
      strcat(buffer,"\", ");
      fprintf(pDEF, "%-12s", buffer);

      /* INIT OUTPUT */
      if(pDialog->item[f]->InitFlag) {

         strcpy(buffer,ininames[pDialog->item[f]->type]);
         strcat(buffer,pDialog->item[f]->name);
         strrev(buffer);
         strcat(buffer,"&");
         strrev(buffer);
         strcat(buffer,",");
         fprintf(pDEF, "%-12s", buffer);

      }
      else
         fprintf(pDEF, "NULL,       ");

      /* TYPE OUTPUT */
      strcpy(buffer, types[pDialog->item[f]->type]);

      /* Is it a User Defined item ? */
      if(pDialog->item[f]->type==10) strcpy(buffer, pDialog->item[f]->TypeName);
      fprintf(pDEF, "%-8s", buffer);

      /* SIZE OUTPUT */
      fprintf(pDEF, ", sizeof(");
      fprintf(pDEF, "%-8s), NULL }", strupr(buffer));

      if(f<pDialog->INum) fputs(",",pDEF);
      fputs("\n",pDEF);
   }

   fprintf(pDEF, "};\n\n");
}


/* --------------------------------------------------------------------
                              Load Header file
   -------------------------------------------------------------------- */

int GetDlgHdr(char *dname, char *hfile, DIAGDEF *pLoad)

{
   FILE *pDEF;
   char buffer[255];

   if(!(pDEF = fopen(hfile,"r"))) return FALSE;
   if(!(pDEF=FindDLG(pDEF, dname))) return FALSE;

   pLoad->INum = 0;
   while(fgets(buffer, 250, pDEF)) {

      if(strstr(buffer,"/*[PDE:")) break; /* next dialog? */

      if(strstr(buffer, "#define")) {
         /* allocate storage for item */
         pLoad->INum++;
         free(pLoad->item[pLoad->INum]);
         pLoad->item[pLoad->INum] = NULL;
         pLoad->item[pLoad->INum] = malloc(sizeof(FIELDEF));
         sscanf(buffer, "%*s %s", pLoad->item[pLoad->INum]->name);

         /* set defaults */
         pLoad->item[pLoad->INum]->InitFlag = 0;
         pLoad->item[pLoad->INum]->color = 1;
         pLoad->item[pLoad->INum]->font = 2;
         pLoad->item[pLoad->INum]->lenght = 20;
         pLoad->item[pLoad->INum]->TypeName[0] = 0;
         pLoad->item[pLoad->INum]->dflt[0] = 0;
         pLoad->item[pLoad->INum]->List[0] = 0;
      }
   }

   fclose(pDEF);
   if(pLoad->INum == 0) return FALSE;
   return TRUE;
}


/* --------------------------------------------------------------------
                              Load dialog file
   -------------------------------------------------------------------- */

int GetDlgSrc(char *dname, char *cfile, DIAGDEF *pLoad)
{
   FILE *pDEF;
   char *stat;
   int  f, indx, xpos, ypos, width, depth;
   char buffer[310];
   char StructDef[50];
   char StructName[50];
   char bFont[20];
   char bColor[20];
   char bType[20];

   if(!(pDEF = fopen(cfile,"r"))) return FALSE;

   /* find dialog */
   if(!(pDEF=FindDLG(pDEF, dname))) return FALSE;

   /* Get dialog window dimensions */
   do {
      stat = fgets(buffer, 250, pDEF);
      sscanf(buffer, "%s", StructDef);
      if(strstr(StructDef,"DIALOG")) {
         fgets(buffer, 250, pDEF);
         sscanf(buffer, "%d , %d ,", &pLoad->x2, &pLoad->y2);
         break;
      }

      if(strstr(buffer,"/*[PDE:")) break;  /* next dialog? */
   }while(stat);

   /* back to the beginning */
   fseek(pDEF, 0L, SEEK_SET);

   /* find dialog */
   if(!(pDEF=FindDLG(pDEF, dname))) return FALSE;

   StructDef[0]=0; /* clear string */

   do {
      stat = fgets(buffer, 250, pDEF);
      sscanf(buffer, "%s %s", StructDef, StructName);
      if(strstr(buffer,"/*[PDE:")) break; /* next dialog? */

      for(f=1;f<NUM_OF_TYPES;f++) {
         if(strstr(StructDef, initypes[f])) {

            /* remove 2 char header from name */
            strrev(StructName);
            StructName[strlen(StructName)-2] = 0;
            strrev(StructName);

            /* find corresponding item slot */
            for(indx=1;indx < pLoad->INum+1;indx++) {

               if(indx > pLoad->INum) {
                  fclose(pDEF);
                  return FALSE;
               }
               if(!strcmpi(pLoad->item[indx]->name, StructName)) break;
            }


            /* INIT structure data */

            switch(f) {

               case TEXT:
                  pLoad->item[indx]->InitFlag = 1;
                  fgets(buffer, 300, pDEF);
                  sscanf(buffer, "\"%[^\"]\" , %d , %10[^\0]",
                     &pLoad->item[indx]->dflt,
                     &pLoad->item[indx]->lenght,
                     &bFont);
                  if(strstr(bFont,"SMALL_FONT")) pLoad->item[indx]->font=1;
                  if(strstr(bFont,"MEDIUM_FON")) pLoad->item[indx]->font=2;
                  if(strstr(bFont,"LARGE_FONT")) pLoad->item[indx]->font=3;
                  break;

               case GROUP:
               case LABEL:
                  pLoad->item[indx]->InitFlag = 1;
                  fgets(buffer, 300, pDEF);
                  sscanf(buffer, "%11[^,], %10[^\0]", &bColor, &bFont);
                  if(strstr(bColor,"BLACK_COLOR")) pLoad->item[indx]->color=1;
                  if(strstr(bColor,"WHITE_COLOR")) pLoad->item[indx]->color=2;
                  if(strstr(bFont,"SMALL_FONT")) pLoad->item[indx]->font=1;
                  if(strstr(bFont,"MEDIUM_FON")) pLoad->item[indx]->font=2;
                  if(strstr(bFont,"LARGE_FONT")) pLoad->item[indx]->font=3;
                  break;

               case COMBO:
                  pLoad->item[indx]->InitFlag = 1;
                  fgets(buffer, 300, pDEF);
                  sscanf(buffer, "{\"%[^\"]\" , %d , %10[^\0]",
                     &pLoad->item[indx]->dflt,
                     &pLoad->item[indx]->lenght,
                     &bFont);
                  if(strstr(bFont,"SMALL_FONT")) pLoad->item[indx]->font=1;
                  if(strstr(bFont,"MEDIUM_FON")) pLoad->item[indx]->font=2;
                  if(strstr(bFont,"LARGE_FONT")) pLoad->item[indx]->font=3;

                  fgets(buffer, 300, pDEF);
                  sscanf(buffer, "\"%[^\"]", &pLoad->item[indx]->List);
                  break;

               case LISTBOX:
               case BUTTON:
                  pLoad->item[indx]->InitFlag = 1;
                  fgets(buffer, 300, pDEF);
                  sscanf(buffer, "%10s", &bFont);
                  if(strstr(bFont,"SMALL_FONT")) pLoad->item[indx]->font=1;
                  if(strstr(bFont,"MEDIUM_FON")) pLoad->item[indx]->font=2;
                  if(strstr(bFont,"LARGE_FONT")) pLoad->item[indx]->font=3;
                  break;

               case OPTION:
               case CHECK:
                  pLoad->item[indx]->InitFlag = 1;
                  fgets(buffer, 300, pDEF);
                  sscanf(buffer, "%[^,], %d", &bFont, &pLoad->item[indx]->ChkFlag);
                  if(strstr(bFont,"SMALL_FONT")) pLoad->item[indx]->font=1;
                  if(strstr(bFont,"MEDIUM_FON")) pLoad->item[indx]->font=2;
                  if(strstr(bFont,"LARGE_FONT")) pLoad->item[indx]->font=3;
                  break;

               case BITMAP:
                  pLoad->item[indx]->InitFlag = 1;
                  fgets(buffer, 300, pDEF);
                  sscanf(buffer, "(IMGHDR *)%40s", &pLoad->item[indx]->TypeName);
                  if(strlen(pLoad->item[indx]->TypeName)<2)
                     strcpy(pLoad->item[indx]->TypeName, "NULL");
                  break;

               default:
                  break;
            }
         }
      }



      /* DLGITEM structure data */
      if(strstr(StructDef,"DLGITEM"))  {
         pLoad->DiagFlag = 1;

         /* bypass remark line */
         fgets(buffer, 150, pDEF);

         /* get all item values */
         for(indx=1;indx < pLoad->INum+1;indx++) {
            fgets(buffer, 300, pDEF);
            sscanf(buffer, "{%d , %d , %d , %d , %[^,], \"%[^\"]\" ,%*[^,], %15[^,]",
               &xpos, &ypos, &width, &depth,
               &pLoad->item[indx]->flags,
               &pLoad->item[indx]->label,
               &bType);

            pLoad->item[indx]->x1 = xpos + ((640 - pLoad->x2)/2) + HOFFSET_ADJ;
            pLoad->item[indx]->y1 = ypos + ((200 - pLoad->y2)/2) + VOFFSET_ADJ;
            pLoad->item[indx]->x2 = pLoad->item[indx]->x1 + width;
            pLoad->item[indx]->y2 = pLoad->item[indx]->y1 + depth;

            /* set item type */
            for(f=1;f<NUM_OF_TYPES;f++) {
               if(strstr(bType, types[f])) {
                  pLoad->item[indx]->type = f;
                  break;
               }
            }

            if(f==NUM_OF_TYPES)  { /* set to "other" if not found */
               pLoad->item[indx]->type = NUM_OF_TYPES-1;
               strcpy(pLoad->item[indx]->TypeName,bType);
            }
         }
      }


      /* DIALOG structure data */
      if(strstr(StructDef,"DIALOG"))   {
         /* Dialog name */
         strcpy(pLoad->name, StructName);

         /* bypass Dialog window dimensions */
         fgets(buffer, 100, pDEF);

         /* default center position */
         pLoad->y1 = (200 - pLoad->y2)/ 2;
         pLoad->x1 = (640 - pLoad->x2)/ 2;
         pLoad->x2 += pLoad->x1;
         pLoad->y2 += pLoad->y1;

         /* Dialog Flags */
         fgets(buffer, 30, pDEF);
         sscanf(buffer, "%[^,],", &pLoad->flags);

         /* Dialog Focus */
         fgets(buffer, 30, pDEF);
         sscanf(buffer, "%d ,", &pLoad->focus);

         strcpy(pLoad->handler, "DhStandard");
         fclose(pDEF);
         return TRUE;
      }
   } while(stat);

   fclose(pDEF);

   return TRUE;
}


/* --------------------------------------------------------------------
                              Load Dialog
   -------------------------------------------------------------------- */

int LoadDlg(DIAGDEF *pDialog, char *file, char *name)

{
   PALWIN *pWin;

   char dname[80];
   char fname[80];
   char cfile[100];
   char hfile[100];
   char msg[100];

   strcpy(dname,name);
   strcpy(fname,file);
   strcpy(cfile, fname);
   strcpy(hfile, fname);

   /* name it if loaded from command line */
   strcpy(pDialog->fname, (void *)fname);

   AddExt(cfile, fname, ".c");
   AddExt(hfile, fname, ".h");

   /* pick list if not specified */
   if(strlen(dname)<2) PickDlg(hfile, dname);

   strcpy(msg, "Loading dialog ");
   strcat(msg, dname);
   strcat(msg, "...");
   pWin = OpenWin(WS_HP200, 140, 50, 470, 110, "Attention.");
   WriteText(180,80, msg);

   if(!GetDlgHdr(dname, hfile, pDialog)) {
      CloseWin(pWin);
      return FALSE;
   }

   if(!GetDlgSrc(dname, cfile, pDialog)) {
      CloseWin(pWin);
      return FALSE;
   }

   CloseWin(pWin);

   ShowFKeys(KeyLabels2);
   DisplayDlg(pDialog);
   DispInfo(pDialog,0,0,0,0,0);
   return TRUE;
}


/* --------------------------------------------------------------------
                              Save .C Resource
   -------------------------------------------------------------------- */

void SaveC(DIAGDEF *pDialog, char *cfile, char *hfile)
{

   FILE *pDEF;
   int w, d;

   /* write header if new */
   SaveHdr(cfile, hfile);

   /* delete old dialog in resource */
   DeleteDlg(cfile, pDialog->name);

   if(!(pDEF=fopen(cfile,"a"))) return;

   fprintf(pDEF, "/*[PDE:%s]*/\n", pDialog->name);

   /* INIT STRUCTURES */
   SaveInit(pDialog, pDEF);

   /* ITEM STRUCTURES */
   SaveItem(pDialog, pDEF);


/* DIALOG DEFINITION */

   fprintf(pDEF, "DIALOG %s = {\n", pDialog->name);

   w=((pDialog->x2)-(pDialog->x1));      /* width */
   d=((pDialog->y2)-(pDialog->y1));      /* depth */
   fprintf(pDEF, "%d, %d,\n", w, d);

   fputs(pDialog->flags, pDEF);
   fputs(",\n",pDEF);

   fprintf(pDEF, "%d,\n%d,\n", pDialog->focus, pDialog->INum);
   fprintf(pDEF, "%sItems,\n%s,\nNULL,\nNULL,\n0\n};\n\n",
           pDialog->name, pDialog->handler);

   fclose(pDEF);
}


/* --------------------------------------------------------------------
                              Save .H Resource
   -------------------------------------------------------------------- */

void SaveH(DIAGDEF *pDialog, char *hfile)
{

   int f;
   FILE *pDEF;

   /* create if it doesn't exist */
   if(!(pDEF=fopen(hfile,"r"))) {
      if(!(pDEF = fopen(hfile,"w"))) return;
      fprintf(pDEF, "/* DIALOG RESOURCE HEADER FILE */\n\n");
      fclose(pDEF);
   }

   /* delete old dialog from file */
   DeleteDlg(hfile, pDialog->name);

   if(!(pDEF = fopen(hfile,"a"))) return;

   fprintf(pDEF, "/*[PDE:%s]*/\n\n", pDialog->name);

   fprintf(pDEF, "extern DLGITEM %sItems[];\n", pDialog->name);
   fprintf(pDEF, "extern DIALOG %s;\n\n", pDialog->name);

   for(f=1;f<=pDialog->INum;f++) {
      if((pDialog->item[f]->type==BITMAP)
      &&(!strstr(pDialog->item[f]->TypeName, "NULL")))
         fprintf(pDEF, "extern unsigned char %s[];\n", pDialog->item[f]->TypeName);

      fprintf(pDEF, "#define %s %d\n", pDialog->item[f]->name, (f-1));
   }

   fputs("\n",pDEF);
   fclose(pDEF);
}


/* --------------------------------------------------------------------
                              Save TEST file
   -------------------------------------------------------------------- */

void SaveTest(char *cfile, char *hfile)
{

   unsigned int dNum=0;
   unsigned int MaxD=0;
   FILE *pDEF;
   FILE *pHDR;
   char dname[80];
   char buffer[120];

   if(!(pDEF = fopen("testdlg.c","w"))) return;
   if(!(pHDR = fopen(hfile,"r"))) return;

   fprintf(pDEF, "/* Test file for %s resource. */\n", cfile);
   fprintf(pDEF, "/* To compile: PM ftestdlg */\n\n");

   fprintf(pDEF, "#include \"pal.h\"\n");
   fprintf(pDEF, "#include \"%s\"\n\n", hfile);
   fprintf(pDEF, "DIALOG *DlgArray[] = {\n");

   /* create dialog name array */
   while(fgets(buffer, 100, pHDR)) {
     if(strstr(buffer, "/*[PDE:")) {
        sscanf(buffer, "/*[PDE:%[^]]", dname);
        fprintf(pDEF, "&%s,\n", dname);
     }
   }

   fprintf(pDEF, "};\n\n");

   /* back to the beginning */
   fseek(pHDR, 0L, SEEK_SET);

   fprintf(pDEF, "MENUITEM DlgMnuItems[] = {\n");
   while(fgets(buffer, 100, pHDR)) {
     if(strstr(buffer, "/*[PDE:")) {
        sscanf(buffer, "/*[PDE:%[^]]", dname);
        fprintf(pDEF, "{ \"%s\", MA_VAL, MENUVAL(%d) },\n", dname, dNum);
        dNum++;
     }
   }
   fprintf(pDEF, "};\n\n");
   fclose(pHDR);

   if(dNum<4) MaxD=dNum;
   else MaxD=4;

   fprintf(pDEF, "MENU DlgMnu = { 0, MS_PULLDN, 0, 0, %d, ", MaxD);
   fprintf(pDEF, "%d, DlgMnuItems };\n\n", dNum );

   fprintf(pDEF, "void main(void)\n{\n");
   fprintf(pDEF, "   int n=0, Xpos, Ypos;\n");
   fprintf(pDEF, "   if(!PalInit(1)) FatalExit(\"Init failed");
   fprintf(pDEF, " - CGAGRAPH not loaded ?\", 1);\n\n");

   if(dNum>1) {
      fprintf(pDEF, "   SelectFont(MEDIUM_FONT);\n");
      fprintf(pDEF, "   WriteText(150,60,\"Please select the dialog to test.\");\n\n");
      fprintf(pDEF, "   do {\n");
      fprintf(pDEF, "      n = HandleMenu(&DlgMnu,240, 80);\n");
      fprintf(pDEF, "      if(n >= 0) {\n");
   }

   fprintf(pDEF, "         InitDialog(DlgArray[n]);\n");
   fprintf(pDEF, "         Xpos = (640 - DlgArray[n]->Width) /2;\n");
   fprintf(pDEF, "         Ypos = (200 - DlgArray[n]->Depth) /2;\n");
   fprintf(pDEF, "         ShowDialog(DlgArray[n], Xpos, Ypos, DlgMnuItems[n].Text);\n");
   fprintf(pDEF, "         HandleDialog(DlgArray[n], 0);\n");
   fprintf(pDEF, "         CloseDialog(DlgArray[n]);\n");

   if(dNum>1)
      fprintf(pDEF, "      }\n   } while(n>=0);\n");

   fprintf(pDEF, "\n   PalDeInit(1);\n}\n");
   fclose(pDEF);

   /* SRCLIST for TESTDLG file */
   if(!(pDEF = fopen("testdlg","w"))) return;
   fprintf(pDEF, "testdlg.c\n");
   fprintf(pDEF, "%s\n", cfile);
   fclose(pDEF);

}

