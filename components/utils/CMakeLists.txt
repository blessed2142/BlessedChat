cmake_minimum_required(VERSION 3.1)

project(utils VERSION 1.0 LANGUAGES CXX)

set(SRC base64.cpp utils.cpp)

add_library(utils STATIC ${SRC})