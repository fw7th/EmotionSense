cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_STANDARD 11)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type (default Debug)" FORCE)
endif()

add_subdirectory(3rdparty/ncnn ncnn_build)
add_subdirectory(detection/)
target_include_directories(UltraFace 3rdparty/ncnn/src src/UltraFace.hpp)

add_library(UltraFace)
find_package(OpenCV REQUIRED)

target_link_libraries(UltraFace src/UltraFace.cpp ncnn ${OpenCV_LIBS})
