::
:: Script for testing .cou parser
::
:: Written by Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
:: Version 1.0 from 12th March 2012
::
:: Usage:
::     all-cou [ filemask*.cou ... ]
::                                                                           
:: This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
:: Copyright (C) 2005-2012 Texas A&M University.
:: All rights reserved.
:: 

@echo off

if "%1" == "/?" findstr "^::" "%~f0" & goto END

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
if     "%filename%"=="self-system" couparser.exe %1 200 2 >> %logfile% 2>&1
if not "%filename%"=="self-system" couparser.exe %1 700 2 >> %logfile% 2>&1
rem couparser.exe %1     >> %logfile% 2>&1
rem g++.exe -Wall -time -O2 -DNDEBUG -std=c++0x -I"C:\Projects\PatternMatching" -o %filename%.exe %filename%*.cpp >> %logfile% 2>&1
rem if exist %filename%.o del /Q %filename%.o

:END
