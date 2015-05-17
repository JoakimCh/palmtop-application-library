/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  GETIMG.C
   Author:  Harry Konstas/Gilles Kohl/Thomas Krug
   Started: 10. Nov. 94
   Subject: Get a bitmap image from screen
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
                            msdos includes
   -------------------------------------------------------------------- */
#include <dos.h>

/* --------------------------------------------------------------------
                           local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"

/* --------------------------------------------------------------------
                           Functions
   -------------------------------------------------------------------- */

/* --------------------------------------------------------------------
   GetImg:
   Get a rectangular bitmap from screen and store into buffer.
   Rectangle is given by upper left (x1,y1) and lower right (x2,y2)
   coordinates, sufficient space must be in buffer.
   If the buffer pointer is NULL, GetImg allocates sufficient storage
   and returns the pointer.
   -------------------------------------------------------------------- */

IMGHDR *GetImg(int x1, int y1, int x2, int y2, void far *Bitmap)
{
   /* Vector is a far pointer to a function that neither accepts nor
      returns parameters */
   void (far *Vector)(void);

   /* Here we use a rather nasty device - since one of the supported
      compilers doesn't have inline assembly, we fill an array with
      a 'preassembled' piece of code where we will patch parameters
      into. This code is then called by a far call. */

   static BYTE get_asm[] = {
      0x06,              /* 00: PUSH ES       */
      0x60,              /* 01: PUSHA         */
      0xB9, 0x00, 0x00,  /* 02: MOV  CX,Value */
      0xBA, 0x00, 0x00,  /* 05: MOV  DX,Value */
      0xBE, 0x00, 0x00,  /* 08: MOV  SI,Value */
      0xBD, 0x00, 0x00,  /* 11: MOV  BP,Value */
      0xBF, 0x00, 0x00,  /* 14: MOV  DI,Value */
      0xB8, 0x00, 0x00,  /* 17: MOV  AX,Value */
      0x8E, 0xC0,        /* 20: MOV  ES,AX    */
      0xB8, 0x00, 0x0d,  /* 22: MOV  AX,Value */
      0xCD, 0x5F,        /* 25: INT  5FH      */
      0x61,              /* 27: POPA          */
      0x07,              /* 28: POP  ES       */
      0xCB               /* 29: RETF          */
   };

   if(!Bitmap) {
      if(!(Bitmap = malloc(IMGSIZE(x1, y1, x2, y2)))) return NULL;
   }

   /* Now patch the assembly code above to correctly set up register
      values. The PARAM macro is given the address of an instruction,
      and will yield a pointer to the parameter word immediately
      following the instruction byte - this is used for assignment
      here.  */
   PARAM(get_asm + 2) = x1;
   PARAM(get_asm + 5) = y1;
   PARAM(get_asm + 8) = x2;
   PARAM(get_asm +11) = y2;
   PARAM(get_asm +14) = FP_OFF(Bitmap);
   PARAM(get_asm +17) = FP_SEG(Bitmap);

   /* Now, let 'Vector' point into our assembly language routine. We could
      also directly cast the pointer and call the routine with another
      macro, but TC 2.0 won't correctly compile such a construct. Hence
      the detour via another variable. */
   Vector = (void (far *)(void))get_asm;

   /* Finally, call the routine, indirectly via the vector */
   (*Vector)();
   return Bitmap;
}
