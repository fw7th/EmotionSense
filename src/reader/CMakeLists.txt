add_library(readerLib STATIC reader.cpp)

target_include_directories(readerLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(readerLib PRIVATE opencv_core opencv_imgproc videoio)
