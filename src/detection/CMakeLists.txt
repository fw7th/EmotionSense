cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g")

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type (default Debug)" FORCE)
endif()

add_subdirectory(3rdparty/ncnn ncnn_build)

add_library(UltraFace UltraFace.cpp)

target_include_directories(UltraFace 
     PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ncnn_build/install/include/ncnn ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(OpenCV REQUIRED)

target_link_libraries(
    UltraFace 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ncnn_build/src/libncnn.a 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ncnn_build/install/lib/libncnn.a 
    PUBLIC ${OpenCV_LIBS} readerLib)
