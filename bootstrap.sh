#!/bin/bash

rm -rf tmp
mkdir tmp
cd tmp
git clone https://github.com/catchorg/Catch2.git
cd Catch2
# 3f0283de7a9c43200033da996ff9093be3ac84dc = v.3.3.2
git checkout --detach 3f0283de7a9c43200033da996ff9093be3ac84dc
mkdir build
cd build
cmake ..
cmake --build . --config Debug   --target INSTALL
cmake --build . --config Release --target INSTALL
cd ../..
cd ..
rm -rf tmp
