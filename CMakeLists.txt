cmake_minimum_required(VERSION 3.10)
project(Coordinator VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(httplib)
add_executable(Coordinator Coordinator.cxx Listener.cxx)
target_link_libraries(Coordinator PUBLIC httplib)

target_include_directories(Coordinator PRIVATE "${PROJECT_BINARY_DIR}")
