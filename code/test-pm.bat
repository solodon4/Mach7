::
:: Script for testing C++ pattern matching library
::
:: Written by Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
:: Version 1.0 from 30th April 2011
::
:: Usage: test-pm.bat
::                                                                           
:: This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
:: Copyright (C) 2005-2011 Texas A&M University.
:: All rights reserved.
:: 

@echo off

if "%1" == "/?" findstr "^::" "%~f0" & goto END

rem Prepare log file :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
call :SUB_PARSE_DATE
set logfile=test-pm-%yy%-%mm%-%dd%-%hh%-%mn%.log
echo. > %logfile%
echo C++ Pattern Matching Test Script >> %logfile%
echo Version 1.0 from 2010-11-18 >> %logfile%
echo. >> %logfile%
echo Test from %date% at %time% >> %logfile%

set OPTIONS=-I"%BOOST%" -Wall -O3 -DNDEBUG -std=c++0x -DNO_RANDOMIZATION

if "%1" == "" goto TEST_ALL

:TEST_COMBINATION

g++ %OPTIONS% -DNUMBER_OF_DERIVED=%1 -DNUMBER_OF_VFUNCS=%2 -osynthetic_select_random-%1-%2.exe synthetic_select_random.cpp >> %logfile% 2>&1
echo =========================================================================== >> %logfile%
synthetic_select_random-%1-%2.exe > synthetic_select_random-%1-%2.txt
FOR /f "tokens=*" %%G IN ('find "OVERALL:" ^< synthetic_select_random-%1-%2.txt') DO FOR /f "tokens=*" %%H IN ('find "VTBLS:" ^< synthetic_select_random-%1-%2.txt') DO echo Classes: %1	Vfuncs: %2	[%%G]	[%%H]

goto END

:TEST_ALL

FOR /L %%I IN (100,10,100) DO FOR /L %%J IN (0,5,100) DO call :TEST_COMBINATION %%I %%J

:SUB_PARSE_DATE ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

rem The following code has been derived from: 
rem  - Date Parsing: http://ss64.com/nt/syntax-getdate.html
rem  - Time Parsing: http://ss64.com/nt/syntax-gettime.html

setlocal
for /f "tokens=1-4 delims=/-. " %%G in ('date /t') do (call :SUB_FIXDATE %%G %%H %%I %%J)
for /f "tokens=1-3 delims=1234567890 " %%a in ("%time%") do set "delims=%%a%%b%%c"
for /f "tokens=1-4 delims=%delims%" %%G in ("%time%") do (
  set hh=%%G
  set mn=%%H
  set ss=%%I
  set ms=%%J
)
rem Strip any leading spaces
set hh=%hh: =%
rem Ensure the hours have a leading zero
if 1%hh% LSS 20 Set hh=0%hh%
endlocal&set mm=%mm%&set dd=%dd%&set yy=%yy%&set hh=%hh%&set mn=%mn%&set ss=%ss%&set ms=%ms%
goto END

:SUB_FIXDATE :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

if "%1:~0,1%" GTR "9" shift
for /f "skip=1 tokens=2-4 delims=(-)" %%G in ('echo.^|date') do (
    set %%G=%1&set %%H=%2&set %%I=%3)
goto END

:END
