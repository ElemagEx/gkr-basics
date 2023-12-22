@echo off
if "%1" == "" (set catch2_tag=v3.5.0) else (set catch2_tag=%1)
echo Installing Catch2 %catch2_tag% ...
if exist tmp rd /S /Q tmp
md tmp
cd tmp
git clone --branch %catch2_tag% --depth 1 https://github.com/catchorg/Catch2
cd Catch2
cmake -A x64   -S . -B x64-build
cmake -A Win32 -S . -B x86-build
cmake --build x64-build --config Debug   --target INSTALL
cmake --build x64-build --config Release --target INSTALL
cmake --build x86-build --config Debug   --target INSTALL
cmake --build x86-build --config Release --target INSTALL
cd ..\..
rd /S /Q tmp
