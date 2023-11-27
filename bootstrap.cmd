@echo off
@rem Installing Catch2
if exist tmp rd /S /Q tmp
md tmp
cd tmp
git clone https://github.com/catchorg/Catch2.git
cd Catch2
@rem 3f0283de7a9c43200033da996ff9093be3ac84dc = v.3.3.2
@rem git checkout --detach 3f0283de7a9c43200033da996ff9093be3ac84dc
@rem 6e79e682b726f524310d55dec8ddac4e9c52fb5f = v.3.4.0
git checkout --detach 6e79e682b726f524310d55dec8ddac4e9c52fb5f
md build
cd build
cmake ..
cmake --build . --config Debug   --target INSTALL
cmake --build . --config Release --target INSTALL
cd ../..
cd ..
rd /S /Q tmp
