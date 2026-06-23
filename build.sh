#!/usr/bin/env bash
set -e

mkdir -p build
cd build

cmake -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    ..

ninja

cd ..
echo "Built ./build/libqamera.so"

qpm qmod build
echo "Packaged ./qamera.qmod"
