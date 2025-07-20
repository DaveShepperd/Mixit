# mixit
This is a very old tool use by Atari programmers to make ROM/EPROM images. The command syntax is a bit obscure and clumsy. See the mixit.html file for details on how to use it.

I didn't write it but have made several fixes and added features to it over the years. Presently it has only been built and tested on Linux systems and under 32 and 64 bit MinGW systems on Windows. The last test was on Windows 11.
I can only indicate it will build okay using with the gcc compiler. Version 6.3.0 on Mingw32 and version 15.1.0 under Msys2 (both 32 and 64 bit). It was developed first under VAX/VMS using Dec C then ported
to a bunch of different systems (Sun, SGI, etc.)

So use a Makefile that might be appropriate for your system and good luck. One sticking point might be with the inclusion of the readline library which I added on the Linux build
to get command line edit and history. Just don't set the USE_READLINE cpp variable non-zero to omit it.
