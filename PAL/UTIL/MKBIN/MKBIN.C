/* --------------------------------------------------------------------
   Module:     MKBIN.C
   Author:     GK
   (Internet: gilles@spam.fido.de, FIDO 2:249/6.3, CIS:100114,3146)
   Started:    02.05.1993   12:18:38
   Modified:   02.05.1993   12:30:38
   Subject:    
   -------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
   FILE *f;
   int c;
   int n = 0;

   if(argc != 3) {
      fprintf(stderr, "Usage: mkbin <file name> <array name>\n");
      exit(1);
   }
   if(!(f = fopen(argv[1], "rb"))) {
      fprintf(stderr, "Can't open %s\n", argv[1]);
      exit(1);
   }
   printf("unsigned char %s[] = {", argv[2]);
   while( (c = fgetc(f)) != EOF ) {
      if(n % 12 == 0) {
         printf("\n   ");
      }
      printf("0x%02X, ", c);
      n++;
   }
   printf("\n};\n");
   return 0;
}
