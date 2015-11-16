::
:: Script for building and testing C++ pattern matching library
::
:: Written by Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
:: Version 1.0 from 4th February 2012
::
:: Usage:
::     build [ pgo | tmp ] [ filemask*.cpp ... ]
::     build        - Build all examples using the most recent MS VC++ compiler
::     build clean  - Clean all examples
::     build test   - Test all built examples
::     build timing - Build with a given version of MS VC++
::                                                                           
:: This file is a part of Mach7 library (http://parasol.tamu.edu/mach7/).
:: Copyright (C) 2011-2012 Texas A&M University.
:: All rights reserved.
:: 

@echo off

if "%1" == "/?" findstr "^::" "%~f0" & goto END
if "%MACH7%" == "" echo Warning: Environment variable MACH7 must be set to library path of Mach7 library (available here: https://github.com/solodon4/Mach7). Assuming ..\..\.. && set MACH7=..\..\..
rem Set-up variables :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
set CXX=cl.exe
rem Debug version of compiler flags
rem CXXFLAGS=/D "XTL_VIRTUAL=virtual" /D "XTL_PROFILING=1" /D "XTL_MAX_LOG_INC=1" /D "XTL_LEAKED_NEW_LOCATIONS" /Zi /nologo /EHsc /W3 /WX- /Od /RTC1        /Oy-         /Gm  /MTd /GS       /fp:precise /Zc:wchar_t /Zc:forScope /Gd /analyze- /errorReport:queue /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_UNICODE" /D "UNICODE" /I%MACH7% /I%MACH7%/test/time
rem Release version of compiler flags
set CXXFLAGS=/D "XTL_VIRTUAL=virtual" /D "XTL_PROFILING=1" /D "XTL_MAX_LOG_INC=1"                               /Zi /nologo /EHsc /W3 /WX- /O2 /Ob2 /Oi /Ot /Oy- /GL /GF /Gm- /MT  /GS- /Gy- /fp:precise /Zc:wchar_t /Zc:forScope /Gr /analyze- /errorReport:queue /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /I%MACH7% /I%MACH7%/test/time
rem          /Zi /nologo       /W3 /WX- /O2 /Ob2 /Oi /Ot /Oy- /GL /GF /Gm- /MT /GS- /Gy- /fp:precise /Zc:wchar_t /Zc:forScope /Gr /analyze- /errorReport:queue /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FU"C:\Program Files (x86)\Reference Assemblies\Microsoft\Framework\.NETFramework\v4.0\System.Core.dll" /Fp"Release\SyntheticSelectRandom.pch" /Fa"Release\" /Fo"Release\" /Fd"Release\vc100.pdb" 
rem Slower: =/Zi /nologo       /W3 /WX- /O2 /Ob2 /Oi /Ot      /GL /GF /Gm- /MT /GS- /Gy  /fp:precise /Zc:wchar_t /Zc:forScope /Gr           /errorReport:queue /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /I%MACH7% /I%MACH7%/test/time 
rem Debug version of linker flags
rem LNKFLAGS=/INCREMENTAL    /NOLOGO "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib" /MANIFEST    /ALLOWISOLATION /SUBSYSTEM:CONSOLE /DEBUG                  /TLBID:1 /DYNAMICBASE    /NXCOMPAT /ERRORREPORT:QUEUE 
rem Release version of linker flags
set LNKFLAGS=/INCREMENTAL:NO /NOLOGO "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib" /MANIFEST:NO /ALLOWISOLATION /SUBSYSTEM:CONSOLE /OPT:REF /OPT:ICF /LTCG /TLBID:1 /DYNAMICBASE:NO /NXCOMPAT /ERRORREPORT:QUEUE 
rem /OUT:"C:\Projects\PatternMatching\Release\SyntheticSelectRandom.exe" /INCREMENTAL:NO /NOLOGO "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib" /MANIFEST /ManifestFile:"Release\SyntheticSelectRandom.exe.intermediate.manifest" /MANIFESTUAC:"level='asInvoker' uiAccess='false'" /DEBUG /PDB:"C:\Projects\PatternMatching\Release\SyntheticSelectRandom.pdb" /SUBSYSTEM:CONSOLE /OPT:REF /OPT:ICF /PGD:"C:\Projects\PatternMatching\Release\SyntheticSelectRandom.pgd" /LTCG /TLBID:1 /DYNAMICBASE:NO /NXCOMPAT /MACHINE:X86 /ERRORREPORT:QUEUE 
set M=X86

rem Reset ERRORLEVEL in case previous command failed
verify > nul

rem Prepare log file :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

rem call :SUB_PARSE_DATE
rem set logfile=build-%yy%-%mm%-%dd%-%hh%-%mn%.log
set logfile=build.log
echo. > %logfile%
echo XTL Pattern Matching Build Script >> %logfile%
echo Version 1.0 from 2012-02-04 >> %logfile%
echo. >> %logfile%
echo Build log from %date% at %time% >> %logfile%

:PARSE_CMD_LINE ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

if "%1" == "pgo"    shift && set PGO=1      && goto PARSE_CMD_LINE
if "%1" == "tmp"    shift && set KEEP_TMP=1 && goto PARSE_CMD_LINE
if "%1" == "clean"  del *.obj *.exe *.pdb *.idb *.intermediate.manifest *.out > nul 2>&1 && goto END
if "%1" == "test"   shift && goto TEST

rem Set-up Visual C++ Environment Variables ::::::::::::::::::::::::::::::::::::

if not "%VS110COMNTOOLS%" == "" call "%VS110COMNTOOLS%vsvars32.bat" && goto PROCEED
if not "%VS100COMNTOOLS%" == "" call "%VS100COMNTOOLS%vsvars32.bat" && goto PROCEED
if not "%VS90COMNTOOLS%"  == "" call "%VS90COMNTOOLS%vsvars32.bat"  && goto PROCEED
if not "%VS80COMNTOOLS%"  == "" call "%VS80COMNTOOLS%vsvars32.bat"  && goto PROCEED
if not "%VS71COMNTOOLS%"  == "" call "%VS71COMNTOOLS%vsvars32.bat"  && goto PROCEED

echo ERROR: Unable to find installation of MS Visual C++ on this computer
goto END

:PROCEED :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

setlocal

if "%1" == "timing" shift && goto TIMING
if "%1" == ""                goto BUILD_ALL

:BUILD_ARG :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

rem Iterate through all the arguments that were passed and build them
for %%i in (%1) do call :SUB_BUILD_FILE "%%i"
shift
if "%1" == "" goto END
goto BUILD_ARG

:BUILD_ALL :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

rem User didn't provide any arguments, assume all .cpp files in the current folder
for %%i in (*.hpp) do call :SUB_BUILD_FILE "%%i"
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
    echo [[ %CXX% %CXXFLAGS% %filename%*.cpp /Fe%filename%.exe /link %LNKFLAGS% /MACHINE:%M% ]] >> %logfile% 2>&1
            %CXX% %CXXFLAGS% %filename%*.cpp /Fe%filename%.exe /link %LNKFLAGS% /MACHINE:%M%    >> %logfile% 2>&1
    if errorlevel 1 <nul (set/p output=- error) & goto HANDLE_TEMPORARIES
)
if "%PGO%"=="" goto HANDLE_TEMPORARIES
if exist %filename%-pgo.exe (
    <nul (set/p output=- pgo exists) 
) else (
    <nul (set/p output=- instrumenting )
    echo [[ %CXX% %CXXFLAGS% %filename%*.cpp /Fe%filename%-pgo.exe /link %LNKFLAGS% /MACHINE:%M% /PGD:"%filename%-pgo.pgd" /LTCG:PGINSTRUMENT ]] >> %logfile% 2>&1
            %CXX% %CXXFLAGS% %filename%*.cpp /Fe%filename%-pgo.exe /link %LNKFLAGS% /MACHINE:%M% /PGD:"%filename%-pgo.pgd" /LTCG:PGINSTRUMENT    >> %logfile% 2>&1
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

:TIMING ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

for %%G in (x86 x64) do (
    rem Set up VC variables for x86 build
    call "%VCINSTALLDIR%vcvarsall.bat" %%G
    for %%F in (0 1) do (
        for %%S in (p f P F) do (
            for %%T in (REP RND SEQ) do (
                call :SUB_BUILD_TIMING %%F %%S %%T %%G
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
set PGO=1
set CXXFLAGS=/DXTL_VISITOR_FORWARDING=%1 /DXTL_DEFAULT_SYNTAX='%S%' /DXTL_%3_TEST %CXXFLAGS% 
call :SUB_BUILD_FILE skeleton.cxx time-%B%-%F%-%G%-%E%-%3
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

:END :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
