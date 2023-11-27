#!/bin/sh
rm -rf tmp
mkdir tmp
cd tmp
git clone https://github.com/catchorg/Catch2.git
cd Catch2
# 3f0283de7a9c43200033da996ff9093be3ac84dc = v.3.3.2
# git checkout --detach 3f0283de7a9c43200033da996ff9093be3ac84dc
# 6e79e682b726f524310d55dec8ddac4e9c52fb5f = v.3.4.0
git checkout --detach 6e79e682b726f524310d55dec8ddac4e9c52fb5f
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make install
cmake -DCMAKE_BUILD_TYPE=Release ..
make install
cd ../..
cd ..
rm -rf tmp
#cat install_manifest.txt | xargs echo sudo rm | sh
