cmake_minimum_required(VERSION 3.10)
project(EmotionSense)
set(CMAKE_CXX_STANDARD 17)

# Add sub_dirs
add_subdirectory(src/frameReader)
add_subdirectory(src/detection)
add_subdirectory(src/tracking)

add_executable(emotion src/main.cpp)
target_link_libraries(emotion PRIVATE readerLib UltraFace tracker)
