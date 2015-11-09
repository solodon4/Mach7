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
:: \file
::
:: Script for testing .cou parser
::
:: \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
::
:: \see https://parasol.tamu.edu/mach7/
:: \see https://github.com/solodon4/Mach7
:: \see https://github.com/solodon4/SELL
::
:: Version 1.0 from 12th March 2012
::
:: Usage:
::     all-cou [ filemask*.cou ... ]
::                                                                           

@echo off

if "%1" == "/?" findstr "^::" "%~f0" & goto END

rem couparser.exe must be available via PATH variable
where couparser.exe
if errorlevel 1 echo ERROR: You need to build couparser.exe first with project in ..\parser directory && goto END

set logfile="%~dp0%~n0.log"
echo %date% %time% > %logfile%

if "%1" == "" goto PARSE_ALL

:PARSE_ARG :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

rem Iterate through all the arguments that were passed and build them
for %%i in (%1) do call :SUB_PARSE_FILE "%%i"
shift
if "%1" == "" goto END
goto PARSE_ARG

:PARSE_ALL :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

for %%i in (*.cou) do call :SUB_PARSE_FILE %%i
goto END

:SUB_PARSE_FILE ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

rem Subroutine that parses a particular file
set filename=%~n1
echo Processing %1 ...
echo ======================================== [ %filename%.cpp ] >> %logfile%
rem couparser.exe < %1 > %filename%.dot 2>> %logfile%
if     "%filename%"=="self-system" couparser.exe %1 200 >> %logfile% 2>&1
if not "%filename%"=="self-system" couparser.exe %1 700 >> %logfile% 2>&1
rem couparser.exe %1     >> %logfile% 2>&1
rem g++.exe -Wall -time -O2 -DNDEBUG -std=c++0x -I"C:\Projects\PatternMatching" -o %filename%.exe %filename%*.cpp >> %logfile% 2>&1
rem if exist %filename%.o del /Q %filename%.o

:END
