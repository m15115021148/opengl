#!/bin/bash

export NDK=/usr/local/ndk/android-ndk-r21b
TOOLCHAIN=$NDK/toolchains/llvm/prebuilt/linux-x86_64

function build_android
{

    echo "开始编译 $CPU"

    ./configure \
        --prefix=$PREFIX \
        --enable-neon  \
        --enable-hwaccels  \
        --enable-gpl   \
        --enable-postproc \
        --disable-shared \
        --disable-debug \
        --enable-small \
        --enable-jni \
        --enable-mediacodec \
        --enable-decoder=h264_mediacodec \
        --enable-static \
        --disable-doc \
        --enable-ffmpeg \
        --disable-ffplay \
        --disable-ffprobe \
        --disable-avdevice \
        --disable-doc \
        --disable-symver \
        --cross-prefix=$CROSS_PREFIX \
        --target-os=android \
        --arch=$ARCH \
        --cpu=$CPU \
        --cc=$CC \
        --cxx=$CXX \
        --enable-cross-compile \
        --sysroot=$SYSROOT \
        --extra-cflags="-Os -fpic $OPTIMIZE_CFLAGS" \
        --extra-ldflags="$ADDI_LDFLAGS"


    make clean
    make
    make install

    echo "编译成功 $CPU"

}
#armv7-a
ARCH=arm
CPU=armv7-a
API=16
CC=$TOOLCHAIN/bin/armv7a-linux-androideabi$API-clang
CXX=$TOOLCHAIN/bin/armv7a-linux-androideabi$API-clang++
SYSROOT=$NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot
CROSS_PREFIX=$TOOLCHAIN/bin/arm-linux-androideabi-
PREFIX=$(pwd)/android/$CPU
OPTIMIZE_CFLAGS="-mfloat-abi=softfp -mfpu=vfp -marm -march=$CPU "

build_android

#armv8-a
ARCH=arm64
CPU=armv8-a
API=21
CC=$TOOLCHAIN/bin/aarch64-linux-android$API-clang
CXX=$TOOLCHAIN/bin/aarch64-linux-android$API-clang++
SYSROOT=$NDK/toolchains/llvm/prebuilt/linux-x86_64/sysroot
CROSS_PREFIX=$TOOLCHAIN/bin/aarch64-linux-android-
PREFIX=$(pwd)/android/$CPU
OPTIMIZE_CFLAGS="-march=$CPU"

build_android
