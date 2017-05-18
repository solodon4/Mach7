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
:: Script for building and testing C++ pattern matching library
::
:: \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
::
:: \see https://parasol.tamu.edu/mach7/
:: \see https://github.com/solodon4/Mach7
:: \see https://github.com/solodon4/SELL
::
:: Version 1.0 from 4th February 2012
::
:: Usage:
::
::     build [ pgo | repro | tmp | <ver> | <arch> ] [ filemask*.cpp ... ]
::     build        - Build all examples using the most recent MS Visual C++ compiler installed
::     build unit   - Build all unit tests
::     build syntax - Build all supported library options combination for syntax variations
::     build timing - Build all supported library options combination for timing variations
::     build cmp    - Build all executables for comparison with other languages
::     build doc    - Build Mach7 documentation
::     build clean  - Clean all built examples
::     build test   -   Run all built examples
::     build check  -   Run those examples for which there are correct_output/*.out files and check that output is the same
::
:: Modifiers:
::           analyze- Run compiler with /analyze option to perform static analysis
::           pgo    - Perform Profile-Guided Optimization on produced executables
::           repro  - In case of error, create and compile a pre-processed repro
::           tmp    - Keep temporaries
::           <ver>  - Use a specific version of Visual C++ to compiler the source 
::                    code. <ver> can be one of the following:
::                     - 2017 - Visual C++ 15.0
::                     - 2015 - Visual C++ 14.0
::                     - 2013 - Visual C++ 12.0
::                     - 2012 - Visual C++ 11.0
::                     - 2010 - Visual C++ 10.0
::                     - 2008 - Visual C++  9.0
::                     - 2005 - Visual C++  8.0
::                     - 2003 - Visual C++  7.1
::                       0000 - Do not use any VS to set up the environment, I will set it up by myself
::           <arch> - Target architecture. Can be one of the following: x86, x64, arm
::                                                                           

@echo off
set MACH7_ROOT=%~dp0
set CurDir=%CD%
set /A "result=0"
setlocal
if "%1" == "/?" findstr "^::" "%~f0" & goto END

rem Reset ERRORLEVEL in case previous command failed
verify > nul

rem Prepare log file :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

rem We would like to have the toolset directly in the log file name, so we duplicate the later check here
if "%1" == ""     set VS20XX=2017
if "%1" == "2017" set VS20XX=2017
if "%1" == "2015" set VS20XX=2015
if "%1" == "2013" set VS20XX=2013
if "%1" == "2012" set VS20XX=2012
if "%1" == "2010" set VS20XX=2010
if "%1" == "2008" set VS20XX=2008
if "%1" == "2005" set VS20XX=2005
if "%1" == "2003" set VS20XX=2003
if "%1" == "0000" set VS20XX=0000

rem Specific invokations of this script write log to common build.log file
rem if not "%1" == "" set logfile=build-VS%VS20XX%.log&goto LOG_FILE_READY
rem Build of everythings gets its own time-stamped log file
call :SUB_PARSE_DATE 
set logfile=build-%yy%-%mm%-%dd%-%hh%-%mn%-VS%VS20XX%.log

:LOG_FILE_READY
set logfile="%MACH7_ROOT%%logfile%"
echo. > %logfile%
echo Mach7 Build Script >> %logfile%
echo Version 1.0.89 from 2016-08-24 >> %logfile%
echo. >> %logfile%
echo Build log from %date% at %time% >> %logfile%
echo Command line: %0 %* >> %logfile%

:PARSE_CMD_LINE ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

rem Parse modifiers

if /I "%1" == "analyze"   shift && set CL=/analyze /analyze:plugin "%MACH7_ROOT%packages\Microsoft.CppCoreCheck\build\native\%ARCH%\EspXEngine.dll" %CL%&& goto PARSE_CMD_LINE
if /I "%1" == "pgo"       shift && set PGO=1&&                          goto PARSE_CMD_LINE
if /I "%1" == "repro"     shift && set REPRO=1&&                        goto PARSE_CMD_LINE
if /I "%1" == "tmp"       shift && set KEEP_TMP=1&&                     goto PARSE_CMD_LINE
if    "%1" == "2017"      shift && set VS_COMN_TOOLS=%VS150COMNTOOLS%&& goto PARSE_CMD_LINE
if    "%1" == "2015"      shift && set VS_COMN_TOOLS=%VS140COMNTOOLS%&& goto PARSE_CMD_LINE
if    "%1" == "2013"      shift && set VS_COMN_TOOLS=%VS120COMNTOOLS%&& goto PARSE_CMD_LINE
if    "%1" == "2012"      shift && set VS_COMN_TOOLS=%VS110COMNTOOLS%&& goto PARSE_CMD_LINE
if    "%1" == "2010"      shift && set VS_COMN_TOOLS=%VS100COMNTOOLS%&& goto PARSE_CMD_LINE
if    "%1" == "2008"      shift && set VS_COMN_TOOLS=%VS90COMNTOOLS%&&  goto PARSE_CMD_LINE
if    "%1" == "2005"      shift && set VS_COMN_TOOLS=%VS80COMNTOOLS%&&  goto PARSE_CMD_LINE
if    "%1" == "2003"      shift && set VS_COMN_TOOLS=%VS71COMNTOOLS%&&  goto PARSE_CMD_LINE
if    "%1" == "0000"      shift && set VS_COMN_TOOLS=0000&&             goto PARSE_CMD_LINE
if /I "%1" == "x86"       shift && set ARCH=x86&&                       goto PARSE_CMD_LINE
if /I "%1" == "x64"       shift && set ARCH=x64&&                       goto PARSE_CMD_LINE
if /I "%1" == "arm"       shift && set ARCH=arm&&                       goto PARSE_CMD_LINE

rem Parse commands

if /I "%1" == "clean"     call :SUB_CLEAN "%MACH7_ROOT%" && call :SUB_CLEAN "%MACH7_ROOT%test\unit" && call :SUB_CLEAN "%MACH7_ROOT%test\time" & goto END
if /I "%1" == "check"     shift && goto CHECK
if /I "%1" == "test"      shift && goto TEST
if /I "%1" == "doc"       shift && goto DOXYGEN
if /I "%1" == "make"      shift && call %1 %2 %3 %4 %5 %6 %7 %8 %9& goto END
if /I "%1" == "cmake"     shift && goto BUILD_WITH_CMAKE

rem Subsequent commands require Visual C++ environment variables to be set up.

if not "%VS_COMN_TOOLS%" == "" goto PROCEED

rem Detect most recent version of the Visual C++ installed :::::::::::::::::::::

if not "%VS150COMNTOOLS%" == "" set VS_COMN_TOOLS=%VS150COMNTOOLS%&& goto PROCEED
if not "%VS140COMNTOOLS%" == "" set VS_COMN_TOOLS=%VS140COMNTOOLS%&& goto PROCEED
if not "%VS120COMNTOOLS%" == "" set VS_COMN_TOOLS=%VS120COMNTOOLS%&& goto PROCEED
if not "%VS110COMNTOOLS%" == "" set VS_COMN_TOOLS=%VS110COMNTOOLS%&& goto PROCEED
if not "%VS100COMNTOOLS%" == "" set VS_COMN_TOOLS=%VS100COMNTOOLS%&& goto PROCEED
if not "%VS90COMNTOOLS%"  == "" set VS_COMN_TOOLS=%VS90COMNTOOLS%&&  goto PROCEED
if not "%VS80COMNTOOLS%"  == "" set VS_COMN_TOOLS=%VS80COMNTOOLS%&&  goto PROCEED
if not "%VS71COMNTOOLS%"  == "" set VS_COMN_TOOLS=%VS71COMNTOOLS%&&  goto PROCEED

echo ERROR: Unable to find installation of MS Visual C++ on this computer
goto END

:PROCEED :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

set CXX=cl.exe
set MACH7_INC=/I %MACH7_ROOT%
if not "%BOOST_ROOT%" == "" set MACH7_INC=%MACH7_INC% /I %BOOST_ROOT% /DHAS_BOOST
rem List of compiler options: http://technet.microsoft.com/en-us/library/fwkeyyhe(v=vs.110).aspx
rem NOTE: Specifying /GL in VC11 fails to link code that uses our decl_helper for some reason.
rem       However not specifying /GL fails when trying to do PGO.
rem       Linker's /LTCG is used only when /GL is passed and vice versa.
rem set CXXFLAGS=/nologo /W4 /EHsc /O2
set CXXFLAGS=%MACH7_INC% /wd4007 /Zi /nologo /EHsc /W4 /WX- /O2 /Ob2 /Oi /Ot /Oy-     /GF /Gm- /MT /GS- /Gy- /fp:precise /Zc:wchar_t /Zc:forScope /Gr /errorReport:queue /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" 
rem          %MACH7_INC% /wd4007 /Zi /nologo       /W3 /WX- /O2 /Ob2 /Oi /Ot /Oy- /GL /GF /Gm- /MT /GS- /Gy- /fp:precise /Zc:wchar_t /Zc:forScope /Gr /errorReport:queue /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FU"C:\Program Files (x86)\Reference Assemblies\Microsoft\Framework\.NETFramework\v4.0\System.Core.dll" /Fp"Release\SyntheticSelectRandom.pch" /Fa"Release\" /Fo"Release\" /Fd"Release\vc100.pdb" 
rem Slower: =%MACH7_INC% /wd4007 /Zi /nologo       /W3 /WX- /O2 /Ob2 /Oi /Ot      /GL /GF /Gm- /MT /GS- /Gy  /fp:precise /Zc:wchar_t /Zc:forScope /Gr /errorReport:queue /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" 
set LNKFLAGS=
rem LNKFLAGS=/INCREMENTAL:NO /NOLOGO "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib" /MANIFEST:NO /ALLOWISOLATION /SUBSYSTEM:CONSOLE /OPT:REF /OPT:ICF /TLBID:1 /DYNAMICBASE:NO /NXCOMPAT /ERRORREPORT:QUEUE 
rem /OUT:"C:\Projects\PatternMatching\Release\SyntheticSelectRandom.exe" /INCREMENTAL:NO /NOLOGO "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib" /MANIFEST /ManifestFile:"Release\SyntheticSelectRandom.exe.intermediate.manifest" /MANIFESTUAC:"level='asInvoker' uiAccess='false'" /DEBUG /PDB:"C:\Projects\PatternMatching\Release\SyntheticSelectRandom.pdb" /SUBSYSTEM:CONSOLE /OPT:REF /OPT:ICF /PGD:"C:\Projects\PatternMatching\Release\SyntheticSelectRandom.pgd" /LTCG /TLBID:1 /DYNAMICBASE:NO /NXCOMPAT /MACHINE:%ARCH% /ERRORREPORT:QUEUE 

if "%ARCH%" == "" set ARCH=x86
set M=%ARCH%

rem Recent versions of vsvars32.bat do not print the toolset used.
if "%VS_COMN_TOOLS%"=="%VS150COMNTOOLS%" echo Setting environment for using Microsoft Visual Studio 2017 %ARCH% tools.
if "%VS_COMN_TOOLS%"=="%VS140COMNTOOLS%" echo Setting environment for using Microsoft Visual Studio 2015 %ARCH% tools.
if "%VS_COMN_TOOLS%"=="%VS120COMNTOOLS%" echo Setting environment for using Microsoft Visual Studio 2013 %ARCH% tools.
if "%VS_COMN_TOOLS%"=="%VS110COMNTOOLS%" echo Setting environment for using Microsoft Visual Studio 2012 %ARCH% tools.
if "%VS_COMN_TOOLS%"=="%VS100COMNTOOLS%" echo Setting environment for using Microsoft Visual Studio 2010 %ARCH% tools.

rem Set-up Visual C++ Environment Variables
if not "%VS_COMN_TOOLS%"=="0000" call "%VS_COMN_TOOLS%..\..\VC\vcvarsall.bat" %ARCH% >> %logfile% 2>&1
where cl.exe > nul 2>&1
if errorlevel 1 if /I %ARCH% == x64 set ARCH=x86_amd64& echo Warning: Unable to find x64 native toolset. Trying x86 to x64 cross compiler & goto proceed
if errorlevel 1 verify > nul & echo Error: There was a problem setting up the environment for this toolset & goto end
rem Dump environment variables
set >> %logfile% 2>&1
rem Dump versions of compiler passes
%CXX% /Bv >> %logfile% 2>&1
rem Used by CppCoreCheck to not report warnings in system folders
set CAExcludePath=%INCLUDE%

rem Account for a problem with some PGO flags described above.
if "%PGO%"=="1" set CXXFLAGS=%CXXFLAGS% /GL 
if "%PGO%"=="1" set LNKFLAGS=%LNKFLAGS% /LTCG

setlocal

if /I "%1" == "unit"   shift && goto BUILD_UNIT
if /I "%1" == "timing" shift && goto BUILD_TIMING
if /I "%1" == "cmp"    shift && goto BUILD_CMP
if /I "%1" == "syntax" shift && goto BUILD_SYNTAX
if    "%1" == ""                goto BUILD_ALL

:BUILD_ARG :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

rem Iterate through all the arguments that were passed and build them
for %%i in (%1) do call :SUB_BUILD_FILE "%%i"
shift
if "%1" == "" echo There were %result% errors & exit /B %result%
goto BUILD_ARG

:BUILD_ALL :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

rem User didn't provide any arguments, assume all .cpp files in the current folder
call :BUILD_UNIT
call :BUILD_SYNTAX
call :BUILD_CMP
call :BUILD_TIMING
call :CHECK_ALL
echo Build log has been saved to %logfile%
echo There were %result% errors
exit /B %result%

:SUB_BUILD_FILE ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

rem Subroutine that builds a particular file
if "%2" == "" (set filename=%~n1) else (set filename=%2)
echo ======================================== [ %filename%.exe ] >> %logfile%
<nul (set/p output=Working on %filename%.exe	) 
if exist %filename%.exe (
    <nul (set/p output=- exe exist ) 
) else (
    <nul (set/p output=- compiling )
    echo [[ %CXX% %CXXFLAGS% %1 /Fo%filename%.obj /Fe%filename%.exe /link %LNKFLAGS% /MACHINE:%M% ]] >> %logfile% 2>&1
            %CXX% %CXXFLAGS% %1 /Fo%filename%.obj /Fe%filename%.exe /link %LNKFLAGS% /MACHINE:%M%    >> %logfile% 2>&1
    if not ERRORLEVEL 1 goto PGO
    set /A "result+=1"
    <nul (set/p output=- ) & call :SUB_COLOR_TEXT 0C "error"
    if not "%REPRO%"=="1" goto HANDLE_TEMPORARIES
    rem Error during compilation, prepare preprocessed file for troubleshooting
    echo ---------------------------------------- [ Preprocessed ] >> %logfile%
    <nul (set/p output=- repro )
    echo [[ %CXX% %CXXFLAGS% /EP /P /Fi%filename%.i %1 ]] >> %logfile% 2>&1
            %CXX% %CXXFLAGS% /EP /P /Fi%filename%.i %1    >> %logfile% 2>&1
    <nul (set/p output=- compiling )
    echo [[ %CXX% %CXXFLAGS% /TP %filename%.i ]] >> %logfile% 2>&1
            %CXX% %CXXFLAGS% /TP %filename%.i    >> %logfile% 2>&1
    goto HANDLE_TEMPORARIES
)
:PGO
if "%PGO%"=="" goto HANDLE_TEMPORARIES
if exist %filename%-pgo.exe (
    <nul (set/p output=- pgo exists) 
) else (
    <nul (set/p output=- instrumenting )
    echo [[ %CXX% %CXXFLAGS% %1 /Fo%filename%-pgo.obj /Fe%filename%-pgo.exe /link %LNKFLAGS% /MACHINE:%M% /PGD:"%filename%-pgo.pgd" /LTCG:PGINSTRUMENT ]] >> %logfile% 2>&1
            %CXX% %CXXFLAGS% %1 /Fo%filename%-pgo.obj /Fe%filename%-pgo.exe /link %LNKFLAGS% /MACHINE:%M% /PGD:"%filename%-pgo.pgd" /LTCG:PGINSTRUMENT    >> %logfile% 2>&1
    if errorlevel 1 <nul (set/p output=- error) & goto HANDLE_TEMPORARIES
    <nul (set/p output=- profiling )
    %filename%-pgo.exe > nul
    <nul (set/p output=- optimizing )
    echo [[ link %LNKFLAGS% /MACHINE:%M% /PGD:"%filename%-pgo.pgd" %filename%-pgo.obj /LTCG:PGOPTIMIZE ]] >> %logfile% 2>&1
            link %LNKFLAGS% /MACHINE:%M% /PGD:"%filename%-pgo.pgd" %filename%-pgo.obj /LTCG:PGOPTIMIZE    >> %logfile% 2>&1
    if errorlevel 1 <nul (set/p output=- error) & goto HANDLE_TEMPORARIES
)
:HANDLE_TEMPORARIES
echo.
if "%KEEP_TMP%"=="" del %filename%.obj %filename%.pdb %filename%-pgo.obj %filename%-pgo.pdb %filename%-pgo.pgd %filename%-pgo*.pgc > nul 2>&1
rem Reset ERRORLEVEL in case previous command failed
verify > nul
goto END

:TEST ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

if "%1" == "" goto TEST_ALL

:TEST_ARG ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

rem Iterate through all the arguments that were passed and test them
for %%i in (%1) do call :SUB_TEST_FILE "%%i"
shift
if "%1" == "" goto END
goto TEST_ARG

:TEST_ALL ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

rem User didn't provide any arguments, assume all .exe files in test sub-folder
cd /D "%MACH7_ROOT%test\unit"
for %%i in (*.exe) do call :SUB_TEST_FILE "%%i"
cd /D "%CurDir%"
goto END

:SUB_TEST_FILE :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

echo.
echo ======================================== [ %1 ]
if not exist %~n1.*in %1
if     exist %~n1.*in for %%i in (%~n1.*in) do echo. && echo ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ { "%%i" } && type "%%i" && echo. && echo ---------------------------------------- && %1 < "%%i"
goto END

:SUB_CLEAN :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
cd /D %1
del *.i *.obj *.exe *.pdb *.idb *.ilk *.pgd *.pgc *.intermediate.manifest *.nativecodeanalysis.xml *.out cmp_haskell.hi cmp_haskell.o cmp_ocaml.cmi cmp_ocaml.cmx > nul 2>&1
cd /D "%CurDir%"
goto END

:CHECK :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

if "%1" == "" goto CHECK_ALL

:CHECK_ARG :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

rem Iterate through all the arguments that were passed and check them
for %%i in (%1) do call :SUB_CHECK_FILE "%%i"
shift
if "%1" == "" goto END
goto CHECK_ARG

:CHECK_ALL :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

rem User didn't provide any arguments, assume all .exe files in the current folder
for %%i in (*.exe) do call :SUB_CHECK_FILE "%%i"
goto END

:SUB_CHECK_FILE ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

set filename=%~n1
if not exist correct_output/%filename%.out goto END
<nul (set/p output= %filename%.out 	) 
%1 > current.out 2>&1
fc current.out correct_output/%~n1.out | FIND "FC: no dif" > nul 
if errorlevel 1 (<nul (set/p output= ) & call :SUB_COLOR_TEXT 0C "differ" & set /A "result+=1") else (<nul (set/p output= OK))
echo.
goto END

:DOXYGEN :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

rem Build documentation
cd /D "%MACH7_ROOT%"
echo Generating HTML documentation ...
if not exist doc md doc
echo ----------------------------------------------------- >> %logfile%
echo Mach7 documentation log                               >> %logfile%
echo %date% %time%                                         >> %logfile%
echo ----------------------------------------------------- >> %logfile%
if exist doc\html      rmdir /S /Q doc\html                >> %logfile% 2>&1
if exist doc\mach7.chm erase /F doc\mach7.chm              >> %logfile% 2>&1
cd doxygen
doxygen mach7.dxg                                          >> %logfile% 2>&1
cd ..
set genres=%errorlevel%
echo Compiling HTML documentation ...
echo ----------------------------------------------------- >> %logfile%
"%ProgramFiles(x86)%\HTML Help Workshop\hhc.exe" doc\html\index.hhp >> %logfile% 2>&1
set cmplres=%errorlevel%
move doc\html\mach7.chm doc\mach7.chm                      >> %logfile% 2>&1
echo ----------------------------------------------------- >> %logfile%
echo %date% %time%                                         >> %logfile%
echo Complete:	%genres%/%cmplres%
cd /D "%CurDir%"
goto END

:BUILD_UNIT ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
cd /D "%MACH7_ROOT%test\unit"
for %%i in (*.cpp) do call :SUB_BUILD_FILE "%%i"
cd /D "%CurDir%" 
goto END

:BUILD_TIMING ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

cd /D "%MACH7_ROOT%test\time"
for %%i in (*.cpp) do call :SUB_BUILD_FILE "%%i"

for %%F in (0 1) do (
    for %%S in (p f P F) do (
        for %%T in (REP RND SEQ) do (
            call :SUB_BUILD_TIMING %%F %%S %%T %ARCH% skeleton.cxx time
        )
    )
)
cd /D "%CurDir%"
goto END

:SUB_BUILD_TIMING ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

setlocal
set S=%2
set M=%4
if "%1" == "0" (
    if "%2" == "f" set S=k
    if "%2" == "F" set S=K
)
if "%1"  == "0"  (set F=non) ELSE (set F=fwd)
if "%S%" == "p"   set G=generic& set E=poly
if "%S%" == "P"   set G=special& set E=poly
if "%S%" == "k"   set G=generic& set E=kind
if "%S%" == "K"   set G=special& set E=kind
if "%S%" == "f"   set G=generic& set E=knfw
if "%S%" == "F"   set G=special& set E=knfw
if "%M%" == "x86" set B=32
if "%M%" == "x64" set B=64
rem set PGO=1
set CXXFLAGS=/DXTL_VISITOR_FORWARDING=%1 /DXTL_DEFAULT_SYNTAX='%S%' /DXTL_%3_TEST %CXXFLAGS% 
call :SUB_BUILD_FILE %5 %6-%B%-%F%-%G%-%E%-%3
endlocal&set result=%result%
goto END

:BUILD_CMP :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

setlocal
cd /D "%MACH7_ROOT%test\time"
call :SUB_BUILD_TIMING 0 p SEQ %ARCH% cmp_cpp.cxx cmp
call :SUB_BUILD_TIMING 0 f SEQ %ARCH% cmp_cpp.cxx cmp
call :SUB_BUILD_TIMING 1 f SEQ %ARCH% cmp_cpp.cxx cmp
call :SUB_BUILD_TIMING 0 P SEQ %ARCH% cmp_cpp.cxx cmp
call :SUB_BUILD_TIMING 0 F SEQ %ARCH% cmp_cpp.cxx cmp
call :SUB_BUILD_TIMING 1 F SEQ %ARCH% cmp_cpp.cxx cmp
where ghc
if errorlevel 1 echo Warning: No Haskell compiler in the PATH. Skipping & goto skip_haskell
echo ======================================== [ cmp_haskell.exe ] >> %logfile%
<nul (set/p output=Working on cmp_haskell.exe	- compiling ) 
ghc -O --make cmp_haskell >> %logfile% 2>&1
if errorlevel 1 <nul (set/p output=- ) & call :SUB_COLOR_TEXT 0C "error" & set /A "result+=1"
echo.
if "%KEEP_TMP%"=="" del cmp_haskell.hi cmp_haskell.o > nul 2>&1
:skip_haskell
where ocamlopt.opt
if errorlevel 1 echo Warning: No Caml compiler in the PATH. Skipping & goto skip_ocaml
echo ======================================== [ cmp_ocaml.exe ] >> %logfile%
set OCAMLPATH=C:\Program Files (x86)\Objective Caml
set OCAMLLIB=%OCAMLPATH%\lib
set PATH=%OCAMLPATH%\bin;C:\Program Files (x86)\flexdll;%PATH%
<nul (set/p output=Working on cmp_ocaml.exe	- compiling ) 
ocamlopt.opt unix.cmxa -o cmp_ocaml.exe cmp_ocaml.ml >> %logfile% 2>&1
if errorlevel 1 <nul (set/p output=- ) & call :SUB_COLOR_TEXT 0C "error" & set /A "result+=1"
echo.
if "%KEEP_TMP%"=="" del cmp_ocaml.cmi cmp_ocaml.cmx cmp_ocaml.obj > nul 2>&1
:skip_ocaml
cd /D "%CurDir%"
endlocal&set result=%result%
goto END

:BUILD_SYNTAX ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

setlocal
cd /D "%MACH7_ROOT%test\unit"
set CF=%CXXFLAGS%
set CXXFLAGS=%CF% /DXTL_FALL_THROUGH=0 /DXTL_USE_BRACES=1 /DXTL_DEFAULT_SYNTAX='S' & call :SUB_BUILD_FILE syntax.cxx syntax-rc0-ft0-br1-S-spe
set CXXFLAGS=%CF% /DXTL_FALL_THROUGH=0 /DXTL_USE_BRACES=1 /DXTL_DEFAULT_SYNTAX='s' & call :SUB_BUILD_FILE syntax.cxx syntax-rc0-ft0-br1-S-gen
set CXXFLAGS=%CF% /DXTL_FALL_THROUGH=0 /DXTL_USE_BRACES=1 /DXTL_DEFAULT_SYNTAX='P' & call :SUB_BUILD_FILE syntax.cxx syntax-rc0-ft0-br1-P-spe
set CXXFLAGS=%CF% /DXTL_FALL_THROUGH=0 /DXTL_USE_BRACES=1 /DXTL_DEFAULT_SYNTAX='p' & call :SUB_BUILD_FILE syntax.cxx syntax-rc0-ft0-br1-P-gen
set CXXFLAGS=%CF% /DXTL_FALL_THROUGH=0 /DXTL_USE_BRACES=1 /DXTL_DEFAULT_SYNTAX='E' & call :SUB_BUILD_FILE syntax.cxx syntax-rc0-ft0-br1-E-spe
set CXXFLAGS=%CF% /DXTL_FALL_THROUGH=0 /DXTL_USE_BRACES=1 /DXTL_DEFAULT_SYNTAX='e' & call :SUB_BUILD_FILE syntax.cxx syntax-rc0-ft0-br1-E-gen
set CXXFLAGS=%CF% /DXTL_FALL_THROUGH=0 /DXTL_USE_BRACES=1 /DXTL_DEFAULT_SYNTAX='F' & call :SUB_BUILD_FILE syntax.cxx syntax-rc0-ft0-br1-F-spe
set CXXFLAGS=%CF% /DXTL_FALL_THROUGH=0 /DXTL_USE_BRACES=1 /DXTL_DEFAULT_SYNTAX='f' & call :SUB_BUILD_FILE syntax.cxx syntax-rc0-ft0-br1-F-gen
set CXXFLAGS=%CF% /DXTL_FALL_THROUGH=0 /DXTL_USE_BRACES=1 /DXTL_DEFAULT_SYNTAX='U' & call :SUB_BUILD_FILE syntax.cxx syntax-rc0-ft0-br1-U-spe
set CXXFLAGS=%CF% /DXTL_FALL_THROUGH=0 /DXTL_USE_BRACES=1 /DXTL_DEFAULT_SYNTAX='u' & call :SUB_BUILD_FILE syntax.cxx syntax-rc0-ft0-br1-U-gen
set CXXFLAGS=%CF% /DXTL_FALL_THROUGH=0 /DXTL_USE_BRACES=1 /DXTL_DEFAULT_SYNTAX='K' & call :SUB_BUILD_FILE syntax.cxx syntax-rc0-ft0-br1-K-spe
set CXXFLAGS=%CF% /DXTL_FALL_THROUGH=0 /DXTL_USE_BRACES=1 /DXTL_DEFAULT_SYNTAX='k' & call :SUB_BUILD_FILE syntax.cxx syntax-rc0-ft0-br1-K-gen
set CXXFLAGS=%CF% /DXTL_FALL_THROUGH=1 /DXTL_USE_BRACES=1 /DXTL_DEFAULT_SYNTAX='S' & call :SUB_BUILD_FILE syntax.cxx syntax-rc0-ft1-br1-S-spe
set CXXFLAGS=%CF% /DXTL_FALL_THROUGH=1 /DXTL_USE_BRACES=1 /DXTL_DEFAULT_SYNTAX='s' & call :SUB_BUILD_FILE syntax.cxx syntax-rc0-ft1-br1-S-gen
set CXXFLAGS=%CF% /DXTL_FALL_THROUGH=1 /DXTL_USE_BRACES=1 /DXTL_DEFAULT_SYNTAX='P' & call :SUB_BUILD_FILE syntax.cxx syntax-rc0-ft1-br1-P-spe
set CXXFLAGS=%CF% /DXTL_FALL_THROUGH=1 /DXTL_USE_BRACES=1 /DXTL_DEFAULT_SYNTAX='p' & call :SUB_BUILD_FILE syntax.cxx syntax-rc0-ft1-br1-P-gen
set CXXFLAGS=%CF% /DXTL_FALL_THROUGH=1 /DXTL_USE_BRACES=1 /DXTL_DEFAULT_SYNTAX='E' & call :SUB_BUILD_FILE syntax.cxx syntax-rc0-ft1-br1-E-spe
set CXXFLAGS=%CF% /DXTL_FALL_THROUGH=1 /DXTL_USE_BRACES=1 /DXTL_DEFAULT_SYNTAX='e' & call :SUB_BUILD_FILE syntax.cxx syntax-rc0-ft1-br1-E-gen
set CXXFLAGS=%CF% /DXTL_FALL_THROUGH=1 /DXTL_USE_BRACES=1 /DXTL_DEFAULT_SYNTAX='F' & call :SUB_BUILD_FILE syntax.cxx syntax-rc0-ft1-br1-F-spe
set CXXFLAGS=%CF% /DXTL_FALL_THROUGH=1 /DXTL_USE_BRACES=1 /DXTL_DEFAULT_SYNTAX='f' & call :SUB_BUILD_FILE syntax.cxx syntax-rc0-ft1-br1-F-gen
set CXXFLAGS=%CF% /DXTL_FALL_THROUGH=1 /DXTL_USE_BRACES=1 /DXTL_DEFAULT_SYNTAX='U' & call :SUB_BUILD_FILE syntax.cxx syntax-rc0-ft1-br1-U-spe
set CXXFLAGS=%CF% /DXTL_FALL_THROUGH=1 /DXTL_USE_BRACES=1 /DXTL_DEFAULT_SYNTAX='u' & call :SUB_BUILD_FILE syntax.cxx syntax-rc0-ft1-br1-U-gen
set CXXFLAGS=%CF% /DXTL_FALL_THROUGH=1 /DXTL_USE_BRACES=1 /DXTL_DEFAULT_SYNTAX='K' & call :SUB_BUILD_FILE syntax.cxx syntax-rc0-ft1-br1-K-spe
set CXXFLAGS=%CF% /DXTL_FALL_THROUGH=1 /DXTL_USE_BRACES=1 /DXTL_DEFAULT_SYNTAX='k' & call :SUB_BUILD_FILE syntax.cxx syntax-rc0-ft1-br1-K-gen
cd /D "%CurDir%"
endlocal&set result=%result%
goto END

:BUILD_WITH_CMAKE ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
setlocal
cd /D %MACH7_ROOT%test
if not exist build mkdir build
cd build
cmake %1 %2 %3 %4 %5 %6 %7 %8 %9 "%MACH7_ROOT%test"
cmake --build .
endlocal
goto END

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

:SUB_COLOR_TEXT ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

rem The following text coloring code has been derived from:
rem http://stackoverflow.com/questions/4339649/how-to-have-multiple-colors-in-a-batch-file
rem The first  argument is the color (see "color /?" for its specification)
rem The second argument is the text to show in that color (no CR-LF is outputted)

for /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do rem"') do (
  set "DEL=%%a"
)

<nul set /p ".=%DEL%" > "%~2"
findstr /v /a:%1 /R "^$" "%~2" nul
del "%~2" > nul 2>&1
goto END

:END :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
