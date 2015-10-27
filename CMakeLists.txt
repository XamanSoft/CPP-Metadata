cmake_minimum_required (VERSION 3.2)
project (CppMetadata)

include(cxx11.cmake)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

include_directories(include)
add_library(CppMd SHARED ${SRC_FILES})

add_executable(CppMd-Test test/test.cpp)
target_link_libraries(CppMd-Test CppMd)