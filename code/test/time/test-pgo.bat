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
:: Version 1.0 from 21st July 2011
::
:: Usage: test-pgo.bat filename.cpp ...
::

@echo off

if "%1" == "/?" findstr "^::" "%~f0" & goto END

if not "%VS140COMNTOOLS%" == "" call "%VS140COMNTOOLS%vsvars32.bat" && echo Using MS Visual C++ 14.0 && goto PROCEED
if not "%VS120COMNTOOLS%" == "" call "%VS120COMNTOOLS%vsvars32.bat" && echo Using MS Visual C++ 12.0 && goto PROCEED
if not "%VS110COMNTOOLS%" == "" call "%VS110COMNTOOLS%vsvars32.bat" && echo Using MS Visual C++ 11.0 && goto PROCEED
if not "%VS100COMNTOOLS%" == "" call "%VS100COMNTOOLS%vsvars32.bat" && echo Using MS Visual C++ 10.0 && goto PROCEED
if not "%VS90COMNTOOLS%"  == "" call "%VS90COMNTOOLS%vsvars32.bat"  && echo Using MS Visual C++ 9.0  && goto PROCEED
if not "%VS80COMNTOOLS%"  == "" call "%VS80COMNTOOLS%vsvars32.bat"  && echo Using MS Visual C++ 8.0  && goto PROCEED
if not "%VS71COMNTOOLS%"  == "" call "%VS71COMNTOOLS%vsvars32.bat"  && echo Using MS Visual C++ 7.1  && goto PROCEED

echo ERROR: Unable to find installation of MS Visual C++ on this computer
goto END

:PROCEED

set logfile=test-pgo.log
set results=test-pgo.txt

echo Test from %date% at %time% > %logfile%
echo Test from %date% at %time% > %results%

set CXX=cl.exe
rem /Zi /nologo /W3 /WX- /O2 /Ob2 /Oi /Ot /Oy- /GL  /GF /Gm- /MT /GS- /Gy- /fp:precise /Zc:wchar_t /Zc:forScope /Gr /analyze- /errorReport:queue 
rem set CXXFLAGS=/W4 /O2 /Ob2 /Oi /Ot /Og /GR /GL /GF /GS- /Gy- /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /I%BOOST_ROOT%
rem /INCREMENTAL:NO /NOLOGO /SUBSYSTEM:CONSOLE /OPT:REF /OPT:ICF /LTCG /TLBID:1 /DYNAMICBASE:NO /NXCOMPAT /MACHINE:X86 /ERRORREPORT:QUEUE 
rem set LNKFLAGS=/LTCG /MACHINE:X86

set CXXFLAGS=/I../.. /I%BOOST_ROOT% /Zi /nologo /W3 /WX- /O2 /Ob2 /Oi /Ot /Oy- /GL /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /GF /Gm- /MT /GS- /Gy- /fp:precise /Zc:wchar_t /Zc:forScope /Gr /analyze- /errorReport:queue 
rem Slower: set CXXFLAGS=/I%BOOST_ROOT% /Zi /nologo /W3 /WX- /O2 /Ob2 /Oi /Ot      /GL /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /GF /Gm- /MT /GS- /Gy  /fp:precise /Zc:wchar_t /Zc:forScope /Gr           /errorReport:queue 
set LNKFLAGS=/INCREMENTAL:NO /NOLOGO "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib" /MANIFEST:NO /ALLOWISOLATION /SUBSYSTEM:CONSOLE /OPT:REF /OPT:ICF /LTCG /TLBID:1 /DYNAMICBASE:NO /NXCOMPAT /ERRORREPORT:QUEUE 

rem Set up VC variables for x86 build
call "%VCINSTALLDIR%vcvarsall.bat" x86
set M=X86

rem Set up VC variables for x64 build
rem call "%VCINSTALLDIR%vcvarsall.bat" x64
rem set M=X64

:TEST_ARG
for %%i in (%1) do call :COMPILE "%%i"
shift
if "%1" == "" goto END
goto TEST_ARG

:COMPILE

set filename=%~n1
<nul (set/p output=Working on %filename%: ) 
<nul (set/p output=- compiling ) 
%CXX% %CXXFLAGS% %1 /Fo%filename%.obj /Fe%filename%.exe /link %LNKFLAGS% /MACHINE:%M% >> %logfile% 2>&1
<nul (set/p output=- instrumenting )
%CXX% %CXXFLAGS% %1 /Fo%filename%-pgo.obj /Fe%filename%-pgo.exe /link %LNKFLAGS% /MACHINE:%M% /PGD:"%filename%-pgo.pgd" /LTCG:PGINSTRUMENT >> %logfile% 2>&1
<nul (set/p output=- profiling )
%filename%-pgo.exe > nul
<nul (set/p output=- optimizing )
link %LNKFLAGS% /MACHINE:%M% /PGD:"%filename%-pgo.pgd" %filename%-pgo.obj /LTCG:PGOPTIMIZE >> %logfile% 2>&1
rem del %filename%.obj %filename%.pdb %filename%-pgo.obj %filename%-pgo.pdb %filename%-pgo.pgd %filename%-pgo*.pgc
echo.

:END
