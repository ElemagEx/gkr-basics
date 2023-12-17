@echo off
if "%1" == "" (set catch2_tag=v3.5.0) else (set catch2_tag=%1)
echo Installing Catch2 %catch2_tag% ...
if exist tmp rd /S /Q tmp
md tmp
cd tmp
git clone --branch %catch2_tag% --depth 1 https://github.com/catchorg/Catch2
cd Catch2
md build
cd build
cmake ..
cmake --build . --config Debug   --target INSTALL
cmake --build . --config Release --target INSTALL
cd ..\..\..
rd /S /Q tmp
