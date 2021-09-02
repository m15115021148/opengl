rm -rf build
mkdir -p build
cd build

cmake \
    -D CMAKE_SYSTEM_NAME=Android \
    -D CMAKE_SYSTEM_VERSION=21 \
    -D ANDROID_PLATFORM=android-21 \
    -D CMAKE_BUILD_TYPE=Release \
    -D CMAKE_TOOLCHAIN_FILE=/usr/local/ndk/android-ndk-r21b/build/cmake/android.toolchain.cmake \
    -D CMAKE_ANDROID_NDK=/usr/local/ndk/android-ndk-r21b \
    -D CMAKE_ANDROID_ARCH_ABI=armeabi-v7a \
    ..


make -j4
