#!/bin/bash

make O=out ARCH=arm64 lineageos_h872_defconfig

make -j$(nproc --all) O=out \
                      ARCH=arm64 \
                      CC="/home/eliminater74/Builds/kernel/toolchains/clang_linux-x86_7.x/bin/clang" \
                      CLANG_TRIPLE=aarch64-linux-gnu- \
                      CROSS_COMPILE="/home/eliminater74/Builds/kernel/toolchains/PureFusionTC/bin/aarch64-linux-android-"
