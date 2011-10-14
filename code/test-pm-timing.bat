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

set CXX="%VCINSTALLDIR%\bin\cl.exe"
rem /Zi /nologo /W3 /WX- /O2 /Ob2 /Oi /Ot /Oy- /GL  /GF /Gm- /MT /GS- /Gy- /fp:precise /Zc:wchar_t /Zc:forScope /Gr /analyze- /errorReport:queue 
set CXXFLAGS=/W4 /O2 /Ob2 /Oi /Ot /Og /GR /GL /GF /GS- /Gy- /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /I%BOOST%
rem /INCREMENTAL:NO /NOLOGO /SUBSYSTEM:CONSOLE /OPT:REF /OPT:ICF /LTCG /TLBID:1 /DYNAMICBASE:NO /NXCOMPAT /MACHINE:X86 /ERRORREPORT:QUEUE 
set LNKFLAGS=/LTCG /MACHINE:X86

echo Building executables for Win32 using MS Visual C++ ...
echo [ MS Visual C++/Win32 ] ========================= >> %logfile%

%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='p' /DXTL_REP_TEST skeleton.cxx /Fetime-fwd-generic-poly-rep.exe /link %LNKFLAGS% >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='p' /DXTL_SEQ_TEST skeleton.cxx /Fetime-fwd-generic-poly-seq.exe /link %LNKFLAGS% >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='p' /DXTL_RND_TEST skeleton.cxx /Fetime-fwd-generic-poly-rnd.exe /link %LNKFLAGS% >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='k' /DXTL_REP_TEST skeleton.cxx /Fetime-fwd-generic-kind-rep.exe /link %LNKFLAGS% >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='k' /DXTL_SEQ_TEST skeleton.cxx /Fetime-fwd-generic-kind-seq.exe /link %LNKFLAGS% >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='k' /DXTL_RND_TEST skeleton.cxx /Fetime-fwd-generic-kind-rnd.exe /link %LNKFLAGS% >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='P' /DXTL_REP_TEST skeleton.cxx /Fetime-fwd-special-poly-rep.exe /link %LNKFLAGS% >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='P' /DXTL_SEQ_TEST skeleton.cxx /Fetime-fwd-special-poly-seq.exe /link %LNKFLAGS% >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='P' /DXTL_RND_TEST skeleton.cxx /Fetime-fwd-special-poly-rnd.exe /link %LNKFLAGS% >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='K' /DXTL_REP_TEST skeleton.cxx /Fetime-fwd-special-kind-rep.exe /link %LNKFLAGS% >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='K' /DXTL_SEQ_TEST skeleton.cxx /Fetime-fwd-special-kind-seq.exe /link %LNKFLAGS% >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=1 /DXTL_DEFAULT_SYNTAX='K' /DXTL_RND_TEST skeleton.cxx /Fetime-fwd-special-kind-rnd.exe /link %LNKFLAGS% >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='p' /DXTL_REP_TEST skeleton.cxx /Fetime-non-generic-poly-rep.exe /link %LNKFLAGS% >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='p' /DXTL_SEQ_TEST skeleton.cxx /Fetime-non-generic-poly-seq.exe /link %LNKFLAGS% >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='p' /DXTL_RND_TEST skeleton.cxx /Fetime-non-generic-poly-rnd.exe /link %LNKFLAGS% >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='k' /DXTL_REP_TEST skeleton.cxx /Fetime-non-generic-kind-rep.exe /link %LNKFLAGS% >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='k' /DXTL_SEQ_TEST skeleton.cxx /Fetime-non-generic-kind-seq.exe /link %LNKFLAGS% >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='k' /DXTL_RND_TEST skeleton.cxx /Fetime-non-generic-kind-rnd.exe /link %LNKFLAGS% >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='P' /DXTL_REP_TEST skeleton.cxx /Fetime-non-special-poly-rep.exe /link %LNKFLAGS% >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='P' /DXTL_SEQ_TEST skeleton.cxx /Fetime-non-special-poly-seq.exe /link %LNKFLAGS% >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='P' /DXTL_RND_TEST skeleton.cxx /Fetime-non-special-poly-rnd.exe /link %LNKFLAGS% >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='K' /DXTL_REP_TEST skeleton.cxx /Fetime-non-special-kind-rep.exe /link %LNKFLAGS% >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='K' /DXTL_SEQ_TEST skeleton.cxx /Fetime-non-special-kind-seq.exe /link %LNKFLAGS% >> %logfile% 2>&1
%CXX% %CXXFLAGS% /DXTL_VISITOR_FORWARDING=0 /DXTL_DEFAULT_SYNTAX='K' /DXTL_RND_TEST skeleton.cxx /Fetime-non-special-kind-rnd.exe /link %LNKFLAGS% >> %logfile% 2>&1

echo [ Running Win32 timings ] ======================= >> %logfile%

for %%i in (time-*.exe) do %%i >> %results% 2>&1 

:END
