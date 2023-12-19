cmake_minimum_required(VERSION 3.1)

project(arg_parser VERSION 1.0 LANGUAGES CXX)

set(SRC poco_arg_parser.cpp)

add_library(arg_parser STATIC ${SRC})