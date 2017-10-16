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

del /s /q ".\Projects\TestingGrounds\bin\exes\*.exe"
del /s /q ".\Projects\TestingGrounds\bin\exes\*.iobj"
del /s /q ".\Projects\TestingGrounds\bin\exes\*.ipdb"
del /s /q ".\Projects\TestingGrounds\bin\exes\*.pdb"
del /s /q ".\Projects\TestingGrounds\bin\exes\*.ilk"