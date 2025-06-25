add_library(tracker tracker.cpp)

target_include_directories(tracker PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(tracker PRIVATE opencv_highgui opencv_core UltraFace)

