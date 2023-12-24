cmake_minimum_required(VERSION 3.1)

project(test VERSION 1.0 LANGUAGES CXX)

set(SRC test.cpp)

include_directories( . )
link_directories(${BUILD_DIR}/components/arg_parser)
add_executable(test ${SRC})
# add_definitions( -DMAIN_DIR="\\"${CMAKE_SOURCE_DIR}\\"" )

target_link_libraries(test arg_parser)
target_link_libraries(test PocoUtil PocoFoundation PocoNet )
