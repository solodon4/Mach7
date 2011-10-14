::
:: Script for testing C++ pattern matching library
::
:: Written by Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
:: Version 1.0 from 30th April 2011
::
:: Usage: test-pm-daily.bat
::                                                                           
:: This file is a part of the XTL framework (http://parasol.tamu.edu/xtl/).
:: Copyright (C) 2005-2011 Texas A&M University.
:: All rights reserved.
:: 

@echo off

if "%1" == "/?" findstr "^::" "%~f0" & goto END

if not "%VS100COMNTOOLS%" == "" call "%VS100COMNTOOLS%vsvars32.bat" && echo Using MS Visual C++ 10.0 && goto PROCEED
if not "%VS90COMNTOOLS%"  == "" call "%VS90COMNTOOLS%vsvars32.bat"  && echo Using MS Visual C++ 9.0  && goto PROCEED
if not "%VS80COMNTOOLS%"  == "" call "%VS80COMNTOOLS%vsvars32.bat"  && echo Using MS Visual C++ 8.0  && goto PROCEED
if not "%VS71COMNTOOLS%"  == "" call "%VS71COMNTOOLS%vsvars32.bat"  && echo Using MS Visual C++ 7.1  && goto PROCEED

echo ERROR: Unable to find installation of MS Visual C++ on this computer
goto END

:PROCEED

set logfile=test-pm-daily.log
set results=test-pm-daily.txt

echo Test from %date% at %time% > %logfile%
echo Test from %date% at %time% > %results%

echo Building executables for Win32 using G++ ...
echo [ G++/Win32 ] =================================== >> %logfile%
make >> %logfile% 2>&1
echo Building executables for Win32 using MS Visual C++ ...
echo [ MS Visual C++/Win32 ] ========================= >> %logfile%
devenv PatternMatching.2010.sln /build "Release|Win32" >> %logfile% 2>&1
echo Building executables for x64   using MS Visual C++ ...
echo [ MS Visual C++/x64 ] =========================== >> %logfile%
devenv PatternMatching.2010.sln /build "Release|x64"   >> %logfile% 2>&1
echo Building OCaml sources ...
echo [ OCaml ] ======================================= >> %logfile%
set PATH=%PATH%C:\Program Files (x86)\flexdll;         >> %logfile% 2>&1
ocamlopt.opt unix.cmxa -o ocaml_cmp_ml.exe ocaml_cmp.ml>> %logfile% 2>&1

rem -----------------------------------------------------------------------------

echo Running synthetic.exe
echo [ synthetic.exe ] ========================== | tee -a %logfile% >> %results%
rem synthetic.exe                  | tee -a %logfile% | grep "L.:" >> %results%
echo Running generic_select_random.exe
echo [ generic_select_random.exe ] ============== | tee -a %logfile% >> %results%
generic_select_random.exe      | tee -a %logfile% | grep "L.:" >> %results%
echo Running synthetic_select_random.exe
echo [ synthetic_select_random.exe ] ============ | tee -a %logfile% >> %results%
synthetic_select_random.exe    | tee -a %logfile% | grep "L.:" >> %results%
echo Running synthetic_select.exe
echo [ synthetic_select.exe ] =================== | tee -a %logfile% >> %results%
synthetic_select.exe           | tee -a %logfile% | grep "L.:" >> %results%
echo Running hierarchy2.exe
echo [ hierarchy2.exe ] ========================= | tee -a %logfile% >> %results%
hierarchy2.exe                 | tee -a %logfile% | grep "L.:" >> %results%
echo Running generic_select_kind.exe
echo [ generic_select_kind.exe ] ================ | tee -a %logfile% >> %results%
generic_select_kind.exe        | tee -a %logfile% | grep "L.:" >> %results%
echo Running synthetic_select_kind.exe
echo [ synthetic_select_kind.exe ] ============== | tee -a %logfile% >> %results%
synthetic_select_kind.exe      | tee -a %logfile% | grep "L.:" >> %results%
echo Running type_switch.exe
echo [ type_switch.exe ] ======================== | tee -a %logfile% >> %results%
type_switch.exe                | tee -a %logfile% | grep "L.:" >> %results%
echo Running ocaml_cmp.exe
echo [ ocaml_cmp.exe ] ========================== | tee -a %logfile% >> %results%
ocaml_cmp.exe                  | tee -a %logfile% | grep ":"   >> %results%
echo Running ocaml_cmp_kind.exe
echo [ ocaml_cmp_kind.exe ] ===================== | tee -a %logfile% >> %results%
ocaml_cmp_kind.exe             | tee -a %logfile% | grep ":"   >> %results%
echo Running numbers.exe
echo [ numbers.exe ] ============================ | tee -a %logfile% >> %results%
numbers.exe                    | tee -a %logfile% | grep "%%"  >> %results%

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

:END
