cmake_minimum_required(VERSION 3.1)

project(BlessedChatServer VERSION 1.0 LANGUAGES CXX)

set(SRC src/Server.cpp)

link_directories(${BUILD_DIR}/components/arg_parser)
add_executable(BlessedChatServer ${SRC})

target_link_libraries(BlessedChatServer arg_parser)
target_link_libraries(BlessedChatServer PocoUtil PocoFoundation )
