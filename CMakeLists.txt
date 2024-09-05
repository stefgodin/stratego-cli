cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(stratego C)

link_directories(${CMAKE_SOURCE_DIR}/lib)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/src/*.h)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

target_link_libraries(stratego ${CURSES_LIBRARIES})

