cmake_minimum_required(VERSION 3.1)

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

set(CMAKE_BUILD_TYPE Debug)

# Some templates abuse offsetof
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-invalid-offsetof")

# OS defines
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_definitions(
        -DGL_SILENCE_DEPRECATION
    )
endif()

include(lib/system/cmake/system.cmake)

# Threads
if(WHOA_SYSTEM_LINUX OR WHOA_SYSTEM_MAC)
    set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
    find_package(Threads REQUIRED)
endif()

add_subdirectory(lib)
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(vendor)
