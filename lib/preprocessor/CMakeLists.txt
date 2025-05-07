add_library(preprocess_lib preprocessor.cpp)
target_include_directories(preprocess_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(preprocess_lib PUBLIC ${OpenCV_LIBS} reader_lib)
