/* --------------------------------------------------------------------
   Project: PalMake: Palmtop Application Library Make utility
   Module:  PM.C
   Author:  Gilles Kohl
   Started: 29.12.94
   Subject: Simple Make tool to build the PAL lib and samples
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                       standard includes
   -------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

/* --------------------------------------------------------------------
                         msdos includes
   -------------------------------------------------------------------- */
#include <io.h>
#include <process.h>
#include <sys\types.h>
#include <sys\stat.h>

#if defined _MSC_VER || defined _QC
#include <dos.h>
#include <direct.h>
#define findfirst(a,b,c) _dos_findfirst(a,c,b)
#define findnext(a) _dos_findnext(a)
#define ffblk find_t
#define ff_name name
#else
#include <dir.h>
#endif


/* --------------------------------------------------------------------
                      constant definitions
   -------------------------------------------------------------------- */

#define MAX_PATH  70
#define MAX_LINE 256
#define MAX_ARGS  50
#define MAX_TGL  256

/* --------------------------------------------------------------------
                         local includes
   -------------------------------------------------------------------- */
#include "pal.h"

/* --------------------------------------------------------------------
                        global variables
   -------------------------------------------------------------------- */
int LxModel;

char Src[MAX_PATH];    /* current source file */
char Tgt[MAX_PATH];    /* current target file */
char Tgl[MAX_TGL];     /* target list         */
char Err[MAX_PATH];    /* error file          */

char PalDir[MAX_PATH]; /* PAL installation directory */

int LibMode  = 1; /* flag: attempting to build library ?           */
int BuildAll = 0; /* flag: ignore dependencies, build always       */
int CleanUp  = 0; /* flag: remove targets instead of building them */
int Debug    = 0; /* flag: output debug info instead of build ?    */

char *PrjFile = "srclist";

/* --------------------------------------------------------------------
                           functions
   -------------------------------------------------------------------- */
/*
 * Output a fatal error message (printf functionality), then bail out.
 * char *Fmt: Format string as in printf()
 *      ... : arguments as in printf()
 */
void Fatal(char *Fmt, ...)
{
   va_list ArgPtr;

   va_start(ArgPtr, Fmt);
   fprintf(stderr, "PM: Fatal error: ");
   vfprintf(stderr, Fmt, ArgPtr);
   fprintf(stderr, "\n");
   exit(-1);
}

/*
 * Return value of internal macro
 *
 */
char *GetMacroString(char *Key, char *Default)
{
   if(stricmp(Key, "src") == 0) return Src;
   if(stricmp(Key, "tgt") == 0) return Tgt;
   if(stricmp(Key, "tgl") == 0) return Tgl;
   if(stricmp(Key, "err") == 0) return Err;
   return Default;
}

/*
 * Envelope for GetConfigString - skips leading 'Lx' in Key if
 * not on palmtop or not found
 *
 */
char *GetConfigStr(char *Section, char *Key, char *Default)
{
   /* If on desktop, or if value not found, skip leading Lx
      in Key name if it is there */
   if( !LxModel || !GetConfigString(Section, Key, NULL )) {
      if(*Key == 'L' && Key[1] == 'x') Key += 2;
   }
   return GetConfigString(Section, Key, Default);
}

/*
 * Replace $(Section/Key) patterns in buffer with the corresponding
 * variable from the config file. Patterns with an empty Section
 * part will be replaced by internal macros if possible.
 *
 */
int ReplaceMac(char *Org, char *Dst)
{
   static char buf[MAX_LINE];
   int Subst;

   strcpy(buf, Org);
   Org = buf;

   Subst = 0;
   while(*Dst++ = *Org) {
      if(*Org == '$') {
         char *Section;
         char *Key;
         char *p;

         if((Org[1] == '(') &&
            (Key = strchr(Org, '/')) &&
            (p = strchr(Org, ')')) && p > Key) {
            ++Subst;
            Section = Org+2;
            *Key++ = '\0';
            *p++ = '\0';
            strcpy(--Dst,
               *Section ? GetConfigStr(Section, Key, "") :
                          GetMacroString(Key, ""));
            Org = p;
            Dst += strlen(Dst);
         }
         else ++Org;
      }
      else ++Org;
   }
   return Subst;
}

/*
 * Replace macros until none are left to replace
 *
 */
void ReplaceAllMac(char *Org, char *Dst)
{
   static char buf[MAX_LINE];
   int Subst;

   strcpy(buf, Org);
   Org = buf;
   do {
      if(Subst = ReplaceMac(Org, Dst)) strcpy(Org, Dst);
   } while(Subst);
}

/*
 * Get a variable from config file, replacing macros
 *
 */
char *GetConfigMac(char *Dest, char *Section, char *Key, char *Default)
{
   char *s;

   s = GetConfigStr(Section, Key, Default);
   if(!s) return NULL;
   ReplaceAllMac(s, Dest);
   return Dest;
}

/*
 * Set the current error variable - either to the passed string
 * (expanding macros) or use the current target name with the .err
 * extension.
 *
 */
void SetErr(char *s)
{
   if(s) {
      ReplaceAllMac(s, Err);
      return;
   }
   strcpy(Err, Tgt); /* copy name of object */
   s = strrchr(Err, '.'); /* find last dot */
   if(!s) s = Err + strlen(Err); /* point to end if not found */
   strcpy(s, ".err");
}

/*
 * Set the current source variable
 *
 */
void SetSrc(char *s)
{
   strcpy(Src, s); /* that's easy ... */
}

/*
 * Set the current target variable
 *
 */
void SetTgt(char *Tool)
{
   static char TargetExt[MAX_PATH];
   char *s;

   if(!GetConfigMac(TargetExt, Tool, "TargetExt", NULL)) {
      Fatal("Missing TargetExt in [%s] in PM.CFG", Tool);
   }
   strcpy(Tgt, Src);
   s = strrchr(Tgt, '.');
   if(!s) Fatal("Source %s has no extension", Src);
   strcpy(s, TargetExt);
}

/*
 * Compile a command into Dest buffer, giving Tool section and Key
 *
 */
void BuildCommand(char *Dest, char *Tool, char *Key)
{
   if(!GetConfigMac(Dest, Tool, Key, NULL)) {
      Fatal("Missing %s in section [%s] of PM.CFG", Key, Tool);
   }
}

/*
 * Execute a command, abort if it fails.
 *
 */
void ExecCmd(char *Cmd)
{
   char *Args[MAX_ARGS];
   char ErrFile[MAX_PATH];
   int Redir = 0;
   int i;
   char *s;

   printf("%s\n", Cmd);
   if(Debug) return;

   /* handle redirection in commandline here */
   if(s = strrchr(Cmd, '>')) { /* redirection requested ? */
      char *q;

      *s++ = '\0';                 /* terminate command there  */
      s += strspn(s, " \t");       /* skip leading whitespace  */
      q =  s+strcspn(s, " \t");    /* find end of filename     */
      *q = '\0';                   /* terminate filename there */
      strcpy(ErrFile, s);          /* copy to error file name  */
      freopen(ErrFile,"w",stdout); /* redirect stdout          */
      ++Redir;                     /* set redirection flag     */
   }

   /* loop over arguments, store pointers in Args array */
   for(i = 0, s = strtok(Cmd, " \t"); s; s = strtok(NULL, " \t")) {
      Args[i++] = s;
   }
   Args[i] = NULL;

   i = spawnvpe(P_WAIT, Args[0], Args, NULL); /* execute command */

   /* re-open stdout if it was redirected */
   if(Redir) freopen("con","w",stdout);

   if(i) Fatal("Command returned error %d.", i);
}

/*
 * Check if target requires to be rebuilt
 *
 */
int BuildRequired(char *s, char *t)
{
   struct stat TgtStat;
   struct stat SrcStat;

   if(BuildAll) return 1;

   /* does source exist at all ? */
   if(stat(s, &SrcStat) < 0) Fatal("Access source file %s failed", s);

   /* if target does not exist, make it very old */
   if(stat(t, &TgtStat) < 0) TgtStat.st_atime = 0L;

   /* return TRUE if source newer or same */
   return SrcStat.st_atime >= TgtStat.st_atime;
}

/*
 * Handle a single source file
 *
 */
int HandleSrcFile(char *s)
{
   static char Tool[MAX_PATH];
   static char Cmd[MAX_LINE];
   int  RetVal;
   char *Ext;

   /* first, compute the Src macro */
   SetSrc(s);

   /* now, find the extension */
   Ext = strrchr(Src, '.');

   if(Ext == NULL || strlen(Ext) > 4) {
      Fatal("File %s has missing or illegal extension\n", Src);
   }

   /* Find the tool section */
   if(!GetConfigMac(Tool, Ext, "LxTool", NULL)) {
      Fatal("Tool variable missing in section [%s]", Ext);
   }

   /* compute the name of the target file */
   SetTgt(Tool);

   /* set the name of the error file (implicitly) */
   SetErr(NULL);

   /* build target list if in 'Application' mode */
   if(!LibMode && ! CleanUp) {
      strcat(Tgl, Tgt);
      strcat(Tgl, " ");
   }

   /* anything to do at all ? */
   if(!BuildRequired(Src, Tgt)) return 0;

   /* now build and execute compile command */
   BuildCommand(Cmd, Tool, CleanUp ?
      "CleanUp" : (LibMode ? "LibCompile" : "AppCompile") );
   ExecCmd(Cmd);

   if(LibMode && !CleanUp) {
      /* build and execute archiving command */
      BuildCommand(Cmd, Tool, "Archive");
      ExecCmd(Cmd); /* no redirection for archiving either */
   }
   return 1;
}

/*
 * Comfort to the max: if srclist not found, try to create it.
 *
 */
void CreatePrjFile(void)
{
   struct ffblk fb;
   FILE *f;
   int Count;
   int Ok;

   /* warn that we're doing this */
   fprintf(stderr,
      "PM: Warning - %s not found, trying to create it\n", PrjFile);
   f = fopen(PrjFile, "w");
   if(!f) Fatal("Attempt to create %s failed", PrjFile);

   Count = 0;
   for(Ok = findfirst("*.c", &fb, 0); Ok == 0; Ok = findnext(&fb)) {
      ++Count;
      fprintf(f, "%s\n", fb.ff_name);
   }
   fclose(f);
   if(!Count) {
      unlink(PrjFile);
      Fatal("PM: No *.c files found, creation of source list failed");
   }
   fprintf(stderr, "PM: Created %s with %d file%s\n", PrjFile,
           Count, Count==1 ? "" : "s");
}


/*
 * main working horse
 *
 */
void Build(void)
{
   static char buf[MAX_LINE];
   FILE *f;
   int TotCount = 0;
   int Count = 0;

   /* If file listing the sources not available, try to create it,
      but only if in Application mode */
   if((access(PrjFile, 0) != 0) && !LibMode && !CleanUp) CreatePrjFile();

   /* open source file list */
   f = fopen(PrjFile, "r");
   if(!f) Fatal("Could not open file '%s' file !", PrjFile);

   /* clear target list */
   Tgl[0] = '\0';

   /* now, loop over lines in file */
   while(fgets(buf, sizeof(buf), f)) {
      buf[strlen(buf)-1] = '\0'; /* discard trailing '\n' */
      /* ignore empty lines and comments */
      if(*buf == '\0' || *buf == ';') continue;

      strlwr(buf); /* convert to lower case */

      /* now handle a single source file */
      Count += HandleSrcFile(buf);
      ++TotCount;
   }

   /* Done, if in app mode, execute link step now */
   if(!LibMode && !CleanUp) {
      char Cmd[MAX_LINE];
      char Tool[MAX_LINE];

      if(!GetConfigMac(Tool, ".c", "LxTool", NULL)) {
         Fatal("Section [.c] needs 'Tool =' for link step");
      }
      BuildCommand(Cmd, Tool, "Link");
      ExecCmd(Cmd);
   }
   fprintf(stderr, "PM: Processed %d of %d source file%s\n",
                   Count, TotCount, TotCount==1 ? "" : "s");
}


/*
 * Check if PalDir exists, and if user is located in src directory
 *
 */
int CheckPalDir(void)
{
   static char CurDir[MAX_PATH];
   static char buf[MAX_PATH];

   if(!GetConfigMac(PalDir, "Env", "LxPalDir", NULL)) {
      Fatal("PalDir variable in [Env] section in PM.CFG not set");
   }
   if(access(PalDir, 0) != 0) Fatal("PalDir '%s' does not exist", PalDir);
   getcwd(CurDir, sizeof(CurDir));
   sprintf(buf, "%s\\src", PalDir);
   return stricmp(CurDir, buf) == 0; /* true if in PalDir\src */
}

/*
 * Temporarily set a variable in PM.CFG (override)
 * Syntax of the string is Section/Variable=Value, e.g. .c/Tool=BC
 *
 */
void SetPmCfgVar(char *s)
{
   char *Sec;
   char *Key;
   char *Val;

   Sec = strdup(s);
   if(!(Key = strchr(Sec, '/'))) { free(Sec); return; }
   *Key++ = '\0';
   if(!(Val = strchr(Key, '='))) { free(Sec); return; }
   *Val++ = '\0';
   ReplaceConfigString(Sec, Key, Val);
   free(Sec);
}

/*
 * Handle an argument string
 *
 */
void HandleArg(char *s)
{
   int c;

   while(c = *s++) {
      switch(tolower(c)) {
         case '!': SetPmCfgVar(s);
                   s += strlen(s);
                   break;
         case 'c': CleanUp  = 1; /* Note: fall thru into build all */
         case 'a': BuildAll = 1; break;
         case 'd': Debug    = 1; break;
         case 'f': PrjFile  = strdup(s);
                   s += strlen(s);
                   break;
         default:
            fprintf(stderr, "PM: Warning: ignoring unknown switch %c\n", c);
            break;
      }
   }
}

/*
 * Main.
 *
 */
int main(int argc, char *argv[])
{
   int i;

   ReadMyConfig(argv[0]);
   LxModel = WhichHp();

   for(i = 1; i < argc; i++) HandleArg(argv[i]);
   LibMode = CheckPalDir();
   Build();
   return 0;
}

