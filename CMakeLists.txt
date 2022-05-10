cmake_minimum_required(VERSION 3.20)
project(Image_Format_Transform)

set(CMAKE_CXX_STANDARD 14)

# OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(main samples/main.cpp)
target_link_libraries(main ${OpenCV_LIBRARIES})