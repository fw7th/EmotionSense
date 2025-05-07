add_library(reader_lib frame_reader.cpp)
target_include_directories(reader_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(reader_lib PUBLIC ${OpenCV_LIBS})
