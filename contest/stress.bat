@echo off
setlocal EnableExtensions DisableDelayedExpansion
cd /d "%~dp0"
if errorlevel 1 exit /b 2
set "limit=%~1"
if not defined limit set "limit=1000"
g++ -std=c++17 -O2 -Wall -Wextra "gen.cpp" -o "gen.exe"
if not "%errorlevel%"=="0" exit /b 2
g++ -std=c++17 -O2 -Wall -Wextra "brute.cpp" -o "brute.exe"
if not "%errorlevel%"=="0" exit /b 2
g++ -std=c++17 -O2 -Wall -Wextra "main.cpp" -o "main.exe"
if not "%errorlevel%"=="0" exit /b 2
set /a seed=0
:loop
set /a seed+=1
if %seed% GTR %limit% exit /b 0
>stress.seed echo %seed%
gen.exe %seed% >stress.in 2>gen.err
if not "%errorlevel%"=="0" goto generator_error
brute.exe <stress.in >stress.ans 2>brute.err
if not "%errorlevel%"=="0" goto brute_error
main.exe <stress.in >stress.out 2>main.err
if not "%errorlevel%"=="0" goto main_error
fc.exe /b stress.ans stress.out >stress.diff
if errorlevel 2 exit /b 2
if errorlevel 1 (
	echo WA #%seed%: inspect stress.in / stress.diff
	exit /b 1
)
echo AC #%seed%
goto loop
:generator_error
echo Generator failed: seed=%seed%
exit /b 2
:brute_error
echo Brute failed: seed=%seed%
exit /b 2
:main_error
echo Main failed: seed=%seed%
exit /b 2
