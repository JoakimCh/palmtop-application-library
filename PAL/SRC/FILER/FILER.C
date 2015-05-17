/* --------------------------------------------------------------------
   Project: HP200LX FILER PROTOCOL (CLIENT) COMMUNICATIONS FOR PAL
   Module:  FILER.C
   Author:  Harry Konstas
   Started: 17. Oct. 95
   Subject: Filer communications module
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                       standard includes
   -------------------------------------------------------------------- */

#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

/* --------------------------------------------------------------------
                         local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"

/* --------------------------------------------------------------------
                           Set Baudrate
   -------------------------------------------------------------------- */

void SetBaudRate(unsigned int port, unsigned long baud) {

   unsigned char dbits, sbits, parity, lcr;

   /* set default values */
   dbits = 8; sbits = 2; parity = 0;

   /* baud-rate setup */
   outportb(port+3,0x80);

   /* compute baud-rate divisor */
   baud=(int)(115200L/(long)baud);

   outportb(port+0,baud & 0xff); /* set MSB of baud rate  */
   outportb(port+1,baud >> 8);   /* set LSB of baud rate  */

   /* data, parity & stop bits */
   lcr=(dbits-5) | (((sbits-1) & 1)<<2) | ((parity & 7)<<3);
   outportb(port+3,lcr & 0x3f);

}

/* -------------------------------------------------------------------
                        Update CRC16 (Bisync) checksum
   -------------------------------------------------------------------- */

WORD UpdateCRC16(WORD CRC, BYTE data)
{
   WORD i, temp_crc, polynomial = 0xa001;

   temp_crc = CRC;
   temp_crc ^= data;

   for (i=0; i<8; i++)
      temp_crc = (temp_crc >> 1) ^ ((temp_crc & 1) ? polynomial : 0);

   return temp_crc;
}


/* --------------------------------------------------------------------
          Get RS232 byte, update CRC and return status
   -------------------------------------------------------------------- */

int GetByte(WORD port, BYTE *value, WORD *Checksum)
{
   WORD  far *volatile ClockLoc  = (WORD far *volatile)0x0040006cL;
   DWORD start, end, current;

   /* start counting time */
   start = *ClockLoc;
   end = (DWORD)start + TIMEOUT_DELAY / 55;

   while(!(inportb(port+5) & 0x01)) {
      current = *ClockLoc;
      if(current < start) current += 65536L;

      if(current >= end) {
         return TIMEOUT;           /* timeout occured */
      }
   }

   *value = inportb(port);

   /* '0x10' CRC trick, receive it twice */
   if( ((*value)==0x10) && (Checksum!=NULL) ) {
      while(!(inportb(port+5) & 0x01));
      inportb(port);
   }

   /* update & return checksum */
   if(Checksum)
      *Checksum = UpdateCRC16(*Checksum, *value);

#ifdef DEBUG_PORT
   printf("r%X ", *value);
#endif

   return 0;

}


/* --------------------------------------------------------------------
                    Send RS232 byte / update CRC
   -------------------------------------------------------------------- */

WORD SendByte(WORD port, BYTE value, WORD Checksum)
{
   /* wait till buffer is empty */
   while(!(inportb(port+5) & 0x20));

   /* now output our byte */
   outportb(port,value);

   /* '0x10' CRC trick! got to send it twice */
   if( (Checksum!=0) && (value == 0x10) ) {
      while(!(inportb(port+5) & 0x20));
      outportb(port,0x10);
   }

#ifdef DEBUG_PORT
   printf("s%X ", value);
#endif

   /* update & return checksum */
   return (UpdateCRC16(Checksum, value));
}


/* --------------------------------------------------------------------
                   Send packet to server, return status
   -------------------------------------------------------------------- */

int  SendPacket(FILERCOM *pPacket, int function, WORD count, WORD size, BYTE *pData)
{
   WORD f, port, Checksum = 0;
   BYTE Signature[] = { 0x16, 0x16, 0x16, 0x10, 0x02 }; /* packet signature */

   if(size > PACKET_DATA_SIZE) return PACKET_TOO_LARGE;

   port = pPacket->Port;

#ifdef DEBUG_FILER
   printf("\n\nSENDING PACKET# : 0x%X\n", count);
   printf("SENDING function: 0x%X\n", function);
#endif

   /* send signature */
   for(f=0;f<5;f++) {
      SendByte(port, Signature[f], 0);  /* no CRC yet */
   }

   /* send SOH (start of header) */
   Checksum = SendByte(port, SOH, Checksum);  /* CRC starts here */

   /* send function (command request) */
   Checksum = SendByte(port, function, Checksum);

   /* send packet count */
   Checksum = SendByte(port, count, Checksum);

   /* send DM  (data marker) */
   Checksum = SendByte(port, 0x01, Checksum);
   Checksum = SendByte(port, 0x02, Checksum);

   /* process function request */
   switch(function) {

      case GET_DIR:
      case CONNECT_SERVER:
      case DISCONNECT_SERVER:
      case INIT_GET:
         break;  /* nothing else to say */

      case MAKE_DIR:
      case DEL_DIR:
      case DEL_FILE:
      case SEND_PATH:
      case SEND_FILENAME:
      case GET_FILENAME:
      case ASK_DIR:
         /* send path/filename size LO, HI */
         Checksum = SendByte(port, size & 0xff, Checksum);
         Checksum = SendByte(port, size >>0x08, Checksum);

         /* send path/filename */
         for(f=0;f<size;f++)
            Checksum = SendByte(port, pData[f], Checksum);

         /* send Data End Marker */
         Checksum = SendByte(port, 0x00, Checksum);
         Checksum = SendByte(port, 0x00, Checksum);
         break;

      case SEND_DATA:
         /* double word size, MSB always zero */
         Checksum = SendByte(port, 0x00, Checksum);
         Checksum = SendByte(port, 0x00, Checksum);
         Checksum = SendByte(port, size & 0xff, Checksum);
         Checksum = SendByte(port, size >>0x08, Checksum);

         /* send data */
         for(f=0;f<size;f++)
            Checksum = SendByte(port, pData[f], Checksum);

         break;

      case GET_DATA:
         /* double word size, MSB always zero */
         Checksum = SendByte(port, 0x00, Checksum);
         Checksum = SendByte(port, 0x00, Checksum);
         Checksum = SendByte(port, size & 0xff, Checksum);
         Checksum = SendByte(port, size >>0x08, Checksum);
         break;

      case DATA_END:
         Checksum = SendByte(port, 0x00, Checksum);
         break;

      default:
         return INVALID_FUNCTION;
   }

   /* send CRCM (CRC Marker) */
   SendByte(port, 0x10, 0);   /* No CRC on this one (tricky!) */
   Checksum = SendByte(port, 0x03, Checksum);

   /* Finally send Checksum LO, HI */
   SendByte(port, (Checksum & 0xff), 0);
   SendByte(port, (Checksum >> 8), 0);

   return PACKET_SEND;   /* everything O.K. */

}

/* --------------------------------------------------------------------
                        Get packet from server
   -------------------------------------------------------------------- */

int GetPacket(FILERCOM *pPacket)
{

   int c=0, f, end_of_dir = 0;
   BYTE data = 0, crchi, crclo, sizehi, sizelo;
   WORD port, Checksum = 0;
   BYTE Signature[] = { 0x16, 0x16, 0x16, 0x10, 0x02 }; /* packet signature */

   if(!pPacket) return SERVER_CLOSED;
   port = pPacket->Port;

   /* wait for signature till timeout */
   for(;;) {
      if(GetByte(port, &data, NULL)==TIMEOUT) return TIMEOUT;
      if(data!=Signature[c++]) c=0;
      if(c==5) break;
   }

   /* get SOH */
   if(GetByte(port, &data, &Checksum)==TIMEOUT) return TIMEOUT;
   if(data != SOH) return BAD_PACKET;

   /* get function */
   if(GetByte(port, &data, &Checksum)==TIMEOUT) return TIMEOUT;
   pPacket->Function = data;

   inportb(port);
   /* get packet count */
   if(GetByte(port, &data, &Checksum)==TIMEOUT) return TIMEOUT;
   pPacket->Count = data;

   /* get packet status */
   if(GetByte(port, &data, &Checksum)==TIMEOUT) return TIMEOUT;
   pPacket->Status = data;

   /* get data marker */
   if(GetByte(port, &data, &Checksum)==TIMEOUT) return TIMEOUT;
   if(data != 0x02) return BAD_PACKET;


   /* server replies */
   switch(pPacket->Function) {

      /* replies */
      case CONNECT_SERVER:
      case DISCONNECT_SERVER:
         /* data end marker */
         if(GetByte(port, &data, &Checksum)==TIMEOUT) return TIMEOUT;
         if(data) return BAD_PACKET;
         if(GetByte(port, &data, &Checksum)==TIMEOUT) return TIMEOUT;
         if(data) return BAD_PACKET;
         break;

      case MAKE_DIR:
      case DEL_DIR:
      case DEL_FILE:
      case SEND_PATH:
      case SEND_FILENAME:
      case GET_FILENAME:
      case SEND_DATA:
         /* get data marker and size MSB (always 0) */
         for(f=0;f<4;f++) {
            if(GetByte(port, &data, &Checksum)==TIMEOUT) return TIMEOUT;
            if(data) return BAD_PACKET;
         }

         /* get data size */
         if(GetByte(port, &data, &Checksum)==TIMEOUT) return TIMEOUT;
         sizelo = data;
         if(GetByte(port, &data, &Checksum)==TIMEOUT) return TIMEOUT;
         sizehi = data;;
         pPacket->Size = (sizehi * 256) + sizelo;
         break;

      case DATA_END:
      case INIT_GET:
      case ASK_DIR:
         /* get data size & end marker (6 bytes) */
         for(f=0;f<6;f++) {
            if(GetByte(port, &data, &Checksum)==TIMEOUT) return TIMEOUT;
            if(data) return BAD_PACKET;
         }
         break;

      /* data retreival */
      case GET_DATA:
         /* get data marker and size MSB (always 0) */
         for(f=0;f<4;f++) {
            if(GetByte(port, &data, &Checksum)==TIMEOUT) return TIMEOUT;
            if(data) return BAD_PACKET;
         }

         /* get data size */
         if(GetByte(port, &data, &Checksum)==TIMEOUT) return TIMEOUT;
         sizelo = data;
         if(GetByte(port, &data, &Checksum)==TIMEOUT) return TIMEOUT;
         sizehi = data;;
         pPacket->Size = (sizehi * 256) + sizelo;

         for(f=0; f<pPacket->Size; f++) {
            if(GetByte(port, &pPacket->pData[f], &Checksum)==TIMEOUT)
               return TIMEOUT;
         }

         break;

      case GET_DIR:
         /* get data marker and size MSB (always 0) */
         for(f=0;f<4;f++) {
            if(GetByte(port, &data, &Checksum)==TIMEOUT) return TIMEOUT;
            if(data) return BAD_PACKET;
         }

         /* get data size */
         if(GetByte(port, &data, &Checksum)==TIMEOUT) return TIMEOUT;
         sizelo = data;
         if(GetByte(port, &data, &Checksum)==TIMEOUT) return TIMEOUT;
         sizehi = data;;
         pPacket->Size = (sizehi * 256) + sizelo;

         for(f=0; f<pPacket->Size; f++) {
            if(GetByte(port, &pPacket->pData[f], &Checksum)==TIMEOUT)
               return TIMEOUT;
         }

         /* special 'dir' data end marker */
         if(GetByte(port, &data, &Checksum)==TIMEOUT) return TIMEOUT;
         if((data!=0x01) && (data != 0x00)) return BAD_PACKET;
         if(data == 0x00) end_of_dir = 1;
         if(GetByte(port, &data, &Checksum)==TIMEOUT) return TIMEOUT;
         if(data) return BAD_PACKET;
         if(GetByte(port, &data, &Checksum)==TIMEOUT) return TIMEOUT;
         if(data) return BAD_PACKET;
         if(GetByte(port, &data, &Checksum)==TIMEOUT) return TIMEOUT;
         if(data) return BAD_PACKET;

         break;

      default:
         return INVALID_FUNCTION;

   }

   /* get CRC Marker (no CRC on the next byte!) */
   if(GetByte(port, &data, NULL)==TIMEOUT) return TIMEOUT;
   if(data != 0x10) return BAD_PACKET;
   if(GetByte(port, &data, &Checksum)==TIMEOUT) return TIMEOUT;
   if(data != 0x03) return BAD_PACKET;

   /* get received CRC */
   if(GetByte(port, &data, NULL)==TIMEOUT) return TIMEOUT;
   crclo = data;
   if(GetByte(port, &data, NULL)==TIMEOUT) return TIMEOUT;
   crchi = data;
   pPacket->CRC16 = (crchi * 256) + crclo;

   /* check if CRC is good */
   if(pPacket->CRC16 != Checksum) return BAD_CRC;

#ifdef DEBUG_FILER
   printf("\n\nGOT PACKET# : 0x%X\n", pPacket->Count);
   printf("GOT Function: 0x%X\n", pPacket->Function);
   printf("GOT Status  : 0x%X\n", pPacket->Status);
   printf("GOT Size    : 0x%X\n", pPacket->Size);
#endif

   if(end_of_dir) return CANNOT_GET_ENTRY;
   return PACKET_RECVD;
}


/* --------------------------------------------------------------------
                      Talk (handshake) with server
   -------------------------------------------------------------------- */

int FilerRequest(FILERCOM *pFiler, int Request, WORD Size, BYTE *pData)

{
   int attempt = 0, stat;
   WORD Count;

   Count = pFiler->Count;

   /* packet handshake */
   while(attempt<NUM_OF_ATTEMPTS) {
      SendPacket(pFiler, Request, Count, Size, pData);
      stat=GetPacket(pFiler);
      if(stat==PACKET_RECVD) break;
      if(stat == CANNOT_GET_ENTRY) break;
      attempt++;
   }

   if(attempt==NUM_OF_ATTEMPTS) return NO_RESPONSE;
   if(pFiler->Function != (BYTE)Request) return BAD_REQUEST;
   if(pFiler->Count != Count) return BAD_PACKET_COUNT;

   /* update packet counter */
   Count++;
   pFiler->Count = Count;

   if(stat == CANNOT_GET_ENTRY) return stat;
   return SERVER_ACK;    /* client <> server agree */

}

/* --------------------------------------------------------------------
                      Open Filer communications
   -------------------------------------------------------------------- */

FILERCOM *FilerConnect(int PortNumber, unsigned long BaudRate, FLCB *pCb)
{

   int f;
   FILERCOM *pFiler;

   static unsigned int  port[]= {0, 0x3f8, 0x2f8, 0x3e8, 0x2e8 };

   static unsigned long BaudTable[]= {
      0,115200L,57600L,38400L,19200L,9600L,4800L,2400L,1200L,INV_BAUDRATE
   };

   /* check if PortNumber is valid (valid= 1 to 4) */
   if((PortNumber > 4)||(PortNumber ==0)) return 0;

   /* check for valid baud-rate */
   for(f=0;f<10;f++) {
      if(BaudRate == BaudTable[f]) break;
   }
   if(BaudTable[f]==INV_BAUDRATE) return 0;   /* invalid baudrate */


   /* allocate packet storage */
   if(!(pFiler = malloc(sizeof(FILERCOM)))) return 0;

   /* packet structure defaults */
   pFiler->pData=0;
   pFiler->Function=0;
   pFiler->Count=0;
   pFiler->Status=0;
   pFiler->CRC16=0;
   pFiler->Port=port[PortNumber];
   pFiler->pCb = pCb;

   /* allocate data buffer storage */
   if(!(pFiler->pData = malloc(sizeof(FILERCOM)+PACKET_DATA_SIZE+100)))
      return 0;

   if(!BaudRate) {
   /* Determine baudrate automatically if given baudrate == 0 */
      for(f=1;f<10;f++) {

         if(inportb(0x60)==1) {          /* ESC key pressed ? */
            free(pFiler->pData);
            free(pFiler);
            return 0;
         }

         /* no response from server */
         if(BaudTable[f]==INV_BAUDRATE) {
            free(pFiler->pData);
            free(pFiler);
            return 0;
         }

         pFiler->Baud = BaudTable[f];
         SetBaudRate(pFiler->Port, pFiler->Baud);

         /* try 2 attempts */
         SendPacket(pFiler, CONNECT_SERVER, 0,0, NULL);
         if(GetPacket(pFiler)==PACKET_RECVD) break;

         SendPacket(pFiler, CONNECT_SERVER, 0,0, NULL);
         if(GetPacket(pFiler)==PACKET_RECVD) break;
      }
   }

   else {
      pFiler->Baud=BaudRate;
      SetBaudRate(pFiler->Port, BaudRate);
   }

   if(FilerRequest(pFiler, CONNECT_SERVER, 0, NULL) == NO_RESPONSE) {
      free(pFiler->pData);
      free(pFiler);
      return 0;
   }
   return pFiler;
}


/* --------------------------------------------------------------------
                     CloseServer channel (disconnect)
   -------------------------------------------------------------------- */

int FilerDisconnect(FILERCOM *pFiler)
{

   if(!pFiler) return SERVER_CLOSED;
   pFiler->Count=0;

   if(FilerRequest(pFiler, DISCONNECT_SERVER, 0, NULL) == NO_RESPONSE) {
      return NO_RESPONSE;
   }

   free(pFiler->pData);
   free(pFiler);

   return SERVER_CLOSED;

}


