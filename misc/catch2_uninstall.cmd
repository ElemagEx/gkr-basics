@echo off

echo Removing previously installed Catch2 ...

if not exist "%ProgramFiles%\Catch2" goto next

rd /S /Q "%ProgramFiles%\Catch2"

:next

if not exist "%ProgramFiles(x86)%\Catch2" goto end

rd /S /Q "%ProgramFiles(x86)%\Catch2"

:end
