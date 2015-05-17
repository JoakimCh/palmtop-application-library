/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  scerror.c
   Author:  Bob Chernow
   Started: 11/24/1995
   Subject: Software Carousel Error Message Handler
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                       standard includes
   -------------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>

/* --------------------------------------------------------------------
                         msdos includes
   -------------------------------------------------------------------- */
#include <dos.h>

/* --------------------------------------------------------------------
                         local includes
   -------------------------------------------------------------------- */
#include "palsc.h"

/* --------------------------------------------------------------------
                         global variables
   -------------------------------------------------------------------- */
char ErrorBuffer[80];

/* --------------------------------------------------------------------
                           functions
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
   SCErrorMessage: Sprint error mesage for error number and return buffer..

   -------------------------------------------------------------------- */
char *SCErrorMessage( int number )
{

char *msg[14] = {
         "(1) Invalid work area number was supplied",
         "(2) Can't switch to a work area with no memory size",
         "(3) Can't kill a program in a work area with nothing running",
         "(4) Can't change the size of an active work area",
         "(5) Invalid memory size was supplied",
         "(6) Can't send a command to a work area with a command pending",
         "(7) Software Carousel is busy with another 0DFh call; Try later",
         "(8) Invalid register values supplied",
         "(9) No memory block available",
         "(10) Invalid memory block ID",
         "(11) Invalid parameter block",
         "(12) Error reading or writing a memory block",
         "(13) Too many pending memory block requests",
         "(14) The video screen can not be saved when the Carousel menu is up."
         };

   sprintf(ErrorBuffer,"\n%s",msg[number-1]);
   return(ErrorBuffer);
}

