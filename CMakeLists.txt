cmake_minimum_required(VERSION 3.1...3.5)

if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR
        "In-source builds not allowed.
        Please make a new directory (called a build directory) and run CMake from there.
        You may need to remove CMakeCache.txt."
    )
endif()

# OS variables
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.14" CACHE STRING "macOS Deployment Target" FORCE)

# Project
project(whoa)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "dist" CACHE PATH "Installation prefix" FORCE)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 11)

include(lib/system/cmake/system.cmake)

find_package(ZLIB REQUIRED)
find_package(BZip2 REQUIRED)

# Some templates abuse offsetof
if(WHOA_SYSTEM_LINUX OR WHOA_SYSTEM_MAC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-invalid-offsetof")
endif()

# OS defines
if(WHOA_SYSTEM_WIN)
    # Avoid win32 header hell
    add_compile_definitions(
        NOMINMAX
        WIN32_LEAN_AND_MEAN
    )

    # Make DirectXMath boring
    add_definitions(
        -D_XM_NO_INTRINSICS_
    )

    # Lua uses various "unsafe" C functions
    add_definitions(
        -D_CRT_SECURE_NO_WARNINGS
    )
endif()

if(WHOA_SYSTEM_MAC)
    # Suppress OpenGL deprecation warnings
    add_definitions(
        -DGL_SILENCE_DEPRECATION
    )
endif()

# Threads
if(WHOA_SYSTEM_LINUX OR WHOA_SYSTEM_MAC)
    set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
    find_package(Threads REQUIRED)
endif()

# Library search paths
if(WHOA_SYSTEM_MAC)
    set(CMAKE_SKIP_BUILD_RPATH FALSE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    set(CMAKE_INSTALL_RPATH "@executable_path")
elseif(WHOA_SYSTEM_LINUX)
    set(CMAKE_SKIP_BUILD_RPATH FALSE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    set(CMAKE_INSTALL_RPATH "$ORIGIN")
endif()

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(vendor)
