cmake_minimum_required(VERSION 3.16)

project(podofo.js)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include(Misc)
include(npm)

option(ENABLE_TESTS "Enable tests" ON)

set(ALL_SOURCES
    src/bind.cpp
)

set(PODOFO_BUILD_LIB_ONLY ON)
set(PODOFO_BUILD_STATIC ON)
add_subdirectory(podofo)

add_subdirectory(src)
add_subdirectory(npm)

if (ENABLE_TESTS)
    add_subdirectory(tests)
endif()

add_clang_format_target()
