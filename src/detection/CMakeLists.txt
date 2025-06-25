cmake_minimum_required(VERSION 3.10)

add_library(UltraFace STATIC UltraFace.cpp)

target_include_directories(UltraFace PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/3rdparty/ncnn/src)

target_link_libraries(
    UltraFace PRIVATE ncnn readerLib opencv_core opencv_imgproc)
