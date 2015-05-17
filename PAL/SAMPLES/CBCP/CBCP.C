/* --------------------------------------------------------------------
   Project: Clipboard test program
   Module:  cbcp.c
   Author:  Ron Crain/Gilles Kohl
   Started: Tue 11-22-1994
   Subject: Test program - access SysMgr clipboard
   -------------------------------------------------------------------- */

#include <stdio.h>

#include "pal.h"
#include "palsmsvc.h"

#define CB_ERROR  "Fatal Clipboard Error\n"
#define CB_EMPTY  "Clipboard is <Blank>\n"

void read_cb( void )
{
   char buf[1024];
   int TextIndex;
   int TextLength;

   if(m_open_cb() != CB_OK) {
      fprintf(stderr, CB_ERROR );
      return;
   }
   if(m_rep_index( CB_REP_TEXT, &TextIndex, &TextLength) != CB_OK ) {
      fprintf( stderr, CB_EMPTY );
   }
   else {
      int pos  = 0;
      int read;
      int retval;
      int i;

      while( TextLength > 0 ) {
         if(( retval = m_cb_read( TextIndex, pos, buf, sizeof( buf )-1)) >= 0) {
            read = sizeof( buf )-1 - retval;
            buf[read] = '\0';

            for ( i=0; i < read; i++ ){
                if( buf[i]!=0x0d ) putchar( buf[i] );
                else putchar( 0x0a );
            }
            TextLength -= read;
            pos += read;
         }
         else {
            fprintf( stderr, CB_ERROR );
            TextLength = 0;
         }
      }
   }
   m_close_cb();
}

void main (void)
{
   PalInit(0);
   if(!CheckSysMgr()) FatalExit("SysMgr not loaded.", 1);
   read_cb();
   PalDeInit(0);
}

