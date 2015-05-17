@echo off
REM
REM  This batch file converts all VFN files in the current directory into
REM  HFN files for use with PAL.
REM

for %%i in (*.vfn) do vfn2hfn %%i 1

