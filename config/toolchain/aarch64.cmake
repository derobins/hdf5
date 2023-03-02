set(TOOLCHAIN_PREFIX aarch64-linux-gnu)
set(ANDROID_NDK /opt/android-ndk-linux)
set (CMAKE_SYSTEM_NAME Android)
set (CMAKE_ANDROID_ARCH_ABI x86_64)
#set (CMAKE_ANDROID_STANDALONE_TOOLCHAIN ${ANDROID_NDK}/build/cmake/andriod.toolchain.cmake)
set (CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
set (CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
#set (CMAKE_RC_COMPILER ${TOOLCHAIN_PREFIX}-windres)
set (CMAKE_Fortran_COMPILER ${TOOLCHAIN_PREFIX}-gfortran)
set (CMAKE_FIND_ROOT_PATH /usr/${TOOLCHAIN_PREFIX})
set (CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set (CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set (CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set (CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
set (CMAKE_CROSSCOMPILING_EMULATOR qemu-aarch64)

include_directories(/usr/${TOOLCHAIN_PREFIX}/include)

