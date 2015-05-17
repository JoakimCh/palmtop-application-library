#include <stdio.h>
#include <stdlib.h>

#define BEFORE 0xe2f446c6L
#define AFTER  0xdef446c6L

void main(void)
{
   FILE *f;
   unsigned long l;

   f = fopen("cs.lib", "rb+");
   if(!f) {
      fprintf(stderr, "Open CS.LIB failed.\n");
      exit(1);
   }

   fseek(f, 0xc9f7L, SEEK_SET);
   fread(&l, sizeof(l), 1, f);
   if(l == AFTER) {
      fprintf(stderr, "Patch is installed.\n");
      exit(1);
   }
   if(l != BEFORE) {
      fprintf(stderr, "Unexpected value at patch location - aborting.\n");
      exit(1);
   }
   l = AFTER;
   fseek(f, 0xc9f7, SEEK_SET);
   fwrite(&l, sizeof(l), 1, f);
   fclose(f);
   fprintf(stderr, "Patch done - run again to check\n");
}

