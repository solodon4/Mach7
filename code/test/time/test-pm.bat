::
::  Mach7: Pattern Matching Library for C++
::
::  Copyright 2011-2013, Texas A&M University.
::  Copyright 2014 Yuriy Solodkyy.
::  All rights reserved.
::
::  Redistribution and use in source and binary forms, with or without
::  modification, are permitted provided that the following conditions are met:
::
::      * Redistributions of source code must retain the above copyright
::        notice, this list of conditions and the following disclaimer.
::
::      * Redistributions in binary form must reproduce the above copyright
::        notice, this list of conditions and the following disclaimer in the
::        documentation and/or other materials provided with the distribution.
::
::      * Neither the names of Mach7 project nor the names of its contributors
::        may be used to endorse or promote products derived from this software
::        without specific prior written permission.
::
::  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
::  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
::  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
::  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY
::  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
::  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
::  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
::  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
::  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
::  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

::
:: Script for testing C++ pattern matching library
::
:: \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
::
:: \see https://parasol.tamu.edu/mach7/
:: \see https://github.com/solodon4/Mach7
:: \see https://github.com/solodon4/SELL
::
:: Version 1.0 from 30th April 2011
::
:: Usage: test-pm.bat
::                                                                           

@echo off

if "%1" == "/?" findstr "^::" "%~f0" & goto END

rem Prepare log file :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
call :SUB_PARSE_DATE
set logfile=test-pm-%yy%-%mm%-%dd%-%hh%-%mn%.log
echo. > %logfile%
echo C++ Pattern Matching Test Script >> %logfile%
echo Version 1.0 from 2011-04-30 >> %logfile%
echo. >> %logfile%
echo Test from %date% at %time% >> %logfile%

set OPTIONS=-I"%BOOST_ROOT%" -Wall -O3 -DNDEBUG -std=c++0x -DNO_RANDOMIZATION

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
