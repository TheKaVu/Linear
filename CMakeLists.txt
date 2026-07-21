cmake_minimum_required(VERSION 4.2)
set(CMAKE_CXX_STANDARD 20)

project(Linear)

add_library(Linear SHARED
        src/library.cpp
        src/vec.h
        src/mat.h
        src/rational.h
)

target_include_directories(Linear PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(tests)

set_target_properties(Linear PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)