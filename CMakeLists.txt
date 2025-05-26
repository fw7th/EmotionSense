cmake_minimum_required(VERSION 3.10)
project(EmotionSense)
set(CMAKE_CXX_STANDARD 17)

# Add sub_dirs
add_subdirectory(src/frameReader)
add_subdirectory(src/detection)

add_executable(EmotionSense src/main.cpp)
target_link_libraries(EmotionSense PRIVATE readerLib UltraFace)
