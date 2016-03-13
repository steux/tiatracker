rmdir /S deploy
mkdir deploy
set PATH=%PATH%;C:\Qt\Qt5.5.1\5.5\mingw492_32\bin\;C:\Qt\Qt5.5.1\Tools\mingw492_32\bin\
windeployqt.exe --dir deploy --release --compiler-runtime ..\build-TIATracker-Desktop_Qt_5_5_1_MinGW_32bit-Release\release\TIATracker.exe
copy ..\build-TIATracker-Desktop_Qt_5_5_1_MinGW_32bit-Release\release\TIATracker.exe deploy\
copy ..\build-TIATracker-Desktop_Qt_5_5_1_MinGW_32bit-Release\keymap.cfg deploy\
copy ..\build-TIATracker-Desktop_Qt_5_5_1_MinGW_32bit-Release\license.txt deploy\
copy sdl\windows\lib\SDL2.dll deploy\
mkdir deploy\player
xcopy /S player deploy\player\
