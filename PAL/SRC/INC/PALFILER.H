/* --------------------------------------------------------------------
   Project: HP200LX FILER PROTOCOL (CLIENT) COMMUNICATIONS FOR PAL
   Module:  FILER.H
   Author:  Harry Konstas
   Started: 17. Oct. 95
   Subject: Filer communications header file.
   -------------------------------------------------------------------- */

#ifndef _FILER_H
#define _FILER_H

#include <stddef.h>

/* --------------------------------------------------------------------
                            definitions
   -------------------------------------------------------------------- */

/* debug switches */
/* #define DEBUG_FILER */
/* #define DEBUG_PORT  */

/* timings */
#define NUM_OF_ATTEMPTS 3
#define TIMEOUT_DELAY 3000

/* protocol definitions */
#define SOH 1
#define PACKET_DATA_SIZE 0x800

#define CONNECT_SERVER 0x40
#define DISCONNECT_SERVER 0x41
#define SEND_PATH 0x08
#define SEND_FILENAME 0x03
#define GET_FILENAME 0xB
#define SEND_DATA 0x15
#define INIT_GET 0x16
#define GET_DATA 0xC
#define DATA_END 0x1
#define DEL_FILE 0x4
#define MAKE_DIR 0xA
#define DEL_DIR 0xE
#define ASK_DIR 0x12
#define GET_DIR 0x9


/* --------------------------------------------------------------------
                          type definitions
   -------------------------------------------------------------------- */

/* communications messages */

typedef enum {

   PACKET_SEND=1,       /* packet was sent successfuly          */
   PACKET_RECVD,        /* packet received successfully         */
   CONNECTED,           /* Server connection established        */
   SERVER_CLOSED,       /* Server connection is broken          */
   SERVER_ACK,          /* Server acknowledged Request          */

   FILE_SEND_OK,        /* File was sent to Server OK           */
   GOT_FILE_OK,         /* File received from Server OK         */
   FILE_DELETED,        /* File in Server was deleted OK        */
   DIR_CREATED,         /* Client created directory on server   */
   DIR_DELETED,         /* Client deleted directory on server   */
   DIR_FOUND,           /* Client found remote directory        */
   GOT_DIR_ENTRY,       /* Client got remote directory entry    */

   TIMEOUT,             /* Serial port Timeout occured          */
   INV_BAUDRATE,        /* Invalid baud rate                    */
   INVALID_FUNCTION,    /* Packet has invalid function          */
   NO_RESPONSE,         /* Server is not responding             */
   PACKET_TOO_LARGE,    /* Received packet truncated, too large */
   BAD_CRC,             /* Received packet has bad crc          */
   BAD_PACKET,          /* Truncated/invalid packet received    */
   BAD_REQUEST,         /* Invalid Request from Client          */
   BAD_PACKET_COUNT,    /* Packet counter out of sync           */

   NO_SOURCE_FILE,      /* Cannot open Client source file       */
   CANNOT_SEND_FNAME,   /* Client can't send filename to Server */
   DISK_FULL,           /* Server disk is full                  */
   CANNOT_DELETE,       /* Client cannot delete Server file     */
   CANNOT_CREATE,       /* Client cannot open file for Server   */
   CANNOT_INITIATE,     /* Client cannot initiate transfer      */
   CANNOT_CREATE_DIR,   /* Client cannot create remote directory*/
   CANNOT_DELETE_DIR,   /* Client cannot delete remote directory*/
   CANNOT_FIND_DIR,     /* Client cannot find remote directory  */
   CANNOT_GET_ENTRY     /* Client can't get next directory entry*/

} COM_MESSAGE;

/* Filer Callback functions */

typedef struct {
   void   *(*FlcbSendOpen)(char *FileName);
   size_t  (*FlcbSendBlock)(void *Buf, size_t Size, void *Handle);
   void    (*FlcbSendClose)(void *Handle);

   void   *(*FlcbRecvOpen)(char *FileName);
   size_t  (*FlcbRecvBlock)(void *Buf, size_t Size, void *Handle);
   void    (*FlcbRecvClose)(void *Handle);
} FLCB;
extern FLCB FlCb;

/* file date-stamp structure */

typedef struct {
   unsigned sec:5;
   unsigned min:6;
   unsigned hour:5;
   unsigned day:5;
   unsigned month:4;
   unsigned year:7;
} FDATESTAMP;

/* Filer Communications structure */

typedef struct {
   WORD Port;                 /* Serial port address   */
   unsigned long Baud;        /* Baud-rate             */
   BYTE Name[16];             /* Entry-name (file/dir) */
   BYTE Attribute;            /* File attribute        */
   unsigned long FileSize;    /* File size             */
   FDATESTAMP DateStamp;      /* File date stamp       */
   BYTE Function;             /* Packet function       */
   BYTE Count;                /* packet count ID       */
   BYTE Status;               /* packet Status/ACK     */
   WORD Size;                 /* data lenght           */
   BYTE *pData;               /* pointer to data       */
   WORD CRC16;                /* 16 bit CRC checksum   */
   FLCB *pCb;                 /* pointer to structure with callbacks */
} FILERCOM;


/* --------------------------------------------------------------------
                             prototypes
   -------------------------------------------------------------------- */

/* protocol communications functions */

FILERCOM *FilerConnect(int PortNumber, unsigned long BaudRate, FLCB *pCb);
int  FilerDisconnect(FILERCOM *pFiler);
int  FilerRequest(FILERCOM *pFiler, int Request,WORD size, BYTE *pData);
int  FilerSync(FILERCOM *pFiler);

/* filer transfer operations */

int  FilerSendFile(FILERCOM *pFiler, char *LocalFile, char *RemoteFile);
int  FilerGetFile(FILERCOM *pFiler, char *RemoteFile, char *LocalFile);
int  FilerDelFile(FILERCOM *pFiler, char *RemoteFile);
int  FilerMakeDir(FILERCOM *pFiler, char *RemoteDir);
int  FilerDelDir(FILERCOM *pFiler, char *RemoteDir);
int  FilerAskDir(FILERCOM *pFiler, char *RemoteDir);
int  FilerGetDir(FILERCOM *pFiler);

/* internal communications functions */

void SetBaudRate(unsigned int port, unsigned long baud);
int  GetByte(WORD port, BYTE *value, WORD *Checksum);
WORD SendByte(WORD port, BYTE value, WORD Checksum);
WORD UpdateCRC16(WORD CRC, BYTE data);
int  SendPacket(FILERCOM *pPacket, int function, WORD count, WORD size, BYTE *pData);
int  GetPacket(FILERCOM *pPacket);

#endif

