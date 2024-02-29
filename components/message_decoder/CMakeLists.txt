cmake_minimum_required(VERSION 3.1)

project(message_decoder VERSION 1.0 LANGUAGES CXX)

set(SRC message_decoder.cpp)

add_library(message_decoder STATIC ${SRC})