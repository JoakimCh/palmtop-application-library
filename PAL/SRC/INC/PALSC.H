/* --------------------------------------------------------------------
   Project: Software Carousel Interface for PAL
   Module:  PALSC.H
   Author:  Bob Chernow
   Started: 7. Dec. 95
   Subject: Software Carousel Interface Header
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                             prototypes
   -------------------------------------------------------------------- */

/* Software Carousel Interface functions */

int  SCPresent( void );
int  SCVersion( void );
int  SCMenu( void );
int  SCSwitch( int session );
void SCQuit( int type );
void SCBoot( int type );
int  SCKill( int session );
int  SCSizeStatus( int number, int* size, int* status );
char *SCErrorMessage( int number );
int  SCGetName( int session, char *name );
int  SCSetSize( int session, int size );
int  SCSetName( int session, char *name );
int  SCSendCommand( int session, char * cmd );
int  SCCodeMode( int *mode );
int  SCKeyboardCommands( int *type );

