cmake_minimum_required(VERSION 3.1)

project(client VERSION 1.0 LANGUAGES CXX)

set(SRC client.cpp)

include_directories( . ${CMAKE_SOURCE_DIR} )
link_directories(${BUILD_DIR}/components/arg_parser ${BUILD_DIR}/components/message_decoder)
add_executable(client ${SRC})
# add_definitions( -DMAIN_DIR="\\"${CMAKE_SOURCE_DIR}\\"" )

target_link_libraries(client arg_parser message_decoder)
target_link_libraries(client PocoUtil PocoFoundation PocoNet )
