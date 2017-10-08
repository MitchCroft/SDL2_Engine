rem Copy all of the header files into an include folder
XCOPY ".\Projects\SDL2_Engine\src\*.hpp" ".\SDL2_Engine\include\" /f /d /s

rem Copy the release builds for Win32 projects
XCOPY ".\Projects\SDL2_Engine\bin\Win32\Release\*.dll" ".\SDL2_Engine\lib\Win32\" /f /d
XCOPY ".\Projects\SDL2_Engine\bin\Win32\Release\*.lib" ".\SDL2_Engine\lib\Win32\" /f /d

rem Copy the release builds for x64 projects
XCOPY ".\Projects\SDL2_Engine\bin\x64\Release\*.dll" ".\SDL2_Engine\lib\x64\" /f /d
XCOPY ".\Projects\SDL2_Engine\bin\x64\Release\*.lib" ".\SDL2_Engine\lib\x64\" /f /d

pause