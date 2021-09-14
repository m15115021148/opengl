rm -rf build_android_v7a
mkdir -p build_android_v7a
cd build_android_v7a

#	-D ASSIMP_BUILD_FBX_IMPORTER=TRUE \

cmake \
    -D CMAKE_SYSTEM_NAME=Android \
    -D CMAKE_SYSTEM_VERSION=21 \
    -D ANDROID_PLATFORM=android-21 \
    -D CMAKE_BUILD_TYPE=Release \
	-D ASSIMP_ANDROID_JNIIOSYSTEM=ON \
	-D ANDROID_FORCE_ARM_BUILD=TRUE \
    -D CMAKE_TOOLCHAIN_FILE=/usr/local/ndk/android-ndk-r21b/build/cmake/android.toolchain.cmake \
    -D CMAKE_ANDROID_NDK=/usr/local/ndk/android-ndk-r21b \
    -D CMAKE_ANDROID_ARCH_ABI=armeabi-v7a \
	-D ASSIMP_BUILD_OBJ_IMPORTER=TRUE \
	-D ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT=FALSE \
	-D ASSIMP_BUILD_TESTS=OFF \
	-D ASSIMP_NO_EXPORT=TRUE \
	-D ANDROID_STL=c++_static \
	-D CMAKE_CXX_FLAGS=-Wno-c++11-narrowing \
	-D ANDROID_TOOLCHAIN=clang \
    ..


make -j8