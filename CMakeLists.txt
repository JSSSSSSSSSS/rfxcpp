cmake_minimum_required(VERSION 3.21)
project(rfxcpp)

set(CMAKE_CXX_STANDARD 20)

add_executable(rfxcpp main.cpp)

add_subdirectory(librfx)
add_subdirectory(libbmp)
add_subdirectory(utils)
add_subdirectory(tests)

target_link_libraries(rfxcpp rfx)
target_link_libraries(rfxcpp bmp)