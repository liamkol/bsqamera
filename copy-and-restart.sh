#!/usr/bin/env bash
set -e

PKG="com.beatgames.beatsaber"
SO_PATH="./build/libqamera.so"

if [ ! -f "$SO_PATH" ]; then
    echo "libqamera.so not found, run build.sh first." >&2
    exit 1
fi

adb shell am force-stop "$PKG"
adb push "$SO_PATH" "/sdcard/ModData/$PKG/Modloader/early_mods/libqamera.so"
adb shell am start -n "$PKG/com.unity3d.player.UnityPlayerActivity"

echo "Pushed libqamera.so and restarted Beat Saber."
