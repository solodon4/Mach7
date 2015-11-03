@echo off
IF "%1" == "" echo ERROR: argument with text file to process is missing
FOR /F "tokens=2,4,6,8,9,10 delims= " %%G IN (%1) DO call :PROCESS %%G %%H %%I %%J %%K %%L
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

echo \newcommand{\%F%%S%%E%%T%}{\%M%{%P%}}
goto END

:END