cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g")

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type (default Debug)" FORCE)
endif()

find_package(OpenCV REQUIRED)
find_package(OpenMP)
add_library(UltraFace UltraFace.cpp)

target_include_directories(UltraFace 
     PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/build/install/include/ncnn ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(
    UltraFace 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/build/src/libncnn.a 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/build/install/lib/libncnn.a 
    PUBLIC ${OpenCV_LIBS} readerLib)

if(OpenMP_CXX_FOUND)
    target_link_libraries(UltraFace PUBLIC OpenMP::OpenMP_CXX)
endif()
