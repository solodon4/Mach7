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
:: Usage: test-pm-timing.bat
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

set logfile=test-pm-timing.log
set results=test-pm-timing.txt

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

echo [ MS Visual C++/Win32 ] ========================= >> %logfile%

rem Set up VC variables for x86 build
call "%VCINSTALLDIR%vcvarsall.bat" x86

call :COMPILE 1 p REP 32
call :COMPILE 1 p SEQ 32
call :COMPILE 1 p RND 32
call :COMPILE 1 f REP 32
call :COMPILE 1 f SEQ 32
call :COMPILE 1 f RND 32
call :COMPILE 1 P REP 32
call :COMPILE 1 P SEQ 32
call :COMPILE 1 P RND 32
call :COMPILE 1 F REP 32
call :COMPILE 1 F SEQ 32
call :COMPILE 1 F RND 32
call :COMPILE 0 p REP 32
call :COMPILE 0 p SEQ 32
call :COMPILE 0 p RND 32
call :COMPILE 0 k REP 32
call :COMPILE 0 k SEQ 32
call :COMPILE 0 k RND 32
call :COMPILE 0 P REP 32
call :COMPILE 0 P SEQ 32
call :COMPILE 0 P RND 32
call :COMPILE 0 K REP 32
call :COMPILE 0 K SEQ 32
call :COMPILE 0 K RND 32

echo [ MS Visual C++/x64 ] ========================= >> %logfile%

rem Set up VC variables for x64 build
call "%VCINSTALLDIR%vcvarsall.bat" x64

call :COMPILE 1 p REP 64
call :COMPILE 1 p SEQ 64
call :COMPILE 1 p RND 64
call :COMPILE 1 f REP 64
call :COMPILE 1 f SEQ 64
call :COMPILE 1 f RND 64
call :COMPILE 1 P REP 64
call :COMPILE 1 P SEQ 64
call :COMPILE 1 P RND 64
call :COMPILE 1 F REP 64
call :COMPILE 1 F SEQ 64
call :COMPILE 1 F RND 64
call :COMPILE 0 p REP 64
call :COMPILE 0 p SEQ 64
call :COMPILE 0 p RND 64
call :COMPILE 0 k REP 64
call :COMPILE 0 k SEQ 64
call :COMPILE 0 k RND 64
call :COMPILE 0 P REP 64
call :COMPILE 0 P SEQ 64
call :COMPILE 0 P RND 64
call :COMPILE 0 K REP 64
call :COMPILE 0 K SEQ 64
call :COMPILE 0 K RND 64

rem echo Running timings ...
rem for %%i in (time-*.exe) do echo %%i & echo ==================[ %%i ]===================  >> %results% & %%i >> %results% 2>&1 

goto END

:COMPILE

if "%1" == "1" (set F=fwd) ELSE (set F=non)
if "%2" == "p"  set S=generic& set E=poly
if "%2" == "P"  set S=special& set E=poly
if "%2" == "k"  set S=generic& set E=kind
if "%2" == "K"  set S=special& set E=kind
if "%2" == "f"  set S=generic& set E=knfw
if "%2" == "F"  set S=special& set E=knfw
if "%4" == "32" set M=X86
if "%4" == "64" set M=X64
set filename=%4-%F%-%S%-%E%-%3
<nul (set/p output=Working on time-%filename%.exe: ) 
<nul (set/p output=- compiling ) 
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=%1 /DXTL_DEFAULT_SYNTAX='%2' /DXTL_%3_TEST skeleton.cxx /Fotime-%filename%.obj     /Fetime-%filename%.exe     /link %LNKFLAGS% /MACHINE:%M% >> %logfile% 2>&1
<nul (set/p output=- instrumenting )
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=%1 /DXTL_DEFAULT_SYNTAX='%2' /DXTL_%3_TEST skeleton.cxx /Fotime-pgo-%filename%.obj /Fetime-pgo-%filename%.exe /link %LNKFLAGS% /MACHINE:%M% /PGD:"time-pgo-%filename%.pgd" /LTCG:PGINSTRUMENT >> %logfile% 2>&1
<nul (set/p output=- profiling )
time-pgo-%filename%.exe > nul
<nul (set/p output=- optimizing )
link %LNKFLAGS% /MACHINE:%M% /PGD:"time-pgo-%filename%.pgd" time-pgo-%filename%.obj /LTCG:PGOPTIMIZE >> %logfile% 2>&1
rem del time-%filename%.obj time-%filename%.pdb time-pgo-%filename%.obj time-pgo-%filename%.pdb time-pgo-%filename%.pgd time-pgo-%filename%*.pgc
echo.

:END
