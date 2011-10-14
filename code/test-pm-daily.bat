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

set logfile=test-pm-daily.log
set results=test-pm-daily.txt

echo Test from %date% at %time% > %logfile%
echo Test from %date% at %time% > %results%

echo Building executables ...
make >> %logfile% 2>&1

echo Running synthetic.exe
echo [ synthetic.exe ] ========================== | tee -a %logfile% >> %results%
synthetic.exe                  | tee -a %logfile% | grep "L.:" >> %results%
echo Running synthetic_select_random.exe
echo [ synthetic_select_random.exe ] ============ | tee -a %logfile% >> %results%
synthetic_select_random.exe    | tee -a %logfile% | grep "L.:" >> %results%
echo Running synthetic_select.exe
echo [ synthetic_select.exe ] =================== | tee -a %logfile% >> %results%
synthetic_select.exe           | tee -a %logfile% | grep "L.:" >> %results%
echo Running hierarchy2.exe
echo [ hierarchy2.exe ] ========================= | tee -a %logfile% >> %results%
hierarchy2.exe                 | tee -a %logfile% | grep "L.:" >> %results%
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

rem Build OCaml sources

if not "%VS100COMNTOOLS%" == "" call "%VS100COMNTOOLS%vsvars32.bat" && echo Using MS Visual C++ 10.0 && goto PROCEED
if not "%VS90COMNTOOLS%"  == "" call "%VS90COMNTOOLS%vsvars32.bat"  && echo Using MS Visual C++ 9.0  && goto PROCEED
if not "%VS80COMNTOOLS%"  == "" call "%VS80COMNTOOLS%vsvars32.bat"  && echo Using MS Visual C++ 8.0  && goto PROCEED
if not "%VS71COMNTOOLS%"  == "" call "%VS71COMNTOOLS%vsvars32.bat"  && echo Using MS Visual C++ 7.1  && goto PROCEED

echo ERROR: Unable to find installation of MS Visual C++ on this computer
goto END

:PROCEED

rem Build OCaml sources

set PATH=%PATH%C:\Program Files (x86)\flexdll;
ocamlopt.opt unix.cmxa -oocaml_cmp_ml.exe ocaml_cmp.ml

:END
