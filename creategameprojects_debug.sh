#!/bin/bash

echo Build bzip2
pushd ./thirdparty/bzip2
make
popd

echo Build libjpeg-turbo
mkdir ./thirdparty/libjpeg-turbo/out
pushd ./thirdparty/libjpeg-turbo/out
cmake -G "Ninja" -DENABLE_SHARED=OFF -DENABLE_STATIC=ON -DWITH_CDJPEG=OFF -DWITH_TURBOJPEG=OFF ..
cmake --build . --config Release
popd

echo Build zlib
mkdir ./thirdparty/zlib/out
pushd ./thirdparty/zlib/out
cmake -G "Ninja" ..
cmake --build . --config Release
popd

ZLIB_SOURCE_DIR=$pwd/thirdparty/zlib

echo Build libpng
mkdir ./thirdparty/libpng/out
pushd ./thirdparty/libpng/out
cmake -G "Ninja" -DPNG_BUILD_ZLIB=ON -DPNG_SHARED=OFF -DPNG_STATIC=ON -DPNG_EXECUTABLES=OFF -DPNG_TESTS=OFF -DZLIB_INCLUDE_DIRS=$ZLIB_SOURCE_DIR;$ZLIB_SOURCE_DIR/out -DZLIB_LIBRARIES=$ZLIB_SOURCE_DIR/out/zlibstatic.a ..
cmake --build . --config Release
popd

echo Build SDL
mkdir ./thirdparty/SDL/out
pushd ./thirdparty/SDL/out
cmake -G "Ninja" -DSDL_TEST=OFF ..
cmake --build . --config Release
popd

echo Build protobuf
pushd ./thirdparty/protobuf
./autogen.sh
./configure
make
popd

echo Build cryptopp
pushd ./thirdparty/cryptopp
./TestScripts/cryptest-autotools.sh
popd

echo Build vpc
./external/vpc/devtools/bin/vpc ./devtools/bin/.
./devtools/bin/vpc_linux /2015 /define:WORKSHOP_IMPORT_DISABLE /define:SIXENSE_DISABLE /define:NO_X360_XDK /define:RAD_TELEMETRY_DISABLED /define:DISABLE_ETW /define:NO_STEAM /define:NO_ATI_COMPRESS /define:NO_NVTC /define:LTCG /no_ceg /nofpo /gmod +game /mksln gmod.sln
