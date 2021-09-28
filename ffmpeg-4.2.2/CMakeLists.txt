# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# now build app's shared lib
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

set(jnilibs ${CMAKE_SOURCE_DIR}/android)
set(libyuv ${CMAKE_SOURCE_DIR}/libyuv)
set(libname media-ffmpeg)

include_directories(
        include
        ${jnilibs}/${ANDROID_ABI}/include
)

link_directories(
        ${jnilibs}/${ANDROID_ABI}/lib)

# libyuv lib include	start
include_directories(
        include
        ${libyuv}/include
)

link_directories(
        ${libyuv}/${ANDROID_ABI})
# libyuv lib include	end

file(GLOB_RECURSE ALL_SOURCE "src/*.cpp" "src/*.h")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             ${libname}

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             ${ALL_SOURCE})
			 
			 
set(third-party-libs

        avformat
        avcodec
        avfilter
        swresample
        swscale
        avutil)

set(yuv-libs

	yuv)

set(native-libs

        android
        EGL
        GLESv3
        OpenSLES
        log
        m
        z)

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.


# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       ${libname}

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib}
                       ${third-party-libs}
					   ${yuv-libs}
                       ${native-libs})
