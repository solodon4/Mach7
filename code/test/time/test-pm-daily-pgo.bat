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
:: Script for testing C++ pattern matching library with Profile-Guided Optimizations
::
:: \author Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
::
:: \see https://parasol.tamu.edu/mach7/
:: \see https://github.com/solodon4/Mach7
:: \see https://github.com/solodon4/SELL
::
:: Version 1.0 from 18th May 2011
::
:: Usage: test-pm-daily-pgo.bat
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

set logfile=test-pm-daily-pgo.log
set results=test-pm-daily-pgo.txt

echo Test from %date% at %time% > %logfile%
echo Test from %date% at %time% > %results%

rem Set up VC variables for x86 build
call "%VCINSTALLDIR%vcvarsall.bat" x86

rem  Build all sources for x86 without PGO
echo Build all sources for x86 without PGO ...
echo ========== [ Visual C++/Win32 without PGO ] =============================== >> %logfile%
echo ========== [ Visual C++/Win32 without PGO ] =============================== >> %results%
MSBuild.exe PatternMatching.2010.sln /property:Configuration=Release /property:Platform=Win32 >> %logfile% 2>&1

rem -----------------------------------------------------------------------------

echo Running Win32/Synthetic.exe
echo [ Win32/Synthetic.exe ] ============================ | tee -a %logfile% >> %results%
rem Release\Synthetic.exe                  | tee -a %logfile% | grep "L.:" >> %results%
echo Running Win32/GenericSelectRandom.exe
echo [ Win32/GenericSelectRandom.exe ] ================== | tee -a %logfile% >> %results%
Release\GenericSelectRandom.exe        | tee -a %logfile% | grep "L.:" >> %results%
echo Running Win32/SyntheticSelectRandom.exe
echo [ Win32/SyntheticSelectRandom.exe ] ================ | tee -a %logfile% >> %results%
Release\SyntheticSelectRandom.exe      | tee -a %logfile% | grep "L.:" >> %results%
echo Running Win32/SyntheticSelect.exe
echo [ Win32/SyntheticSelect.exe ] ====================== | tee -a %logfile% >> %results%
Release\SyntheticSelect.exe            | tee -a %logfile% | grep "L.:" >> %results%
echo Running Win32/Hierarchy.exe
echo [ Win32/Hierarchy.exe ] ============================ | tee -a %logfile% >> %results%
Release\Hierarchy.exe                  | tee -a %logfile% | grep "L.:" >> %results%
echo Running Win32/GenericSelectKind.exe
echo [ Win32/GenericSelectKind.exe ] ================== | tee -a %logfile% >> %results%
Release\GenericSelectKind.exe          | tee -a %logfile% | grep "L.:" >> %results%
echo Running Win32/SyntheticSelectKind.exe
echo [ Win32/SyntheticSelectKind.exe ] ================== | tee -a %logfile% >> %results%
Release\SyntheticSelectKind.exe        | tee -a %logfile% | grep "L.:" >> %results%
echo Running Win32/TypeSwitch.exe
echo [ Win32/TypeSwitch.exe ] =========================== | tee -a %logfile% >> %results%
Release\TypeSwitch.exe                 | tee -a %logfile% | grep "L.:" >> %results%
echo Running Win32/OCamlCmp.exe
echo [ Win32/OCamlCmp.exe ] ============================= | tee -a %logfile% >> %results%
Release\OCamlCmp.exe                   | tee -a %logfile% | grep ":"   >> %results%
echo Running Win32/OCamlCmpKind.exe
echo [ Win32/OCamlCmpKind.exe ] ========================= | tee -a %logfile% >> %results%
Release\OCamlCmpKind.exe               | tee -a %logfile% | grep ":"   >> %results%
echo Running Win32/Numbers.exe
echo [ Win32/Numbers.exe ] ============================== | tee -a %logfile% >> %results%
Release\Numbers.exe                    | tee -a %logfile% | grep "%%"  >> %results%

rem Instrument all projects for Profile Guided Optimizations
echo Build all sources for x86 instrumented for PGO ...
echo ========== [ Visual C++/Win32 instrument PGO ] ============================ >> %logfile%
MSBuild.exe PatternMatching.2010.sln /property:Configuration=Release /property:Platform=Win32 /property:WholeProgramOptimization=PGInstrument >> %logfile% 2>&1

echo Profiling Win32/Category.exe
Release\Category.exe              > nul
echo Profiling Win32/GenericSelectKind.exe
Release\GenericSelectKind.exe     > nul
echo Profiling Win32/GenericSelectRandom.exe
Release\GenericSelectRandom.exe   > nul
echo Profiling Win32/Hierarchy.exe
Release\Hierarchy.exe             > nul
echo Profiling Win32/MemoizedCast.exe
Release\MemoizedCast.exe          > nul
echo Profiling Win32/Numbers.exe
Release\Numbers.exe               > nul
echo Profiling Win32/OCamlCmp.exe
Release\OCamlCmp.exe              > nul
echo Profiling Win32/OCamlCmpKind.exe
Release\OCamlCmpKind.exe          > nul
echo Profiling Win32/Shape1.exe
Release\Shape1.exe                > nul
echo Profiling Win32/Shape2.exe
Release\Shape2.exe                > nul
echo Profiling Win32/Shape3.exe
Release\Shape3.exe                > nul
echo Profiling Win32/Synthetic.exe
rem Release\Synthetic.exe         > nul
echo Profiling Win32/SyntheticSelect.exe
Release\SyntheticSelect.exe       > nul
echo Profiling Win32/SyntheticSelectKind.exe
Release\SyntheticSelectKind.exe   > nul
echo Profiling Win32/SyntheticSelectRandom.exe
Release\SyntheticSelectRandom.exe > nul
echo Profiling Win32/TypeSwitch.exe
Release\TypeSwitch.exe            > nul

rem Optimize programs based on collected data
echo Build all sources for x86 optimized with PGO ...
echo ========== [ Visual C++/Win32 optimize PGO ] ============================== >> %logfile%
echo ========== [ Visual C++/Win32 optimize PGO ] ============================== >> %results%
MSBuild.exe PatternMatching.2010.sln /property:Configuration=Release /property:Platform=Win32 /property:WholeProgramOptimization=PGOptimize >> %logfile% 2>&1

rem -----------------------------------------------------------------------------

echo Running Win32/Synthetic.exe
echo [ Win32/Synthetic.exe ] ============================ | tee -a %logfile% >> %results%
rem Release\Synthetic.exe                  | tee -a %logfile% | grep "L.:" >> %results%
echo Running Win32/GenericSelectRandom.exe
echo [ Win32/GenericSelectRandom.exe ] ================== | tee -a %logfile% >> %results%
Release\GenericSelectRandom.exe        | tee -a %logfile% | grep "L.:" >> %results%
echo Running Win32/SyntheticSelectRandom.exe
echo [ Win32/SyntheticSelectRandom.exe ] ================ | tee -a %logfile% >> %results%
Release\SyntheticSelectRandom.exe      | tee -a %logfile% | grep "L.:" >> %results%
echo Running Win32/SyntheticSelect.exe
echo [ Win32/SyntheticSelect.exe ] ====================== | tee -a %logfile% >> %results%
Release\SyntheticSelect.exe            | tee -a %logfile% | grep "L.:" >> %results%
echo Running Win32/Hierarchy.exe
echo [ Win32/Hierarchy.exe ] ============================ | tee -a %logfile% >> %results%
Release\Hierarchy.exe                  | tee -a %logfile% | grep "L.:" >> %results%
echo Running Win32/GenericSelectKind.exe
echo [ Win32/GenericSelectKind.exe ] ==================== | tee -a %logfile% >> %results%
Release\GenericSelectKind.exe          | tee -a %logfile% | grep "L.:" >> %results%
echo Running Win32/SyntheticSelectKind.exe
echo [ Win32/SyntheticSelectKind.exe ] ================== | tee -a %logfile% >> %results%
Release\SyntheticSelectKind.exe        | tee -a %logfile% | grep "L.:" >> %results%
echo Running Win32/TypeSwitch.exe
echo [ Win32/TypeSwitch.exe ] =========================== | tee -a %logfile% >> %results%
Release\TypeSwitch.exe                 | tee -a %logfile% | grep "L.:" >> %results%
echo Running Win32/OCamlCmp.exe
echo [ Win32/OCamlCmp.exe ] ============================= | tee -a %logfile% >> %results%
Release\OCamlCmp.exe                   | tee -a %logfile% | grep ":"   >> %results%
echo Running Win32/OCamlCmpKind.exe
echo [ Win32/OCamlCmpKind.exe ] ========================= | tee -a %logfile% >> %results%
Release\OCamlCmpKind.exe               | tee -a %logfile% | grep ":"   >> %results%
echo Running Win32/Numbers.exe
echo [ Win32/Numbers.exe ] ============================== | tee -a %logfile% >> %results%
Release\Numbers.exe                    | tee -a %logfile% | grep "%%"  >> %results%

rem Set up VC variables for x64 build
call "%VCINSTALLDIR%vcvarsall.bat" x64

rem  Build all sources for x64 without PGO
echo Build all sources for x64 without PGO ...
echo ========== [ Visual C++/x64 without PGO ] ================================= >> %logfile%
echo ========== [ Visual C++/x64 without PGO ] ================================= >> %results%
MSBuild.exe PatternMatching.2010.sln /property:Configuration=Release /property:Platform=x64 >> %logfile% 2>&1

rem -----------------------------------------------------------------------------

echo Running x64/Synthetic.exe
echo [ x64/Synthetic.exe ] ============================== | tee -a %logfile% >> %results%
rem x64\Release\Synthetic.exe              | tee -a %logfile% | grep "L.:" >> %results%
echo Running x64/GenericSelectRandom.exe
echo [ x64/GenericSelectRandom.exe ] ==================== | tee -a %logfile% >> %results%
x64\Release\GenericSelectRandom.exe    | tee -a %logfile% | grep "L.:" >> %results%
echo Running x64/SyntheticSelectRandom.exe
echo [ x64/SyntheticSelectRandom.exe ] ================== | tee -a %logfile% >> %results%
x64\Release\SyntheticSelectRandom.exe  | tee -a %logfile% | grep "L.:" >> %results%
echo Running x64/SyntheticSelect.exe
echo [ x64/SyntheticSelect.exe ] ======================== | tee -a %logfile% >> %results%
x64\Release\SyntheticSelect.exe        | tee -a %logfile% | grep "L.:" >> %results%
echo Running x64/Hierarchy.exe
echo [ x64/Hierarchy.exe ] ============================== | tee -a %logfile% >> %results%
x64\Release\Hierarchy.exe              | tee -a %logfile% | grep "L.:" >> %results%
echo Running x64/GenericSelectKind.exe
echo [ x64/GenericSelectKind.exe ] ====================== | tee -a %logfile% >> %results%
x64\Release\GenericSelectKind.exe      | tee -a %logfile% | grep "L.:" >> %results%
echo Running x64/SyntheticSelectKind.exe
echo [ x64/SyntheticSelectKind.exe ] ==================== | tee -a %logfile% >> %results%
x64\Release\SyntheticSelectKind.exe    | tee -a %logfile% | grep "L.:" >> %results%
echo Running x64/TypeSwitch.exe
echo [ x64/TypeSwitch.exe ] ============================= | tee -a %logfile% >> %results%
x64\Release\TypeSwitch.exe             | tee -a %logfile% | grep "L.:" >> %results%
echo Running x64/OCamlCmp.exe
echo [ x64/OCamlCmp.exe ] =============================== | tee -a %logfile% >> %results%
x64\Release\OCamlCmp.exe               | tee -a %logfile% | grep ":"   >> %results%
echo Running x64/OCamlCmpKind.exe
echo [ x64/OCamlCmpKind.exe ] =========================== | tee -a %logfile% >> %results%
x64\Release\OCamlCmpKind.exe           | tee -a %logfile% | grep ":"   >> %results%
echo Running x64/Numbers.exe
echo [ x64/Numbers.exe ] ================================ | tee -a %logfile% >> %results%
x64\Release\Numbers.exe                | tee -a %logfile% | grep "%%"  >> %results%

rem Instrument all projects for Profile Guided Optimizations
echo Build all sources for x64 instrumented for PGO ...
echo ========== [ Visual C++/x64 instrument PGO ] ============================== >> %logfile%
MSBuild.exe PatternMatching.2010.sln /property:Configuration=Release /property:Platform=x64 /property:WholeProgramOptimization=PGInstrument >> %logfile% 2>&1

rem Run instrumented programs to collect data
echo Profiling x64/Category.exe
x64\Release\Category.exe              > nul
echo Profiling x64/GenericSelectKind.exe
x64\Release\GenericSelectKind.exe     > nul
echo Profiling x64/GenericSelectRandom.exe
x64\Release\GenericSelectRandom.exe   > nul
echo Profiling x64/Hierarchy.exe
x64\Release\Hierarchy.exe             > nul
echo Profiling x64/MemoizedCast.exe
x64\Release\MemoizedCast.exe          > nul
echo Profiling x64/Numbers.exe
x64\Release\Numbers.exe               > nul
echo Profiling x64/OCamlCmp.exe
x64\Release\OCamlCmp.exe              > nul
echo Profiling x64/OCamlCmpKind.exe
x64\Release\OCamlCmpKind.exe          > nul
echo Profiling x64/Shape1.exe
x64\Release\Shape1.exe                > nul
echo Profiling x64/Shape2.exe
x64\Release\Shape2.exe                > nul
echo Profiling x64/Shape3.exe
x64\Release\Shape3.exe                > nul
echo Profiling x64/Synthetic.exe
rem x64\Release\Synthetic.exe         > nul
echo Profiling x64/SyntheticSelect.exe
x64\Release\SyntheticSelect.exe       > nul
echo Profiling x64/SyntheticSelectKind.exe
x64\Release\SyntheticSelectKind.exe   > nul
echo Profiling x64/SyntheticSelectRandom.exe
x64\Release\SyntheticSelectRandom.exe > nul
echo Profiling x64/TypeSwitch.exe
x64\Release\TypeSwitch.exe            > nul

rem Optimize programs based on collected data
echo Build all sources for x64 optimized with PGO ...
echo ========== [ Visual C++/x64 optimize PGO ] ================================ >> %logfile%
echo ========== [ Visual C++/x64 optimize PGO ] ================================ >> %results%
MSBuild.exe PatternMatching.2010.sln /property:Configuration=Release /property:Platform=x64 /property:WholeProgramOptimization=PGOptimize >> %logfile% 2>&1

echo Running x64/Synthetic.exe
echo [ x64/Synthetic.exe ] ============================== | tee -a %logfile% >> %results%
rem x64\Release\Synthetic.exe              | tee -a %logfile% | grep "L.:" >> %results%
echo Running x64/GenericSelectRandom.exe
echo [ x64/GenericSelectRandom.exe ] ==================== | tee -a %logfile% >> %results%
x64\Release\GenericSelectRandom.exe    | tee -a %logfile% | grep "L.:" >> %results%
echo Running x64/SyntheticSelectRandom.exe
echo [ x64/SyntheticSelectRandom.exe ] ================== | tee -a %logfile% >> %results%
x64\Release\SyntheticSelectRandom.exe  | tee -a %logfile% | grep "L.:" >> %results%
echo Running x64/SyntheticSelect.exe
echo [ x64/SyntheticSelect.exe ] ======================== | tee -a %logfile% >> %results%
x64\Release\SyntheticSelect.exe        | tee -a %logfile% | grep "L.:" >> %results%
echo Running x64/Hierarchy.exe
echo [ x64/Hierarchy.exe ] ============================== | tee -a %logfile% >> %results%
x64\Release\Hierarchy.exe              | tee -a %logfile% | grep "L.:" >> %results%
echo Running x64/GenericSelectKind.exe
echo [ x64/GenericSelectKind.exe ] ====================== | tee -a %logfile% >> %results%
x64\Release\GenericSelectKind.exe      | tee -a %logfile% | grep "L.:" >> %results%
echo Running x64/SyntheticSelectKind.exe
echo [ x64/SyntheticSelectKind.exe ] ==================== | tee -a %logfile% >> %results%
x64\Release\SyntheticSelectKind.exe    | tee -a %logfile% | grep "L.:" >> %results%
echo Running x64/TypeSwitch.exe
echo [ x64/TypeSwitch.exe ] ============================= | tee -a %logfile% >> %results%
x64\Release\TypeSwitch.exe             | tee -a %logfile% | grep "L.:" >> %results%
echo Running x64/OCamlCmp.exe
echo [ x64/OCamlCmp.exe ] =============================== | tee -a %logfile% >> %results%
x64\Release\OCamlCmp.exe               | tee -a %logfile% | grep ":"   >> %results%
echo Running x64/OCamlCmpKind.exe
echo [ x64/OCamlCmpKind.exe ] =========================== | tee -a %logfile% >> %results%
x64\Release\OCamlCmpKind.exe           | tee -a %logfile% | grep ":"   >> %results%
echo Running x64/Numbers.exe
echo [ x64/Numbers.exe ] ================================ | tee -a %logfile% >> %results%
x64\Release\Numbers.exe                | tee -a %logfile% | grep "%%"  >> %results%

echo Finished on %date% at %time% >> %logfile%

:END
