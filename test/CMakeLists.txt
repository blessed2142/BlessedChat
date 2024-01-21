cmake_minimum_required(VERSION 3.1)

project(test VERSION 1.0 LANGUAGES CXX)

set(SRC test.cpp)

message("Blessed ${CMAKE_SOURCE_DIR}")
include_directories( . ${CMAKE_SOURCE_DIR} )
link_directories(${BUILD_DIR}/components/arg_parser)
link_directories(${BUILD_DIR}/components/utils)
add_executable(test ${SRC})
add_compile_options( -gdb )
# add_definitions( -DMAIN_DIR="\\"${CMAKE_SOURCE_DIR}\\"" )

target_link_libraries(test arg_parser utils)
target_link_libraries(test PocoUtil PocoFoundation PocoNet )
