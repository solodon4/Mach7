@echo off

echo ^% This file defines variables with performance numbers for the table in Evaluation section > data.tex

set results=test-pm-timing.txt
echo Test from %date% at %time% > %results%

echo Timings for x86 unoptimized > temp.tmp
for %%i in (time-32-*.exe)     do echo %%i & echo ==================[ %%i ]===================  >> temp.tmp & %%i >> temp.tmp 2>&1 
for /F "tokens=2,4,6,8,9,10 delims= " %%G in ('grep SYNTAX temp.tmp') do call :PROCESS %%G %%H %%I %%J %%K %%L v w
type temp.tmp >> %results%

echo Timings for x64 unoptimized > temp.tmp
for %%i in (time-64-*.exe)     do echo %%i & echo ==================[ %%i ]===================  >> temp.tmp & %%i >> temp.tmp 2>&1 
for /F "tokens=2,4,6,8,9,10 delims= " %%G in ('grep SYNTAX temp.tmp') do call :PROCESS %%G %%H %%I %%J %%K %%L v x
type temp.tmp >> %results%

echo Timings for x86 optimized   > temp.tmp
for %%i in (time-pgo-32-*.exe) do echo %%i & echo ==================[ %%i ]===================  >> temp.tmp & %%i >> temp.tmp 2>&1 
for /F "tokens=2,4,6,8,9,10 delims= " %%G in ('grep SYNTAX temp.tmp') do call :PROCESS %%G %%H %%I %%J %%K %%L V w
type temp.tmp >> %results%

echo Timings for x64 optimized   > temp.tmp
for %%i in (time-pgo-64-*.exe) do echo %%i & echo ==================[ %%i ]===================  >> temp.tmp & %%i >> temp.tmp 2>&1 
for /F "tokens=2,4,6,8,9,10 delims= " %%G in ('grep SYNTAX temp.tmp') do call :PROCESS %%G %%H %%I %%J %%K %%L V x
type temp.tmp >> %results%

del temp.tmp

goto END

:PROCESS
rem echo %1 %2 %3 %4 %5 %6
set S=%1
set E=%2
set F=%3
set T=%4
set P=%5
set M=%6

set S=%S:~0,1%
set E=%E:~0,1%
set F=%F:~0,1%
set T=%T:~2,1%
rem set P=%P:~0,1%
set M=%M:~0,1%

rem IF "%1" == "Generic" (set S=g) ELSE (set S=s)
rem IF "%2" == "Polymorphic" set E=p
rem IF "%2" == "Kind"        set E=k
rem IF "%3" == "Yes"     (set F=f) ELSE (set F=n)
rem IF "%2" == "Kind"        set E=k

echo \newcommand{\%7%8%F%%S%%E%%T%}{\%M%{%P%}} >> data.tex
goto END

:END