rem Delete the Visual Studio Database file
del /q /s ".\*.db"

rem Delete the Extracted library folder
RD /q /s ".\SDL2_Engine\"

rem Clean the SDL2_Engine project
RD /q /s ".\Projects\SDL2_Engine\bin\"
RD /q /s ".\Projects\SDL2_Engine\int\"

rem Clean the TestingGrounds project
RD /q /s ".\Projects\TestingGrounds\int\"
RD /q /s ".\Projects\TestingGrounds\dep\SDL2_Engine\"

del /s /q ".\Projects\TestingGrounds\bin\exes\*.exe"
del /s /q ".\Projects\TestingGrounds\bin\exes\*.iobj"
del /s /q ".\Projects\TestingGrounds\bin\exes\*.ipdb"
del /s /q ".\Projects\TestingGrounds\bin\exes\*.pdb"
del /s /q ".\Projects\TestingGrounds\bin\exes\*.ilk"