cmake_minimum_required (VERSION 3.2)
project (MetaData)

include(cxx11)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

include_directories(include/metadata)
add_library(MetaData ${SRC_FILES})