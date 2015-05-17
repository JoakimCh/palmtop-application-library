/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  SHOWTOPT.C
   Author:  Dave Vickers
   Started: 20. Jan. 95
   Subject: Show title, date and time on the top line of the screen
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                          standard includes
   -------------------------------------------------------------------- */

#include <limits.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

/* --------------------------------------------------------------------
                           local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"

/* --------------------------------------------------------------------
                        global variables
   -------------------------------------------------------------------- */
char  *pSetupFileName = "C:\\_DAT\\SETUP.ENV";

struct {
   BYTE  DateFmt;      /* LX Date format */
   BYTE  Filler;       /* Unknown */
   BYTE  TimeFmt;      /* LX Time format */
} TimeSetup = { 3, 0, 1 };

#define  MAX_DATE_FMT  10   /* Highest HP Time Format code */
#define  MAX_TIME_FMT   9   /* Highest HP Time Format code */

/* --------------------------------------------------------------------
                           Functions
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
   ReadSetup:
   Read the Date & Time formats from the C:\_DAT\SETUP.ENV file.
   -------------------------------------------------------------------- */

void ReadSetup(void)
{
   FILE  *pSetupFile;
   static BYTE GotSetup = 0;

   if(GotSetup) return;
   if(!(pSetupFile = fopen(pSetupFileName, "rb"))) return;
   fseek(pSetupFile, 12L, SEEK_SET);
   fread(&TimeSetup, sizeof(TimeSetup), 1, pSetupFile);
   fclose(pSetupFile);
   GotSetup = 1;
}

/* --------------------------------------------------------------------
   FormatDate:
   Format a date for display, using the date format specified in the HP
   C:\_DAT\SETUP.ENV file.
   Inputs:
      struct tm *pTm - Pointer to the date/time array to be formatted.
                       Only month, day and year are used.
   Outputs:
      char *DateStr  - Formatted ASCIIZ date string.
                       Max length 10 bytes, including terminating null.
   Returns:
      A pointer to the formatted date string.
   -------------------------------------------------------------------- */

char *FormatDate(char *DateStr, struct tm *pTm)
{
   WORD   Fmt;
   int    Cnt;
   static char  MonthNames[] = "JanFebMarAprMayJunJulAugSepOctNovDec";

#define  DD     1
#define  MM     2
#define  YY     3
#define  MMM    4
#define  DASH   5
#define  SLASH  6
#define  DOT    7
#define  DATE_FMT(a, b, c, d, e)  (((a) << 12) | \
                                   ((b) << 9)  | \
                                   ((c) << 6)  | \
                                   ((d) << 3)  | (e))

   static WORD DateFmtTbl[MAX_DATE_FMT+1] = {  /* HP LX Date format table */

      /* 0  DD-MMM-YY */  DATE_FMT( DD , DASH , MMM, DASH , YY ),
      /* 1  DD-MMM    */  DATE_FMT( DD , DASH , MMM, 0    , 0  ),
      /* 2  MMM-YY    */  DATE_FMT( MMM, DASH , YY , 0    , 0  ),
      /* 3  MM/DD/YY  */  DATE_FMT( MM , SLASH, DD , SLASH, YY ),
      /* 4  DD/MM/YY  */  DATE_FMT( DD , SLASH, MM , SLASH, YY ),
      /* 5  DD.MM.YY  */  DATE_FMT( DD , DOT  , MM , DOT  , YY ),
      /* 6  YY-MM-DD  */  DATE_FMT( YY , DASH , MM , DASH , DD ),
      /* 7  MM/DD     */  DATE_FMT( MM , SLASH, DD , 0    , 0  ),
      /* 8  DD/MM     */  DATE_FMT( DD , SLASH, MM , 0    , 0  ),
      /* 9  DD.MM     */  DATE_FMT( DD , DOT  , MM , 0    , 0  ),
      /*10  MM-DD     */  DATE_FMT( MM , DASH , DD , 0    , 0  )};

   static char *ppTmp[8] = { "",
                             "dd",     /* DD */
                             "dd",     /* MM */
                             "dd",     /* YY */
                             "ccc",    /* MMM */
                             "-",      /* DASH */
                             "/",      /* SLASH */
                             "." };    /* DOT */

   ReadSetup();

   sprintf(ppTmp[DD], "%02d", pTm->tm_mday);
   sprintf(ppTmp[MM], "%02d", pTm->tm_mon+1);
   sprintf(ppTmp[YY], "%02d", pTm->tm_year % 100);
   memcpy(ppTmp[MMM], &MonthNames[pTm->tm_mon*3], 3);

   DateStr[0] = '\0';

   for (Fmt = DateFmtTbl[TimeSetup.DateFmt], Cnt = 4;  Cnt >= 0;  --Cnt)
     strcat(DateStr, ppTmp[(Fmt >> (3 * Cnt)) & 0x7]);

   return DateStr;
}

/* --------------------------------------------------------------------
   FormatTime:
   Format a time for display, using the time format specified in the HP
   C:\_DAT\SETUP.ENV file.
   Inputs:
      struct tm *pTm - Pointer to the time array to be formatted.
                       Only hour, minutes and seconds are used.
   Outputs:
      char *TimeStr  - Formatted ASCIIZ time string.
                       Max length 12 bytes, including terminating null.
   Returns:
      A pointer to the formatted time string.
   -------------------------------------------------------------------- */

char *FormatTime(char *TimeStr, struct tm *pTm)
{

   DWORD  Fmt;
   int    Cnt,
          Hour;

   #undef   MM

   #define  HH12   1    /* Hour: 12 hour format */
   #define  HH24   2    /* Hour: 24 hour format */
   #define  MM     3
   #define  SS     4
   #define  AMPM   5
   #define  COLON  6
   #define  DOT    7
   #define  COMMA  8
   #define  LCH    9    /* Lower Case h */
   #define  LCM   10    /* Lower Case m */
   #define  LCS   11    /* Lower Case s */

   #define  TIME_FMT(a, b, c, d, e, f)  ((long) a << 20 | (long) b << 16 | \
                                         c << 12 | d <<  8 | e <<  4 | f)

   static DWORD TimeFmtTbl[MAX_TIME_FMT+1] = {  /* HP LX Time format table */

      /* 0  HH:MM:SS (AM/PM) */  TIME_FMT( HH12, COLON, MM, COLON, SS, AMPM ),
      /* 1  HH:MM (AM/PM)    */  TIME_FMT( HH12, COLON, MM, AMPM , 0 , 0    ),
      /* 2  HH:MM:SS         */  TIME_FMT( HH24, COLON, MM, COLON, SS, 0    ),
      /* 3  HH.MM.SS         */  TIME_FMT( HH24, DOT  , MM, DOT  , SS, 0    ),
      /* 4  HH,MM,SS         */  TIME_FMT( HH24, COMMA, MM, COMMA, SS, 0    ),
      /* 5  HHhMMmSSs        */  TIME_FMT( HH24, LCH  , MM, LCM  , SS, LCS  ),
      /* 6  HH:MM            */  TIME_FMT( HH24, COLON, MM, 0    , 0 , 0    ),
      /* 7  HH.MM            */  TIME_FMT( HH24, DOT  , MM, 0    , 0 , 0    ),
      /* 8  HH,MM            */  TIME_FMT( HH24, COMMA, MM, 0    , 0 , 0    ),
      /* 9  HHhMMm           */  TIME_FMT( HH24, LCH  , MM, LCM  , 0 , 0    )};

   static char *ppTmp[12] = { "",
                              "dd",     /* HH12  */
                              "dd",     /* HH24  */
                              "dd",     /* MM    */
                              "dd",     /* SS    */
                              " am",    /* AMPM  */
                              ":",      /* COLON */
                              ".",      /* DOT   */
                              ",",      /* COMMA */
                              "h",      /* LCH   */
                              "m",      /* LCM   */
                              "s" };    /* LCS   */


   ReadSetup();

   if((Hour = pTm->tm_hour % 12) == 0)
      Hour = 12;

   sprintf(ppTmp[HH12], "%2d",  Hour);
   sprintf(ppTmp[HH24], "%2d",  pTm->tm_hour);
   sprintf(ppTmp[MM],   "%02d", pTm->tm_min);
   sprintf(ppTmp[SS],   "%02d", pTm->tm_sec);
   ppTmp[AMPM][1] = (char) ((pTm->tm_hour < 12) ? 'a' : 'p');

   TimeStr[0] = '\0';

   for (Fmt = TimeFmtTbl[TimeSetup.TimeFmt], Cnt = 5;  Cnt >= 0;  --Cnt)
     strcat(TimeStr, ppTmp[(Fmt >> (4 * Cnt)) & 0xF]);

   return TimeStr;
}

/* --------------------------------------------------------------------
   ShowTopTime
   Display the title, date and time on the top line of the HP display.
   Inputs:
      char *pTitle     - Title string for left part of top line.
                         Max length 57 bytes.
      int  ForceUpdate - Normally the title and date/time are not updated
                         if the time has not changed since the last update.
                         To force an update, specify ForceUpdate as TRUE.
   -------------------------------------------------------------------- */

void ShowTopTime(char *pTitle, int ForceUpdate)
{

   char    Str[24];
   time_t  CurrTime;
   static  time_t  PrevTime;
   struct  tm  *pLocalTm;

   /* If the time has not changed, then there is no need to update screen */

   time(&CurrTime);
   if(CurrTime == PrevTime && !ForceUpdate)
      return;

   /* Format the date and time */

   PrevTime = CurrTime;
   pLocalTm = localtime(&CurrTime);

   strcpy(Str, " ");
   FormatDate(Str + 1, pLocalTm);
   strcat(Str, " ");
   FormatTime(Str + strlen(Str), pLocalTm);

   /* Display the title, date and time */
   ShowTopLine(pTitle, Str);
}

