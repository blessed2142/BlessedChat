cmake_minimum_required(VERSION 3.1)

project(BlessedChatServer VERSION 1.0 LANGUAGES CXX)

set(TARGET_OS "Linux" CACHE STRING "Target Operating System")

set(RESULT_DIR ${CMAKE_SOURCE_DIR}/_result)
set(BUILD_DIR ${CMAKE_BINARY_DIR})
set(APP_DIR ${CMAKE_SOURCE_DIR}/BlessedChat)
set(COMPONENTS_DIR ${CMAKE_SOURCE_DIR}/components)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
     add_compile_definitions(_DEBUG)
     set(CONFIG Debug )
else()
     set(CONFIG Release )
endif()

#set(CMAKE_VERBOSE_MAKEFILE ON)

if(MSVC)
    add_compile_options(/W4)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        message( "Add DEBUG ${PROJECT_NAME} ${CMAKE_SYSTEM_NAME}")
    else()
        message( "Add RELEASE ${PROJECT_NAME} ${CMAKE_SYSTEM_NAME}")
    endif()
else()
    add_compile_options(-Wall -Wpedantic -Wextra)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        message( "Add DEBUG ${PROJECT_NAME} ${CMAKE_SYSTEM_NAME}")
        add_compile_options(-g -O0)
    else()
        message( "Add RELEASE ${PROJECT_NAME} ${CMAKE_SYSTEM_NAME}")
        add_compile_options(-Ofast)
    endif()
endif()
if (EXT_DBG_LOG STREQUAL "ON")
    add_compile_definitions(EXT_DBG_LOG)
endif()


# add_subdirectory(cmake/cmn cmn)
# 
# add_subdirectory(BlessedChat BlessedChatServer)
add_subdirectory(${COMPONENTS_DIR}/arg_parser)
add_subdirectory(BlessedChat)

add_subdirectory(test)

#developer test projects
if(DEFINED BUILD_DEVOLOPERTOOLS)
    # add_subdirectory(cmake/sies_sm_ws_test sies_sm_ws_test)
endif()

# include(cmake/Packing.cmake)
