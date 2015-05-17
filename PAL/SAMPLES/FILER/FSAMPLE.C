/* --------------------------------------------------------------------
   Project: HP200LX FILER PROTOCOL (CLIENT) COMMUNICATIONS FOR PAL
   Module:  FSAMPLE.C
   Author:  Harry Konstas / Gilles Kohl
   Started: 17. Oct. 95
   Subject: Filer communications sample
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                             includes
   -------------------------------------------------------------------- */
#include <dos.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

/* --------------------------------------------------------------------
                         local includes
   -------------------------------------------------------------------- */

#include "pal.h"


size_t  MySendBlock(void *Buf, size_t Size, void *Handle);
size_t  MyRecvBlock(void *Buf, size_t Size, void *Handle);

/* --------------------------------------------------------------------
                        User defined GET/SEND blocks
   -------------------------------------------------------------------- */

FLCB MyFlCb;

/* User defined Send-block routine, replaces default FlCb */

size_t  MySendBlock(void *Buf, size_t Size, void *Handle)
{
   putchar('.');
   return fread(Buf, 1, Size, Handle);
}

/* User defined Get-block routine, replaces default FlCb */

size_t  MyRecvBlock(void *Buf, size_t Size, void *Handle)
{
   putchar('.');
   return fwrite(Buf, 1, Size, Handle);
}


/* --------------------------------------------------------------------
                               M A I N
   -------------------------------------------------------------------- */

int main(int argc, char *argv[])
{
   int   stat, port;
   FILERCOM *pFiler;
   FLCB MyFlCb;        /* our own callback get/send routines */

   if(argc != 2) {
      fprintf(stderr, "Usage: fsample <Port Number>\n");
      exit(-1);
   }

   port = atoi(argv[1]);
   if(port < 1 || port > 4) {
      fprintf(stderr, "Invalid port #, must be between 1 and 4\n");
      exit(-1);
   }

   /* replace default (PAL) FlCb handler by one of our own */
   MyFlCb = FlCb;
   MyFlCb.FlcbSendBlock = MySendBlock;
   MyFlCb.FlcbRecvBlock = MyRecvBlock;


   /* attempt AUTOBAUD connection -------------------------------------------*/
   printf("\nDetermining baud-rate and attempting connection, please wait...");

   /* If given baudrate==0, FilerConnect determines the baudrate */
   if(!(pFiler = FilerConnect(port,0, &MyFlCb))) {
      printf("\nUnable to connect!\n");
      exit(-1);
   }

   /* 1. CREATE A TEST DIRECTORY -------------------------------------------*/
   printf("\nCreating a 'FLRTEST' directory on drive C: of the palmtop...");

   stat = FilerMakeDir(pFiler, "c:\\flrtest");

   if(stat==CANNOT_CREATE_DIR) printf("\nCannot create remote directory.\n");
   if(stat==DIR_CREATED) printf("\nRemote directory created.\n");


   /* 2. TRANSFER FSAMPLE.C into remote FLRTEST directory ------------------*/
   printf("\n\nSending file 'fsample.exe' to FLRTEST directory...");

   stat = FilerSendFile(pFiler, "fsample.exe", "c:\\flrtest\\fsample.exe");

   if(stat== CANNOT_SEND_FNAME) printf("\nCan't send file. Already exists?\n");
   if(stat== DISK_FULL) printf("\nRemote Disk is full.\n");
   if(stat== NO_RESPONSE) printf("\nServer Not responding.\n");
   if(stat== FILE_SEND_OK) printf("\n File was sent ok.\n");


   /* 3. TRANSFER FSAMPLE.C into remote FLRTEST directory as FSAMPLE2.C ----*/
   printf("\n\nSending file 'fsample2.c' to FLRTEST directory...");

   stat = FilerSendFile(pFiler, "fsample.exe", "c:\\flrtest\\fsample2.c");

   if(stat== CANNOT_SEND_FNAME) printf("\nCan't send file. Already exists?\n");
   if(stat== DISK_FULL) printf("\nRemote Disk is full.\n");
   if(stat== NO_RESPONSE) printf("\nServer Not responding.\n");
   if(stat== FILE_SEND_OK) printf("\n File was sent ok.\n");


   /* 4. SHOW REMOTE DIRECTORY (FLRTEST) INFO -----------------------------*/
   printf("\n\nLogging into c:\\flrtest directory...\n\n");

   /* display directory for "*.*"      */
   stat = FilerAskDir(pFiler, "c:\\flrtest\\*.*");

   if(stat== NO_RESPONSE) printf("\nServer Not responding.\n");

   /* get & display individual directory entries (FindNext) */
   for(;;) {
      if(FilerGetDir(pFiler) == CANNOT_GET_ENTRY) break;
      printf("%-12s %12lu  %02d-%02d-%02d  %02d:%02d  ATTRIB=0x%2X\n",
               pFiler->Name, pFiler->FileSize,
               pFiler->DateStamp.month, pFiler->DateStamp.day,
               pFiler->DateStamp.year+80,
               pFiler->DateStamp.hour, pFiler->DateStamp.min,
               pFiler->Attribute);
   }

   /* 5. GET FSAMPLE.C back to Local as 'FEEDBACK.C' -----------------------*/
   printf("\n\nGetting back 'fsample.exe' to desktop as 'FEEDBACK'..");

   stat = FilerGetFile(pFiler, "c:\\flrtest\\fsample.exe", "feedback");

   if(stat== CANNOT_SEND_FNAME) printf("\nCan't get file.\n");
   if(stat== NO_RESPONSE) printf("\nServer Not responding.\n");
   if(stat== GOT_FILE_OK) printf("\n File was received ok.\n");


   /* 6. DELETE THE FSAMPLE.C FILE -----------------------------------------*/
   printf("\nDeleting the 'fsample.exe' file from the palmtop...");

   stat = FilerDelFile(pFiler, "c:\\flrtest\\fsample.exe");

   if(stat==CANNOT_DELETE) printf("\nCannot delete the file!\n");
   if(stat==FILE_DELETED) printf("\nfsample.exe is wiped out.\n");


   /* 7. DELETE THE FSAMPLE2.C FILE ----------------------------------------*/
   printf("\nDeleting the 'fsample2.c' file from the palmtop...");

   stat = FilerDelFile(pFiler, "c:\\flrtest\\fsample2.c");

   if(stat==CANNOT_DELETE) printf("\nCannot delete the file!\n");
   if(stat==FILE_DELETED) printf("\nfsample2.c file is wiped out.\n");


   /* 8. DELETE THE FLRTEST DIRECTORY --------------------------------------*/
   printf("\nDeleting the FLRTEST directory from the palmtop...");

   stat = FilerDelDir(pFiler, "c:\\flrtest");

   if(stat==CANNOT_DELETE_DIR) printf("\nCannot delete remote directory.\n");
   if(stat==DIR_DELETED) printf("\nRemote directory vanished.\n");


   /* CLOSE DOWN FILER COMMUNICATIONS ------------------------------------- */
   FilerDisconnect(pFiler);

}

