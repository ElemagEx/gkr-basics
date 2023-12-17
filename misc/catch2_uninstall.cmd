@echo off
if not exist "%ProgramFiles(x86)%\Catch2" goto end

echo Removing previously installed Catch2 ...
rd /S /Q "%ProgramFiles(x86)%\Catch2"

:end
