::
:: Script for testing C++ pattern matching library
::
:: Written by Yuriy Solodkyy <yuriy.solodkyy@gmail.com>
:: Version 1.0 from 21st July 2011
::
:: Usage: test-pm-timing.bat
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

set logfile=test-pm-timing.log
set results=test-pm-timing.txt

echo Test from %date% at %time% > %logfile%
echo Test from %date% at %time% > %results%

set CXX=cl.exe
rem /Zi /nologo /W3 /WX- /O2 /Ob2 /Oi /Ot /Oy- /GL  /GF /Gm- /MT /GS- /Gy- /fp:precise /Zc:wchar_t /Zc:forScope /Gr /analyze- /errorReport:queue 
rem set CXXFLAGS=/W4 /O2 /Ob2 /Oi /Ot /Og /GR /GL /GF /GS- /Gy- /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /I%BOOST%
rem /INCREMENTAL:NO /NOLOGO /SUBSYSTEM:CONSOLE /OPT:REF /OPT:ICF /LTCG /TLBID:1 /DYNAMICBASE:NO /NXCOMPAT /MACHINE:X86 /ERRORREPORT:QUEUE 
rem set LNKFLAGS=/LTCG /MACHINE:X86

set CXXFLAGS=/I%BOOST% /Zi /nologo /W3 /WX- /O2 /Ob2 /Oi /Ot /Oy- /GL /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /GF /Gm- /MT /GS- /Gy- /fp:precise /Zc:wchar_t /Zc:forScope /Gr /analyze- /errorReport:queue 
rem Slower: set CXXFLAGS=/I%BOOST% /Zi /nologo /W3 /WX- /O2 /Ob2 /Oi /Ot      /GL /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /GF /Gm- /MT /GS- /Gy  /fp:precise /Zc:wchar_t /Zc:forScope /Gr           /errorReport:queue 
set LNKFLAGS=/INCREMENTAL:NO /NOLOGO "kernel32.lib" "user32.lib" "gdi32.lib" "winspool.lib" "comdlg32.lib" "advapi32.lib" "shell32.lib" "ole32.lib" "oleaut32.lib" "uuid.lib" "odbc32.lib" "odbccp32.lib" /MANIFEST:NO /ALLOWISOLATION /SUBSYSTEM:CONSOLE /OPT:REF /OPT:ICF /LTCG /TLBID:1 /DYNAMICBASE:NO /NXCOMPAT /ERRORREPORT:QUEUE 

echo [ MS Visual C++/Win32 ] ========================= >> %logfile%

rem Set up VC variables for x86 build
call "%VCINSTALLDIR%vcvarsall.bat" x86

%CXX% %CXXFLAGS% exception_select_random.cpp /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% generic_select_kind.cpp     /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% generic_select_random.cpp   /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% generic_select_union.cpp    /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% hierarchy2.cpp              /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% ocaml_cmp.cpp               /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% ocaml_cmp_kind.cpp          /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% synthetic.cpp               /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% synthetic_dynamic_cast.cpp  /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% synthetic_select.cpp        /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% synthetic_select_kind.cpp   /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% synthetic_select_random.cpp /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% type_switch.cpp             /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1

goto END

%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='p' /DXTL_REP_TEST skeleton.cxx /Fetime-32-fwd-generic-poly-rep.exe /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='p' /DXTL_SEQ_TEST skeleton.cxx /Fetime-32-fwd-generic-poly-seq.exe /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='p' /DXTL_RND_TEST skeleton.cxx /Fetime-32-fwd-generic-poly-rnd.exe /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='k' /DXTL_REP_TEST skeleton.cxx /Fetime-32-fwd-generic-kind-rep.exe /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='k' /DXTL_SEQ_TEST skeleton.cxx /Fetime-32-fwd-generic-kind-seq.exe /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='k' /DXTL_RND_TEST skeleton.cxx /Fetime-32-fwd-generic-kind-rnd.exe /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='P' /DXTL_REP_TEST skeleton.cxx /Fetime-32-fwd-special-poly-rep.exe /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='P' /DXTL_SEQ_TEST skeleton.cxx /Fetime-32-fwd-special-poly-seq.exe /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='P' /DXTL_RND_TEST skeleton.cxx /Fetime-32-fwd-special-poly-rnd.exe /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='K' /DXTL_REP_TEST skeleton.cxx /Fetime-32-fwd-special-kind-rep.exe /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='K' /DXTL_SEQ_TEST skeleton.cxx /Fetime-32-fwd-special-kind-seq.exe /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='K' /DXTL_RND_TEST skeleton.cxx /Fetime-32-fwd-special-kind-rnd.exe /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='p' /DXTL_REP_TEST skeleton.cxx /Fetime-32-non-generic-poly-rep.exe /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='p' /DXTL_SEQ_TEST skeleton.cxx /Fetime-32-non-generic-poly-seq.exe /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='p' /DXTL_RND_TEST skeleton.cxx /Fetime-32-non-generic-poly-rnd.exe /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='k' /DXTL_REP_TEST skeleton.cxx /Fetime-32-non-generic-kind-rep.exe /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='k' /DXTL_SEQ_TEST skeleton.cxx /Fetime-32-non-generic-kind-seq.exe /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='k' /DXTL_RND_TEST skeleton.cxx /Fetime-32-non-generic-kind-rnd.exe /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='P' /DXTL_REP_TEST skeleton.cxx /Fetime-32-non-special-poly-rep.exe /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='P' /DXTL_SEQ_TEST skeleton.cxx /Fetime-32-non-special-poly-seq.exe /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='P' /DXTL_RND_TEST skeleton.cxx /Fetime-32-non-special-poly-rnd.exe /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='K' /DXTL_REP_TEST skeleton.cxx /Fetime-32-non-special-kind-rep.exe /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='K' /DXTL_SEQ_TEST skeleton.cxx /Fetime-32-non-special-kind-seq.exe /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='K' /DXTL_RND_TEST skeleton.cxx /Fetime-32-non-special-kind-rnd.exe /link %LNKFLAGS% /MACHINE:X86 >> %logfile% 2>&1

echo [ MS Visual C++/x64 ] ========================= >> %logfile%

rem Set up VC variables for x64 build
call "%VCINSTALLDIR%vcvarsall.bat" x64

%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='p' /DXTL_REP_TEST skeleton.cxx /Fetime-64-fwd-generic-poly-rep.exe /link %LNKFLAGS% /MACHINE:X64 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='p' /DXTL_SEQ_TEST skeleton.cxx /Fetime-64-fwd-generic-poly-seq.exe /link %LNKFLAGS% /MACHINE:X64 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='p' /DXTL_RND_TEST skeleton.cxx /Fetime-64-fwd-generic-poly-rnd.exe /link %LNKFLAGS% /MACHINE:X64 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='k' /DXTL_REP_TEST skeleton.cxx /Fetime-64-fwd-generic-kind-rep.exe /link %LNKFLAGS% /MACHINE:X64 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='k' /DXTL_SEQ_TEST skeleton.cxx /Fetime-64-fwd-generic-kind-seq.exe /link %LNKFLAGS% /MACHINE:X64 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='k' /DXTL_RND_TEST skeleton.cxx /Fetime-64-fwd-generic-kind-rnd.exe /link %LNKFLAGS% /MACHINE:X64 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='P' /DXTL_REP_TEST skeleton.cxx /Fetime-64-fwd-special-poly-rep.exe /link %LNKFLAGS% /MACHINE:X64 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='P' /DXTL_SEQ_TEST skeleton.cxx /Fetime-64-fwd-special-poly-seq.exe /link %LNKFLAGS% /MACHINE:X64 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='P' /DXTL_RND_TEST skeleton.cxx /Fetime-64-fwd-special-poly-rnd.exe /link %LNKFLAGS% /MACHINE:X64 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='K' /DXTL_REP_TEST skeleton.cxx /Fetime-64-fwd-special-kind-rep.exe /link %LNKFLAGS% /MACHINE:X64 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='K' /DXTL_SEQ_TEST skeleton.cxx /Fetime-64-fwd-special-kind-seq.exe /link %LNKFLAGS% /MACHINE:X64 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='K' /DXTL_RND_TEST skeleton.cxx /Fetime-64-fwd-special-kind-rnd.exe /link %LNKFLAGS% /MACHINE:X64 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='p' /DXTL_REP_TEST skeleton.cxx /Fetime-64-non-generic-poly-rep.exe /link %LNKFLAGS% /MACHINE:X64 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='p' /DXTL_SEQ_TEST skeleton.cxx /Fetime-64-non-generic-poly-seq.exe /link %LNKFLAGS% /MACHINE:X64 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='p' /DXTL_RND_TEST skeleton.cxx /Fetime-64-non-generic-poly-rnd.exe /link %LNKFLAGS% /MACHINE:X64 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='k' /DXTL_REP_TEST skeleton.cxx /Fetime-64-non-generic-kind-rep.exe /link %LNKFLAGS% /MACHINE:X64 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='k' /DXTL_SEQ_TEST skeleton.cxx /Fetime-64-non-generic-kind-seq.exe /link %LNKFLAGS% /MACHINE:X64 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='k' /DXTL_RND_TEST skeleton.cxx /Fetime-64-non-generic-kind-rnd.exe /link %LNKFLAGS% /MACHINE:X64 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='P' /DXTL_REP_TEST skeleton.cxx /Fetime-64-non-special-poly-rep.exe /link %LNKFLAGS% /MACHINE:X64 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='P' /DXTL_SEQ_TEST skeleton.cxx /Fetime-64-non-special-poly-seq.exe /link %LNKFLAGS% /MACHINE:X64 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='P' /DXTL_RND_TEST skeleton.cxx /Fetime-64-non-special-poly-rnd.exe /link %LNKFLAGS% /MACHINE:X64 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='K' /DXTL_REP_TEST skeleton.cxx /Fetime-64-non-special-kind-rep.exe /link %LNKFLAGS% /MACHINE:X64 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='K' /DXTL_SEQ_TEST skeleton.cxx /Fetime-64-non-special-kind-seq.exe /link %LNKFLAGS% /MACHINE:X64 >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='K' /DXTL_RND_TEST skeleton.cxx /Fetime-64-non-special-kind-rnd.exe /link %LNKFLAGS% /MACHINE:X64 >> %logfile% 2>&1

echo Running timings ...

for %%i in (time-*.exe) do echo %%i & echo ==================[ %%i ]===================  >> %results% & %%i >> %results% 2>&1 

:END
