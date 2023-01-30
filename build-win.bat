if not exist "build-win" mkdir "build-win"
cl /nologo -Zi /I C:\dev\SDL2-2.0.22\include src\main.c shell32.lib SDL2.lib SDL2main.lib /link /LIBPATH:C:\dev\SDL2-2.0.22\lib\x64 /SUBSYSTEM:CONSOLE /out:build-win\main.exe
