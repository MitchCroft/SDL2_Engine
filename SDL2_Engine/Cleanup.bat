rem Delete the Visual Studio Database file
del /q /s ".\*.db"

rem Delete the Extracted library folder
RD /q /s ".\SDL2_Engine\"

rem Clear the Intermidiary folder
RD /q /s ".\int\"

rem Clean the project Bins
RD /q /s ".\Projects\SDL2_Engine\bin\"

rem Clean the rebuildable project dependencies
RD /q /s ".\dep\SDL2_Engine\"

rem Clean the Bomb Squad project
del /s /q ".\Projects\BombSquad\bin\exes\*.exe"
del /s /q ".\Projects\BombSquad\bin\exes\*.iobj"
del /s /q ".\Projects\BombSquad\bin\exes\*.ipdb"
del /s /q ".\Projects\BombSquad\bin\exes\*.pdb"
del /s /q ".\Projects\BombSquad\bin\exes\*.ilk"
del /s /q ".\Projects\BombSquad\bin\exes\Win32\SDL2_Engine.dll"
del /s /q ".\Projects\BombSquad\bin\exes\Win32\SDL2_Engine_d.dll"
del /s /q ".\Projects\BombSquad\bin\exes\x64\SDL2_Engine.dll"
del /s /q ".\Projects\BombSquad\bin\exes\x64\SDL2_Engine_d.dll"