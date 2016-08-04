set INCLUDE=C:\WinDDK\7600.16385.1\inc\api;C:\WinDDK\7600.16385.1\inc\crt;%INCLUDE%
set LIB=C:\WinDDK\7600.16385.1\lib\wxp\i386;C:\WinDDK\7600.16385.1\lib\Crt\i386;%LIB%
set PATH=C:\WinDDK\7600.16385.1\bin\x86\x86;%PATH%




cl /MD /O2 /c /GL /Oy- /Oi /Gm- /DNDEBUG /DWIN32 /D_WINDOWS /D_WIN32_WINNT=0x0501 *.cpp *.c
link /NOLOGO /SUBSYSTEM:CONSOLE /release /OUT:qb_keygen.exe /INCREMENTAL:NO user32.lib advapi32.lib msvcrt_winxp.obj *.obj

pause

