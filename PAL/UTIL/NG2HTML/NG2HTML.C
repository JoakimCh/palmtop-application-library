/*******************************************************************/
/*                                                                 */
/* Convert PAL NG sources to HTML format                           */
/*                                                                 */
/* A. Garzotto, June 1995                                          */
/*                                                                 */
/*******************************************************************/

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define YES 1
#define NO 0

#define PUT(x) fprintf(fout, "%s", x)

/*******************************************************************/

struct sym_desc
{
   char file[66];
   char name[40];
   struct sym_desc *next;
};

typedef struct sym_desc *SYM;

/*******************************************************************/

FILE *ffiles, *fin, *fout;
SYM sym = NULL;
char curr_file[20];
char lbuf[256];

/*******************************************************************/
/* extract file base name */

void strip_file(char *path, char *file)
{
   char *p = &path[strlen(path) - 1];

   while ((p > path) && (*p != '\\') && (*p != ':'))
      p--;
   p++;
   while (*p && (*p != '.'))
      *(file++) = *(p++);
   *file = '\0';
}

/*******************************************************************/
/* add anchor to symbol table */

void sym_add(char *fname, char *anchor)
{
   SYM s = (SYM)malloc(sizeof(struct sym_desc));
   strcpy(s->name, anchor);
   strcpy(s->file, fname);
   s->next = sym;
   sym = s;
}

/*******************************************************************/
/* find anchor in symbol table */

void sym_find(char *anchor, char *link)
{
   SYM s = sym;
   int len = strlen(anchor);

   while (s)
   {
      if (!strncmp(anchor, s->name, len))
      {
         sprintf(link, "%s.HTM#%s", s->file, s->name);
         return;
      }
      s = s->next;
   }
   fprintf(stderr, "WARNING: unresolved reference: %s\n", anchor);
   strcpy(link, "Unknown");
}

/*******************************************************************/
/* create HTML header */

void header(char *title)
{
   PUT("<HTML>\n<HEAD><TITLE>");
   PUT(title);
   PUT("</TITLE></HEAD>\n");
   PUT("<BODY>\n");
   PUT("<H1>");
   PUT(title);
   PUT("</H1>\n<HR>\n");
}

/*******************************************************************/
/* create HTML trailer */

void trailer(void)
{
   PUT("</BODY>\n</HTML>");
}

/*******************************************************************/
/* undo the last written characters */

void unput(int n)
{
   fseek(fout, -(long)n, SEEK_CUR);
}

/*******************************************************************/
/* replace '<', '>' and '&' */

void clean(char *buf)
{
   char buf1[256], *p = buf1, *q = buf;
   
   strcpy(buf1, buf);
   if (!strncmp(buf, "'!HTML:", 7))
   {
      strcpy(buf, &buf1[7]);
      return;
   }
   
   while (*p)
   {
      switch (*p)
      {
      case '<': strcpy(q, "&lt;"); q += 4; p++; break;
      case '>': strcpy(q, "&gt;"); q += 4; p++; break;
      case '&': strcpy(q, "&amp;"); q += 5; p++; break;
      default: *(q++) = *(p++); break;
      }
   }
   *q = '\0';
}

/*******************************************************************/
/* create a link */

char *put_link(char *p, int href, int collect, int show, int add_sym)
{
   char *q = p;
   int quotes = NO;
   char name[80], *np = name;
   
   if (href)
   {
      PUT("<A HREF=\"");
      if (add_sym)
      {
         PUT(curr_file);
         PUT(".HTM");
      }
      PUT("#");
   }
   else
      PUT("<A NAME=\"");
   if (*p == '"')
   {
      quotes = YES;
      p++;
   }
   if (quotes)
   {
      while (*p && (*p != '"'))
      {
         *(np++) = *p;
         fprintf(fout, "%c", *(p++));
      }
      *np = '\0';
      if (*p) fprintf(fout, "%c", *(p++));
   }
   else
   {
      while (*p && (*p != ':') && 
            ((*p > ' ') || (collect && *p && (p[1] > ' '))))
      {
         *(np++) = *p;
         fprintf(fout, "%c", *(p++));
      }
      *np = '\0';
      PUT("\"");
   }
   if (add_sym) sym_add(curr_file, name);
   PUT(">");
   if (!show)
      return p;
   p = q;
   if (quotes)
   {
      p++;
      while (*p && (*p != '"'))
         fprintf(fout, "%c", *(p++));
      p++;
   }
   else
   {
      while (*p && (*p != ':') &&
            ((*p > ' ') || (collect && *p && (p[1] > ' '))))
         fprintf(fout, "%c", *(p++));
   }
   PUT("</A>\n");
   return p;
}

/*******************************************************************/
/* put link to different module */

char *put_far_link(char *p)
{
   int quotes = NO;
   char name[80], *np = name;
   char link[80];
   
   if (*p == '"')
   {
      quotes = YES;
      p++;
   }
   if (quotes)
   {
      while (*p && (*p != '"'))
      {
         *(np++) = *(p++);
      }
      *np = '\0';
      if (*p == '"') p++;
   }
   else
   {
      while (*p && (*p != ':') && (*p > ' '))
         *(np++) = *(p++);
      *np = '\0';
   }

   sym_find(name, link);
   PUT("<A HREF=\"");
   PUT(link);
   PUT("\">");
   PUT(name);
   PUT("</A>\n");
   return p;
}

/*******************************************************************/
/* process short entries */

void process1(char *fname)
{
   char buf[256];
   char *p;
   
   fin = fopen(fname, "r");
   if (!fin)
   {
      fprintf(stderr, "Could not open file '%s'\n", fname);
      return;
   }
   while (fgets(buf, 254, fin))
   {
      clean(buf);
      if (!strnicmp(buf, "!short:", 7))
      {
         p = &buf[7];
         while (*p == ' ') p++;
         if (*p < ' ')
         {
            PUT("<P>\n");
            continue;
         }
         if (*p == '^') p += 2;
         while (*p == ' ') p++;
         p = put_link(p, YES, YES, YES, YES);
         PUT(p);
         PUT("<BR>\n");
      }
   }
   fclose(fin);
}

/*******************************************************************/
/* process first short entry */

void process2(char *fname)
{
   char *p;
   int a_needed = NO;
   int done = NO;
   
   fin = fopen(fname, "r");
   if (!fin)
   {
      fprintf(stderr, "Could not open file '%s'\n", fname);
      return;
   }
   while (fgets(lbuf, 254, fin))
   {
      clean(lbuf);
      if (!strnicmp(lbuf, "!short:", 7))
      {
         if (done) break;
         p = &lbuf[7];
         while (*p == ' ') p++;
         if (*p < ' ') continue;
         done = YES;
         if (*p == '^') p += 2;
         while (*p == ' ') p++;
         p = put_link(p, NO, YES, NO, NO);
         a_needed = YES;
      }
      else if (!strnicmp(lbuf, "!seealso:", 9))
      {
         p = &lbuf[9];
         while (*p == ' ') p++;
         PUT("SEE ALSO: \n");
         while (*p >= ' ')
         {
            p = put_far_link(p);
            while (*p == ' ') p++;
         }
      }
      else
      {
         p = lbuf;
         while (*p == ' ') p++;
         if (*p == '^')
         {
            p += 2;
            PUT("<HR>\n<H2>");
            PUT(p);
            PUT("</H2>\n");
         }
         else if (*p < ' ')
         {
            PUT("<P>\n");
         }
         else if (*p == '-')
         {
            PUT("<BR>");
            p += 2;
            p = put_link(p, YES, NO, YES, NO);
            PUT(p);
         }
         else
         {
            PUT(lbuf);
         }
         if (a_needed)
         {
            a_needed = NO;
            PUT("</A>");
         }
      }
   }
}

/*******************************************************************/
/* process all but the first short entry */

void process3(void)
{
   char *p;
   int inlist = NO;
   int initem = NO;
   
   do
   {
      clean(lbuf);
      if (!strnicmp(lbuf, "!short:", 7))
      {
         p = &lbuf[7];
         while (*p == ' ') p++;
         if (*p < ' ')
            continue;
         if (initem)
         {
            unput(4);
            PUT("</PRE>");
            initem = NO;
         }
         if (inlist)
         {
            PUT("</DL>");
            inlist = NO;
         }
         if (*p == '^') p += 2;
         while (*p == ' ') p++;
         PUT("<HR><H2>");
         p = put_link(p, NO, YES, YES, NO);
         PUT("</H2>\n");
      }
      else if (!strnicmp(lbuf, "!seealso:", 9))
      {
         if (initem)
         {
            unput(4);
            PUT("</PRE>");
            initem = NO;
         }
         p = &lbuf[9];
         while (*p == ' ') p++;
         PUT("<DT>SEE ALSO\n<DD>");
         while (*p >= ' ')
         {
            p = put_far_link(p);
            while (*p == ' ') p++;
         }
      }
      else
      {
         p = lbuf;
         while (*p == ' ') p++;
         if (*p == '^') p += 2;
         if (!strncmp(p, "NAME", 4) ||
             !strncmp(p, "DESC", 4) ||
             !strncmp(p, "SYNO", 4) ||
             !strncmp(p, "INPU", 4) ||
             !strncmp(p, "RETU", 4) ||
             !strncmp(p, "NOTE", 4) ||
             !strncmp(p, "SAMP", 4))
         {
            if (initem)
            {
               initem = NO;
               unput(4);
               PUT("</PRE>");
            }
            if (!strncmp(p, "NAME", 4))
            {
               PUT("<DL>\n");
               inlist = YES;
            }
            PUT("<DT>");
            PUT(p);
            PUT("<DD><PRE>");
            initem = YES;
         }
         else
         {
            PUT(lbuf);
         }
       }
      
   } while (fgets(lbuf, 254, fin));
   
   if (initem)
   {
      unput(4);
      PUT("</PRE>");
   }
   if (inlist)
      PUT("</DL>");

   fclose(fin);
}

/*******************************************************************/

void main(int argc, char **argv)
{
   char buf[128], buf1[20], title[66];;

   if (argc < 4)
   {
      fprintf(stderr, "USAGE: %s <source list> <output file name> <title>\n",
              argv[0]);
      exit(1);
   }
   ffiles = fopen(argv[1], "r");
   if (!ffiles)
   {
      fprintf(stderr, "Could not open file '%s'\n", argv[1]);
      exit(1);
   }
   fout = fopen(argv[2], "w");
   if (!fout)
   {
      fprintf(stderr, "Could not create file '%s'\n", argv[2]);
      exit(1);
   }
   header(argv[3]);
   printf("Pass 1:\n");
   while (fgets(buf, 126, ffiles))
   {
      buf[strlen(buf) - 1] = '\0';
      printf("Processing '%s'\n", buf);
      strip_file(buf, curr_file);
      process1(buf);
   }
   fclose(ffiles);
   trailer();
   fclose(fout);

   ffiles = fopen(argv[1], "r");
   if (!ffiles)
   {
      fprintf(stderr, "Could not open file '%s'\n", argv[1]);
      exit(1);
   }
   printf("Pass 2:\n");
   while (fgets(buf, 126, ffiles))
   {
      buf[strlen(buf) - 1] = '\0';
      printf("Processing '%s'\n", buf);
      strip_file(buf, curr_file);
      sprintf(buf1, "%s.HTM", curr_file);
      fout = fopen(buf1, "w");
      if (!fout)
      {
         fprintf(stderr, "Could not create file '%s'\n", buf1);
         exit(1);
      }
      *curr_file = toupper(*curr_file);
      sprintf(title, "%s: %s", argv[3], curr_file);
      header(title);
      process2(buf);
      process3();
      trailer();
      fclose(fout);
   }
   fclose(ffiles);
}

