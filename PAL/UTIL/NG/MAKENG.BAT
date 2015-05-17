@echo off
REM -----------------------------------------------------------------------
REM
REM         Batchfile to create PAL documentation Norton Guide.
REM         Requires Norton Tools or ExtraHelp from SofSolutions.
REM
REM         If not familiar with Norton Guides, see NGREADME.TXT
REM         for an overview.
REM
REM                                                          GK, 950615
REM -----------------------------------------------------------------------
REM

REM ------------------- User-configurable area ----------------------

REM     Set NG to either "ng" for the original Norton tools, or to "eh"
REM     if you have ExpertHelp.
set NG=ng

REM     Set OPT to set options for your Norton Guide compiler
set OPT=

REM     Set COPYNG if you want to copy your PAL.NG file to the directory
REM     where you keep all your Norton guides.
REM     Leave it empty if you don't need this.
set COPYNG=copy pal.ng f:\ng

REM ---------------- end of user-configurable area ------------------


type ..\..\src\lograph\lograph.doc  >modules.ngs
type ..\..\src\higraph\higraph.doc >>modules.ngs
type ..\..\src\hfn\hfn.doc         >>modules.ngs
type ..\..\src\font\font.doc       >>modules.ngs
type ..\..\src\win\win.doc         >>modules.ngs
type ..\..\src\mnu\mnu.doc         >>modules.ngs
type ..\..\src\dialog\dialog.doc   >>modules.ngs
type ..\..\src\cfg\cfg.doc         >>modules.ngs
type ..\..\src\misc\misc.doc       >>modules.ngs
type ..\..\src\sm_svc\sm_svc.doc   >>modules.ngs
type ..\..\src\ems\ems.doc         >>modules.ngs
type ..\..\src\amgr\amgr.doc       >>modules.ngs
type ..\..\src\filer\filer.doc     >>modules.ngs
type ..\..\src\pck\pck.doc         >>modules.ngs
type ..\..\src\mem\mem.doc         >>modules.ngs
type ..\..\src\sc\sc.doc           >>modules.ngs
%NG%c modules.ngs %OPT%
del modules.ngs >NUL

type ..\pm\pm.doc           >util.ngs
type ..\pde\pde.doc         >>util.ngs
type ..\pfe\pfe.doc         >>util.ngs
type ..\mkbin\mkbin.doc     >>util.ngs
type ..\pcx2img\pcx2img.doc >>util.ngs
type ..\vfn2hfn\vfn2hfn.doc >>util.ngs
type ..\patch\patch.doc     >>util.ngs
type ..\emm200\emm200.doc   >>util.ngs
%NG%c util.ngs %OPT%
del util.ngs >NUL

copy ..\..\docs\overview.txt overview.ngs >NUL
%NG%c overview.ngs %OPT%
del overview.ngs >NUL

copy ..\..\docs\copyrite.txt copyrite.ngs >NUL
%NG%c copyrite.ngs %OPT%
del copyrite.ngs >NUL

copy ..\..\src\inc\pal.h      pal_h.ngs >NUL
%NG%c pal_h.ngs %OPT%
del pal_h.ngs >NUL

copy ..\..\src\inc\palkeys.h      palkey_h.ngs >NUL
%NG%c palkey_h.ngs %OPT%
del palkey_h.ngs >NUL

copy ..\..\src\inc\palsmsvc.h palsm_h.ngs >NUL
%NG%c palsm_h.ngs %OPT%
del palsm_h.ngs >NUL

copy ..\..\src\inc\paldlg.h paldlg_h.ngs >NUL
%NG%c paldlg_h.ngs %OPT%
del paldlg_h.ngs >NUL

copy ..\..\src\inc\palpriv.h  palprv_h.ngs >NUL
%NG%c palprv_h.ngs %OPT%
del palprv_h.ngs >NUL

copy ..\..\src\inc\palfiler.h  palflr_h.ngs >NUL
%NG%c palflr_h.ngs %OPT%
del palflr_h.ngs >NUL

ren *.ngo *.eho
%NG%ml pal.ngl %OPT%
%COPYNG%
del *.eho >NUL

