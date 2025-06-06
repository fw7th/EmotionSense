add_library(tracker tracker.cpp)

find_package(OpenCV REQUIRED)

target_include_directories(tracker PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}byte-track-eigen/include ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${OpenCV_INCLUDE_DIRS}
)
target_link_libraries(tracker PUBLIC ${OpenCV_LIBS} UltraFace)

