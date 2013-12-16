@echo off
for /D %%f in (*) do for %%i in (%%f\*.vcxproj) do call :SUB_RENAME "%%i"
goto END

:SUB_RENAME
set filename=%~n1
if "%filename:~-4%" == "2012" goto END
if "%filename:~-4%" == "2010" goto END
if "%filename:~-4%" == "2008" goto END
if "%filename:~-4%" == "2005" goto END
if "%filename:~-4%" == "2003" goto END
echo %1 will be renamed
rename %~p1%filename%.vcxproj         %filename%.2012.vcxproj
rename %~p1%filename%.vcxproj.filters %filename%.2012.vcxproj.filters
rename %~p1%filename%.vcxproj.user    %filename%.2012.vcxproj.user
goto END

:END
