add_library(tracker tracker.cpp)

target_include_directories(
    tracker 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${OpenCV_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/common
)

target_link_libraries(tracker PRIVATE ${OpenCV_LIBS})

