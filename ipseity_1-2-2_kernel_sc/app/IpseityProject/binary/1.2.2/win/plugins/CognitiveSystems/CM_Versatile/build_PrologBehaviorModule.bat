@echo off

set currentpath=%~dp0
call %currentpath%\linkcpl.bat %currentpath%\PrologBehaviorModule.dll %1 %2
