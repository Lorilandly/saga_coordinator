cmake_minimum_required(VERSION 3.11)
project(Coordinator VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(httplib URL https://github.com/yhirose/cpp-httplib/archive/refs/tags/v0.15.3.tar.gz)
FetchContent_MakeAvailable(httplib)

add_executable(Coordinator Coordinator.cxx Listener.cxx Transaction.cxx Steps.cxx)
target_link_libraries(Coordinator PUBLIC httplib nlohmann_json::nlohmann_json)

target_include_directories(Coordinator PRIVATE "${PROJECT_BINARY_DIR}")
