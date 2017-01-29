rmdir /S deploy
mkdir deploy
set PATH=%PATH%;C:\Qt\Qt5.7.1\5.7\mingw53_32\bin\;C:\Qt\Qt5.7.1\Tools\mingw530_32\bin\
set PATH=%PATH%;C:\Qt\Qt5.5.1\5.5\mingw492_32\bin\;C:\Qt\Qt5.5.1\Tools\mingw492_32\bin\
windeployqt.exe --dir deploy --release --compiler-runtime ..\build-TIATracker-Desktop_Qt_5_5_1_MinGW_32bit-Release\release\TIATracker.exe
copy ..\build-TIATracker-Desktop_Qt_5_5_1_MinGW_32bit-Release\release\TIATracker.exe deploy\
copy ..\build-TIATracker-Desktop_Qt_5_5_1_MinGW_32bit-Release\keymap.cfg deploy\
copy ..\build-TIATracker-Desktop_Qt_5_5_1_MinGW_32bit-Release\license.txt deploy\
copy ..\build-TIATracker-Desktop_Qt_5_5_1_MinGW_32bit-Release\TIATracker_manual.pdf deploy\
copy sdl\windows\lib\SDL2.dll deploy\
mkdir deploy\player
xcopy /S player deploy\player\
mkdir deploy\songs
xcopy /S songs deploy\songs\
mkdir deploy\instruments
xcopy /S instruments deploy\instruments\
mkdir deploy\guides
xcopy /S guides deploy\guides\
