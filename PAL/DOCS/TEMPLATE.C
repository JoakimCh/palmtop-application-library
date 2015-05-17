/* --------------------------------------------------------------------
   Project: PAL: Palmtop Application Library
   Module:  <The name of the module - usually the filename>
   Author:  <The name(s) of the autor(s)>
   Started: <Date when started>
   Subject: <What does it do, what is it there for ?>
   -------------------------------------------------------------------- */

[This is a template for a PAL .C or .H module. Delete the
comments in angular brackets when creating a module, and also
remember to fill out the header fields. If you modified an
existing module, add your name and comments on your
modification/extension below the 'Subject:' field, in a
'Modified:' field. Remove empty sections unless you feel that
they won't be empty RSN.]

/* --------------------------------------------------------------------
                       standard includes
   -------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>

[Add more of the standard ANSI C include headers here as needed.
Please do add only those #include files that are needed, but do
add those. If this module is a .H file, please try to avoid
(nested) includes if at all possible. Remove these
sections entirely in this case.]

/* --------------------------------------------------------------------
                         msdos includes
   -------------------------------------------------------------------- */
[MSDOS-specific includes go here if needed, like <bios.h> or <conio.h>]

/* --------------------------------------------------------------------
                         local includes
   -------------------------------------------------------------------- */
#include "pal.h"
#include "palpriv.h"

[If your module has an include file in its subdirectory, add it here.]


/* --------------------------------------------------------------------
                      constant definitions
   -------------------------------------------------------------------- */
[#define FOO 42 stuff]

/* --------------------------------------------------------------------
                       macro definitions
   -------------------------------------------------------------------- */
[Any #define FOO(bar) foobar(bar) sort of thing]

/* --------------------------------------------------------------------
                        type definitions
   -------------------------------------------------------------------- */
[for typedefs. Always create a typedef for structures.]

/* --------------------------------------------------------------------
                        global variables
   -------------------------------------------------------------------- */
[If you _really_ need them, put them here.]

/* --------------------------------------------------------------------
                           prototypes
   -------------------------------------------------------------------- */
[Add ANSI prototypes here. Prototypes should rarely be needed in .C
files - use bottom-up ordering instead.]

/* --------------------------------------------------------------------
                           functions
   -------------------------------------------------------------------- */
[Here's where your function or functions come in. Please ease full ANSI
style headers, i.e.

   void MyFunction(int x)

instead of

   my_function(x)
      int x;

If you have several functions in a .C file, please order them callee first,
then caller, i.e. bottom-up style. This requires less prototypes.
Please include a header before each function describing what it does.
Avoid 'boxy' comments - they look nice, but are less likely to get updated.

No functions should ever appear in .H files of course.]


