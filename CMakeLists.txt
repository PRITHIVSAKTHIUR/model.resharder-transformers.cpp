cmake_minimum_required(VERSION 3.14)
project(ModelResharderTransformersCPP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Python and Pybind11
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Fetch pybind11
include(FetchContent)
FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        v2.11.1
)
FetchContent_MakeAvailable(pybind11)

add_executable(resharder src/main.cpp src/resharder.cpp src/utils.cpp)
target_include_directories(resharder PRIVATE include)
target_link_libraries(resharder PRIVATE pybind11::embed)
