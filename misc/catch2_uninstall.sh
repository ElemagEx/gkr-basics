#!/bin/bash
if [ "$1" ];
  then catch2_tag=$1
  else catch2_tag=v3.4.0
fi
echo Removing Catch2 $catch2_tag ...
rm -rf tmp
mkdir tmp
cd tmp
git clone --branch $catch2_tag --depth 1 https://github.com/catchorg/Catch2
cd Catch2
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make install
cat install_manifest.txt | xargs echo rm | sh
cmake -DCMAKE_BUILD_TYPE=Release ..
make install
cat install_manifest.txt | xargs echo rm | sh
cd ../..
cd ..
rm -rf tmp
