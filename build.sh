#!/usr/bin/env bash
set -e

if [ -z "$ANDROID_NDK_HOME" ]; then
    echo "ANDROID_NDK_HOME is not set. Point it at NDK r27+." >&2
    exit 1
fi

mkdir -p build
cd build

cmake -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE="$ANDROID_NDK_HOME/build/cmake/android.toolchain.cmake" \
    -DANDROID_ABI=arm64-v8a \
    -DANDROID_PLATFORM=android-24 \
    -DCMAKE_BUILD_TYPE=Release \
    ..

ninja

cd ..
echo "Built ./build/libqamera.so"

qpm qmod build
echo "Packaged ./qamera.qmod"
