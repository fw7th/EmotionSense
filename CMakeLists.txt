cmake_minimum_required(VERSION 3.10)
project(EmotionSense)
set(CMAKE_CXX_STANDARD 17)

# Dependencies
find_package(OpenCV REQUIRED)

# Add sub_dirs
add_subdirectory(lib/frame_reader)
add_subdirectory(lib/preprocessor)

add_executable(EmotionSense main.cpp)
target_link_libraries(EmotionSense PRIVATE reader_lib preprocess_lib)
