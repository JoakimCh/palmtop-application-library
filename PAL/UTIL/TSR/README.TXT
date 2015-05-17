To facilitate debugging on a desktop machine, CGAGRAPH.COM is
provided with PAL. It is a TSR that emulates calls to the INT5F
routines on the desktop - support for INT5F is built into
the palmtop ROM.

Run CGAGRAPH once to have it go resident (do this from plain DOS
and on the desktop only. Don't load it resident from within
a Windows DOS shell).

Run it again to remove it from memory.

If you often debug on the desktop, you may consider setting
the CGAGRAPH variable in your AUTOEXEC.BAT. If your CGAGRAPH.COM
is located in C:\PALMTOP\PAL\UTIL\TSR, you would add this line
to your AUTOEXEC.BAT:

  SET CGAGRAPH=C:\PALMTOP\PAL\UTIL\TSR\CGAGRAPH.COM

Note that the variable should include the name of the executable
too, specifying the path alone will not work.

The PalInit(1) function will try to temporarily load CGAGRAPH,
if it senses that INT5F support is not available, and the above
environment variable is set. PalDeInit will in turn unload
CGAGRAPH again. This has the advantage that CGAGRAPH will be
loaded on demand only - it will not take up low memory
otherwise. This behaviour is new in PAL V 1.1, and very handy
for those of us (like me) who develop and debug on the desktop.
Due to DOS limitations, autoload of CGAGRAPH is supported for
small model PAL applications only, unfortunately.
