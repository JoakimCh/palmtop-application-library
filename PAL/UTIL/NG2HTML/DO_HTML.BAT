@echo off
pm
ng2html over.lst over.htm "Palmtop Application Library"
ng2html pal.lst pal.htm "PAL - Quickreference"
echo About to launch HV, press Ctrl-C to quit
pause
..\hv\hv overview.htm
