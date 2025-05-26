add_library(readerLib frameReader.cpp)
target_include_directories(readerLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
find_package(OpenCV REQUIRED)
target_link_libraries(readerLib PUBLIC ${OpenCV_LIBS})
