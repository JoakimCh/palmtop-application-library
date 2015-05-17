/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  SCUTILS.C
   Author:  Robert Chernow
   Started: 25.11.95
   Subject: Software Carousel Utility and Exerciser
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                          Headers
   -------------------------------------------------------------------- */

#include "palsc.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* --------------------------------------------------------------------
                          Globals
   -------------------------------------------------------------------- */
/* flags and variables that are set from user input and later interpreted
    by main() and executed
*/

int fHELP = 0,
    fISSCHERE = 0,
    fQUITFAST = 0,
    fQUITSLOW = 0,
    fBOOTFAST = 0,
    fBOOTSLOW = 0,
    fCODEMODE = 0,
    fKEYCOMMAND = 0,
    fKEYCMD = 0,
    fMENU = 0,
    fSCPRESENT,
    fKILL = 0,
    fSIZESTAT = 0,
    fSETSIZE = 0,
    fSETSIZESESSION = 0,
    fNAME = 0,
    fSETNAMESESSION = 0,
    fCMDSESSION = 0,
    fSWITCH = 0;

char szNAME[25] = "",
     szCMD[128] = "";


/* --------------------------------------------------------------------
                          Functions
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
   isschere: test prescence of SC and print version.

   -------------------------------------------------------------------- */
int isschere( void )
{
   /* variable to receive results of the function call
   */
   int     version_number;

   /* do function to see if SC is loaded
   */
   version_number = SCPresent();

   /* display appropriate message
   */
   if(version_number)
      printf("\nSOFTWARE CAROUSEL version %d is loaded", version_number);
   else
      printf("\nSOFTWARE CAROUSEL is not loaded");

   return(version_number);
}

/* --------------------------------------------------------------------
   Get2Numbers: Convert string (n1,n2) to integers and eat string.

   -------------------------------------------------------------------- */

void Get2Numbers( char *s, int *n1, int *n2 )
{
   char *psz;


   if (*s != '\0') {
      psz = strtok(s,",");
      *n1 = atoi(psz);
   }
   if (*psz != '\0') {
      psz = strtok(NULL,",");
      *n2 = atoi(psz);
   }
   *s = '\0';
}


/* --------------------------------------------------------------------
   GetNumberString: Convert string (n1,s1) to integer and string.

   -------------------------------------------------------------------- */

void GetNumberString( char *s, int *n1, char *s1 )
{
   char *psz;


   if (*s != '\0') {
      psz = strtok(s,",");
      *n1 = atoi(psz);
   }

   if (*psz != '\0') {
      psz = strtok(NULL,",");
      strcpy(s1,psz);
   }
   *s = '\0';
}

/* --------------------------------------------------------------------
   GetNumbers: Convert string to integer and eat string.

   -------------------------------------------------------------------- */
int GetNumber( char *s )
{
   int rc;
   if (*s != '\0') {
      rc = atoi(s);
   }
   *s = '\0';
   return rc;
}
/* --------------------------------------------------------------------
   HandleArgs: Handle an argument string

   -------------------------------------------------------------------- */
void HandleArg(char *s)
{
   int c;

   while((c = *s++) != 0) {
      switch(c) {
         case '-': break;
         case '/': break;
         case 'b':
            if (*s == '1'){
                fBOOTFAST = 1;
                s++;
            } else if (*s == '0'){
                fBOOTSLOW = 1;
                s++;
            }
            break;
         case 'c':
            /* get the session number
            */
            if (*s != '\0') {
                GetNumberString(s, &fCMDSESSION, szCMD);
            }
            break;
         case 'C': fCODEMODE = 1; break;
         case 'h':
         case 'H':
         case '?': fHELP = 1;
         case 'k':
            /* get the session number
            */
            if (*s != '\0') {
                fKILL = GetNumber(s);
            }
            break;
         case 'K':
            /* get the key command
            */
            if (*s != '\0') {
                fKEYCMD = GetNumber(s);
                fKEYCOMMAND = 1;
            }
            break;
         case 'm': fMENU = 1; break;
         case 'n':
            /* get the session number
            */
            if (*s != '\0') {
                fNAME = GetNumber(s);
            }
            break;
         case 'N':
            /* get the session number
            */
            if (*s != '\0') {
                GetNumberString(s, &fSETNAMESESSION, szNAME);
            }
            break;
         case 'p': fISSCHERE = 1; break;
         case 'q':
            if (*s == '1'){
                fQUITFAST = 1;
                s++;
            } else if (*s == '0'){
                fQUITSLOW = 1;
                s++;
            }
            break;
         case 's':
            /* get the session number
            */
            if (*s != '\0') {
                fSWITCH = GetNumber(s);
            }
            break;
         case 'v': fISSCHERE = 1; break;
         case 'z':
            /* get the session number
            */
            if (*s != '\0') {
                fSIZESTAT = GetNumber(s);
            }
            break;
         case 'Z':
            /* get the session number
            */
            if (*s != '\0') {
                Get2Numbers(s, &fSETSIZESESSION, &fSETSIZE);
            }
            break;

         default:
            fprintf(stderr, "SCUTILS: Warning: ignoring unknown switch %c\n", c);
            /* eat rest of arg
            */
            GetNumber(s);
            break;
      }
   }
}

/* --------------------------------------------------------------------
   PrintSCError: Printf's an SC Error message

   -------------------------------------------------------------------- */
void PrintSCError(int number)
{
    char *msg;
    msg = SCErrorMessage(number);
    printf(msg);
}

/* --------------------------------------------------------------------
   ShowHelp: Explain all the switches

   -------------------------------------------------------------------- */
void ShowHelp( void )
{

   int i;
   char *hlp[21] = {
  "                   SCUTILS COMMAND REFERENCE",
  "",
  "  SWITCH    ARGUMENTS        DESCRIPTION",
  "  -b        none             Reboot Computer",
  "  -c        number,string    Send String to Session number",
  "  -C        none             Get where code is running",
  "  -h|-H|-?  none             Show this help screen",
  "  -k        number           Kill program in Session number",
  "  -K        number           Set/Get Key switch status",
  "  -m        none             Invoke SC Menu",
  "  -n        number           Get Session numbers Name",
  "  -N        number,string    Set Session numbers Name to string",
  "  -p        none             Determine if SC is Present",
  "  -q        number           Quit SC with(0) or without(1) prompting",
  "  -s        number           Switch to Session number",
  "  -v        none             Show SC Version number",
  "  -z        number           Get Session numbers Size",
  "  -Z        number1,number2  Set Session number1s Size to number2",
  "",
  " example:  Show version, send DIR C: to session 2 and activate it",
  "         scutils -v \"-c2,DIR C:\" -s2",
  };

   for (i=0;i<21;i++) {
      printf("\n%s",hlp[i]);
   }


}


int main(int argc, char** argv)
{
   int rc=0, i, size, status;
   char name[26];

   /* work thru all switches
   */
   for(i = 1; i < argc; i++) HandleArg(argv[i]);

   /* test for SC presence
   */
   if (fISSCHERE) {
      rc = isschere();
   }

   /* if user asks for help, give it and get out.
   */
   if (fHELP) {
      ShowHelp();
      exit(0);
   }

   /* Get version
   */
   fSCPRESENT = SCPresent();

   /* only continue if SC is present
   */
   if (fSCPRESENT) {

      /* get a sessions size and status
      */
      if (fSIZESTAT) {
         rc = SCSizeStatus(fSIZESTAT, &size, &status);
         if (rc == 0) {
            printf("\nSession %d has a size of %d", fSIZESTAT, size);
            if (status) {
               printf("\n and a program is running.");
            } else {
               printf("\n and no program is running.");
            }
         } else {
            PrintSCError(rc);
         }
      }

      /* Get a sessions name
      */
      if (fNAME) {
         rc = SCGetName(fNAME, name);
         if (rc == 0) {
            printf("\nSession %d is called [%s]", fNAME, name);
         } else {
            PrintSCError(rc);
         }
      }

      /* set a sessions name
      */
      if (fSETNAMESESSION) {
         rc = SCSetName(fSETNAMESESSION, szNAME);
         if (rc == 0) {
            printf("\nSession %d name set to [%s]", fSETNAMESESSION, szNAME);
         } else {
            PrintSCError(rc);
         }
      }

      /* send a command to a session
      */
      if (fCMDSESSION) {
         rc = SCSendCommand(fCMDSESSION, szCMD);
         if (rc == 0) {
            printf("\nSession %d was sent command [%s]",
                    fCMDSESSION, szCMD);
         } else {
            PrintSCError(rc);
         }
      }

      /* set a sessions size
      */
      if (fSETSIZESESSION) {
         rc = SCSetSize(fSETSIZESESSION, fSETSIZE);
         if (rc == 0) {
            printf("\nSession %d set to a size of %d kbytes",
                   fSETSIZESESSION, fSETSIZE);
         } else {
            PrintSCError(rc);
         }
      }

      /* get the code mode
      */
      if (fCODEMODE) {
         rc = SCCodeMode( &fCODEMODE );
         if (rc == 0) {
            printf("\nCode Mode is [%d]",
                   fCODEMODE);
         } else {
            PrintSCError(rc);
         }
      }

      /* get/set keyboard commands
      */
      if (fKEYCOMMAND) {
         rc = SCKeyboardCommands( &fKEYCMD );
         if (rc == 0) {
            printf("\nKeyCommand Mode is [%d]",
                   fKEYCMD);
         } else {
            PrintSCError(rc);
         }
      }

      /* perform any session switches
      */
      if (fSWITCH) {
         rc = SCSwitch(fSWITCH);
         if (rc == 0) {
            printf("\nSwitched to session [%d]",
                   fSWITCH);
         } else {
            PrintSCError(rc);
         }
      }

      /* kill any sessions
      */
      if (fKILL) {
         rc = SCKill(fKILL);
         if (rc == 0) {
            printf("\nSession [%d] has been killed.",
                   fKILL);
         } else {
            PrintSCError(rc);
         }
      }

      /* perform any menu request
      */
      if (fMENU) {
         rc = SCMenu();
         if (rc == 0) {
            printf("\nMenu Invoked");
         } else {
            printf("\nMenu not invoked");
         }
      }

      /* quit sc without prompt
      */
      if (fQUITFAST){
         SCQuit(1);
      }

      /* quit sc with users permission
      */
      if (fQUITSLOW){
         SCQuit(0);
      }

      /* boot without prompt
      */
      if (fBOOTFAST){
         SCBoot(1);
      }

      /* boot with users permission
      */
      if (fBOOTSLOW){
         SCBoot(0);
      }
   }
   return(rc);
}

