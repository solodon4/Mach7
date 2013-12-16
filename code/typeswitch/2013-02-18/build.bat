::
:: Script for building and testing C++ pattern matching library
::
:: Written by Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
:: Version 1.0 from 4th February 2012
::
:: Usage:
::
::     build [ pgo | tmp | <ver> ] [ filemask*.cpp ... ]
::     build        - Build all examples using the most recent MS Visual C++ compiler installed
::     build timing - Build all supported library options combination for timing variations
::     build cmp    - Build all executables for comparison with other languages
::     build clean  - Clean all built examples
::     build test   -   Run all built examples
::     build check  -   Run those examples for which there are correct_output/*.out files and check that output is the same
::
:: Modifiers:
::            pgo   - Perform Profile-Guided Optimization on produced executables
::            tmp   - Keep temporaries
::           <ver>  - Use a specific version of Visual C++ to compiler the source 
::                    code. <ver> can be one of the following:
::                     - 2003 - Visual C++  7.1
::                     - 2005 - Visual C++  8.0
::                     - 2008 - Visual C++  9.0
::                     - 2010 - Visual C++ 10.0
::                     - 2012 - Visual C++ 11.0
::                                                                           
:: This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
:: Copyright (C) 2011-2013 Texas A&M University.
:: All rights reserved.
:: 

@echo off

if "%1" == "/?" findstr "^::" "%~f0" & goto END

rem Set-up variables :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

set CXX=cl.exe
rem List of compiler options: http://technet.microsoft.com/en-us/library/fwkeyyhe(v=vs.110).aspx
set CXXFLAGS=/wd4007 /Zi /nologo /EHsc /W4 /WX- /O2 /Ob2 /Oi /Ot /Oy- /GL /GF /Gm- /MT /GS- /Gy- /fp:precise /Zc:wchar_t /Zc:forScope /Gr /analyze- /errorReport:queue /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" 
rem          /wd4007 /Zi /nologo       /W3 /WX- /O2 /Ob2 /Oi /Ot /Oy- /GL /GF /Gm- /MT /GS- /Gy- /fp:precise /Zc:wchar_t /Zc:forScope /Gr /analyze- /errorReport:queue /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FU"C:\Program Files (x86)\Reference Assemblies\Microsoft\Framework\.NETFramework\v4.0\System.Core.dll" /Fp"Release\SyntheticSelectRandom.pch" /Fa"Release\" /Fo"Release\" /Fd"Release\vc100.pdb" 
rem Slower: =/wd4007 /Zi /nologo       /W3 /WX- /O2 /Ob2 /Oi /Ot      /GL /GF /Gm- /MT /GS- /Gy  /fp:precise /Zc:wchar_t /Zc:forScope /Gr           /errorReport:queue /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" 
set LNKFLAGS=/INCREMENTAL:NO /NOLOGO "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib" /MANIFEST:NO /ALLOWISOLATION /SUBSYSTEM:CONSOLE /OPT:REF /OPT:ICF /LTCG /TLBID:1 /DYNAMICBASE:NO /NXCOMPAT /ERRORREPORT:QUEUE 
rem /OUT:"C:\Projects\PatternMatching\Release\SyntheticSelectRandom.exe" /INCREMENTAL:NO /NOLOGO "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib" /MANIFEST /ManifestFile:"Release\SyntheticSelectRandom.exe.intermediate.manifest" /MANIFESTUAC:"level='asInvoker' uiAccess='false'" /DEBUG /PDB:"C:\Projects\PatternMatching\Release\SyntheticSelectRandom.pdb" /SUBSYSTEM:CONSOLE /OPT:REF /OPT:ICF /PGD:"C:\Projects\PatternMatching\Release\SyntheticSelectRandom.pgd" /LTCG /TLBID:1 /DYNAMICBASE:NO /NXCOMPAT /MACHINE:X86 /ERRORREPORT:QUEUE 
set M=X86

rem Reset ERRORLEVEL in case previous command failed
verify > nul

rem Prepare log file :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

rem Specific invokations of this script write log to common build.log file
if not "%1" == "" set logfile=build.log&goto LOG_FILE_READY
rem Build of everythings gets its own time-stamped log file
call :SUB_PARSE_DATE 
set logfile=build-%yy%-%mm%-%dd%-%hh%-%mn%.log

:LOG_FILE_READY

echo. > %logfile%
echo Mach7 Build Script >> %logfile%
echo Version 1.0 from 2012-02-04 >> %logfile%
echo. >> %logfile%
echo Build log from %date% at %time% >> %logfile%

:PARSE_CMD_LINE ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

rem Parse modifiers

if "%1" == "pgo"       shift && set PGO=1&&                          goto PARSE_CMD_LINE
if "%1" == "tmp"       shift && set KEEP_TMP=1&&                     goto PARSE_CMD_LINE
if "%1" == "2012"      shift && set VS_COMN_TOOLS=%VS110COMNTOOLS%&& goto PARSE_CMD_LINE
if "%1" == "2010"      shift && set VS_COMN_TOOLS=%VS100COMNTOOLS%&& goto PARSE_CMD_LINE
if "%1" == "2008"      shift && set VS_COMN_TOOLS=%VS90COMNTOOLS%&&  goto PARSE_CMD_LINE
if "%1" == "2005"      shift && set VS_COMN_TOOLS=%VS80COMNTOOLS%&&  goto PARSE_CMD_LINE
if "%1" == "2003"      shift && set VS_COMN_TOOLS=%VS71COMNTOOLS%&&  goto PARSE_CMD_LINE

rem Parse commands

if "%1" == "clean"     del *.obj *.exe *.pdb *.idb *.intermediate.manifest *.out cmp_haskell.hi cmp_haskell.o cmp_ocaml.cmi cmp_ocaml.cmx > nul 2>&1 && goto END
if "%1" == "check"     shift && goto CHECK
if "%1" == "test"      shift && goto TEST

rem Subsequent commands require Visual C++ environment variables to be set up.

if not "%VS_COMN_TOOLS%" == "" goto PROCEED

rem Detect most recent version of the Visual C++ installed :::::::::::::::::::::

if not "%VS110COMNTOOLS%" == "" set VS_COMN_TOOLS=%VS110COMNTOOLS%&& goto PROCEED
if not "%VS100COMNTOOLS%" == "" set VS_COMN_TOOLS=%VS100COMNTOOLS%&& goto PROCEED
if not "%VS90COMNTOOLS%"  == "" set VS_COMN_TOOLS=%VS90COMNTOOLS%&&  goto PROCEED
if not "%VS80COMNTOOLS%"  == "" set VS_COMN_TOOLS=%VS80COMNTOOLS%&&  goto PROCEED
if not "%VS71COMNTOOLS%"  == "" set VS_COMN_TOOLS=%VS71COMNTOOLS%&&  goto PROCEED

echo ERROR: Unable to find installation of MS Visual C++ on this computer
goto END

:PROCEED :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

rem Set-up Visual C++ Environment Variables
call "%VS_COMN_TOOLS%vsvars32.bat"

rem Account for a problem with some PGO flags described above.
if "%PGO%"=="1" set CXXFLAGS=%CXXFLAGS% /GL 
if "%PGO%"=="1" set LNKFLAGS=%LNKFLAGS% /LTCG

setlocal

if "%1" == "timing" shift && goto BUILD_TIMING
if "%1" == "cmp"    shift && goto BUILD_CMP
if "%1" == ""                goto BUILD_ALL

:BUILD_ARG :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

rem Iterate through all the arguments that were passed and build them
for %%i in (%1) do call :SUB_BUILD_FILE "%%i"
shift
if "%1" == "" goto END
goto BUILD_ARG

:BUILD_ALL :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

rem User didn't provide any arguments, assume all .cpp files in the current folder
for %%i in (*.cpp) do call :SUB_BUILD_FILE "%%i"
call :BUILD_CMP
call :BUILD_TIMING
echo Build log has been saved to %logfile%
goto END

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
    if errorlevel 1 <nul (set/p output=- ) & call :SUB_COLOR_TEXT 0C "error" & goto HANDLE_TEMPORARIES
)
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

rem User didn't provide any arguments, assume all .exe files in the current folder
for %%i in (*.exe) do call :SUB_TEST_FILE "%%i"
goto END

:SUB_TEST_FILE :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

echo.
echo ======================================== [ %1 ]
if not exist %~n1.*in %1
if     exist %~n1.*in for %%i in (%~n1.*in) do echo. && echo ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ { "%%i" } && type "%%i" && echo. && echo ---------------------------------------- && %1 < "%%i"
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
if errorlevel 1 (<nul (set/p output= ) & call :SUB_COLOR_TEXT 0C "differ") else (<nul (set/p output= OK))
echo.
goto END

:BUILD_TIMING ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

for %%G in (x86 x64) do (
    rem Set up VC variables for x86 build
    call "%VCINSTALLDIR%vcvarsall.bat" %%G
    for %%F in (0 1) do (
        for %%S in (p f P F) do (
            for %%T in (REP RND SEQ) do (
                call :SUB_BUILD_TIMING %%F %%S %%T %%G skeleton.cxx time
            )
        )
    )
)
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
endlocal
goto END

:BUILD_CMP :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

call :SUB_BUILD_TIMING 0 p SEQ x86 cmp_cpp.cxx cmp
call :SUB_BUILD_TIMING 0 f SEQ x86 cmp_cpp.cxx cmp
call :SUB_BUILD_TIMING 1 f SEQ x86 cmp_cpp.cxx cmp
call :SUB_BUILD_TIMING 0 P SEQ x86 cmp_cpp.cxx cmp
call :SUB_BUILD_TIMING 0 F SEQ x86 cmp_cpp.cxx cmp
call :SUB_BUILD_TIMING 1 F SEQ x86 cmp_cpp.cxx cmp
echo ======================================== [ cmp_haskell.exe ] >> %logfile%
<nul (set/p output=Working on cmp_haskell.exe	- compiling ) 
ghc -O --make cmp_haskell >> %logfile% 2>&1
if errorlevel 1 <nul (set/p output=- ) & call :SUB_COLOR_TEXT 0C "error"
echo.
if "%KEEP_TMP%"=="" del cmp_haskell.hi cmp_haskell.o > nul 2>&1
echo ======================================== [ cmp_ocaml.exe ] >> %logfile%
set OCAMLPATH=C:\Program Files (x86)\Objective Caml
set OCAMLLIB=%OCAMLPATH%\lib
set PATH=%OCAMLPATH%\bin;C:\Program Files (x86)\flexdll;%PATH%
<nul (set/p output=Working on cmp_ocaml.exe	- compiling ) 
ocamlopt.opt unix.cmxa -o cmp_ocaml.exe cmp_ocaml.ml >> %logfile% 2>&1
if errorlevel 1 <nul (set/p output=- ) & call :SUB_COLOR_TEXT 0C "error"
echo.
if "%KEEP_TMP%"=="" del cmp_ocaml.cmi cmp_ocaml.cmx cmp_ocaml.obj > nul 2>&1
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
