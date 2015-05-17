ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                                                ³
³                  Welcome to the PAL library !                  ³
³                                                                ³
³                         Version 1.6                            ³
³                                                                ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
------------------------------------------------------------------

 NOTE: This archive contains subdirectories - if you extracted it
 without the -d switch, delete all the files that were extracted
 with the exception of PAL.ZIP, and try again with:

                       pkunzip -d pal.zip


 NEW: Updates now available via the Internet:
      http://ourworld.compuserve.com/homepages/gilles
      (follow the PAL link)

-----------------------------------------------------------------


What's PAL ?
------------

PAL stands for 'Palmtop Application Library'. PAL is based upon
HPCLIB, a collection of user interface functions developed by
Harry Konstas, and the project was originally suggested as a
common development effort by HPHAND sysop Mark Scardina.
The project is being coordinated by yours truly, Gilles Kohl.
(Email address at the end of this text)

PAL is being discussed in section 15 (Palmtop Programmers) of
the HPHAND forum on CompuServe. HPHAND is the place where you
are likely to find a lot of the answers to your questions on
PAL and HP palmtop programming in general - just ask ! You'll
also find the latest version of PAL in the libraries there.
(See further down for WWW availability)


What exactly is it for ?
------------------------

PAL is meant as a help in developing applications for the
HP100LX and HP200LX palmtops. The idea is to create apps that
look much like the built-in apps, but do not need the developer
pack and LHAPI skills to create. CompuServe and the HPHAND forum
are a great help because:

- it serves as the communication medium for the contributing
programmers (who otherwise have never met in most cases), and
helps provide feedback from potential users to that group of
developers, so that functionality that is really needed by the
user base can be included.

- support is available for PAL in the forum, and problems and
bugs can be reported (and fixed) thru this channel.

- updates are available via my homepage on CompuServe:
  http://ourworld.compuserve.com/homepages/gilles
  (follow the PAL link)


What are the conditions to use it ?
-----------------------------------

Check the file 'COPYRITE.TXT' in the .\DOCS directory for the
legalese. Basically, you may use PAL to develop public domain,
shareware, or commercial applications - no registration fee or
royalties, but you must give credit to the PAL group, and may
not modify the PAL sources themselves for anything else than
contributions.


How can I get it (to work) for my compiler ?
--------------------------------------------

You can use the PM (PalMake) tool to build the PAL libraries for
your compiler. To do so, first install PM as detailed in
.\UTIL\PM\PM.DOC. (This involves modifying PM.CFG so that PM
knows where the sources are and what compiler you are using, and
copying PM.EXE and PM.CFG somewhere into your PATH). Then,
change to the .\SRC directory, and simply enter PM there.

If you have trouble creating or using the libs for your
compiler, drop us a message in the HPHAND forum (section 15:
Palmtop Programmers) on CompuServe. (Be aware that you need an
ANSI compatible C compiler to be able to compile and use PAL)


What files are in this archive ?
--------------------------------

This archive contains the PAL library sources and a number of
peripheral files and utilities. Many directories have a
FILELIST.TXT file that explains what the directory is for and
what files it contains. Watch for these files. In addition, have
a look at the STRUCTUR.TXT file in the .\DOCS subdirectory where
the organization of the project is detailed.



How can I find out what functions are available ?
-------------------------------------------------

Check the DOCS directory - there's informational material there.
In addition, check the *.DOC files in the .\SRC subdirectories -
they describe their respective modules. Finally, have a look at
the .\UTIL\NG and .\UTIL\NG2HTML directories - they contain
utilities to build the Norton Guide resp. HTML hypertext
versions of the PAL documentation. This is probably the best way
to get an overview of what's available and what PAL can do. (To
learn more about Norton Guides if you never heard of them, read
.\UTIL\NG\NGREADME.TXT. HTML is the file format used by
world-wide web browsers - programs such as Mosaic or NetScape
can be used to view it on a variety of machines. PAL also comes
with an HTML viewer for the palmtop - check out .\UTIL\HV)


How can I contribute, what are the requirements ?
-------------------------------------------------

The single most important requirement is a desire to help and
contribute. It helps if you are fluent in C, and log on to
CompuServe often so that you can stay in touch with the rest of
the group. I am the coordinator of the PAL project - to prevent
work from being done twice, please let me know what you intend
to contribute _before_ actually working on it. (I can also
suggest something if you want to help, but aren't sure how).
This is especially important if you intend to modify existing
parts of PAL - somebody is very likely working on then right
now, and coordination is necessary.

In all cases, please send your contributions to me for inclusion
into PAL - do NOT upload them to HPHAND yourself.

You can reach me by posting a message in the "Palmtop
Programmers" section (#15) of the HPHAND forum on CompuServe to
Gilles Kohl, CIS:100114,3146, or by Internet e-mail:
100114.3146@compuserve.com (please notify before sending large
binary email)

 Updates and 'sneak previews' (a nicer word for betas) will
also be made available via my homepage on CompuServe:
http://ourworld.compuserve.com/homepages/gilles (follow the PAL
link)

