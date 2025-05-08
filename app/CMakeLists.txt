cmake_minimum_required(VERSION 3.11)

project(
  Tetris_cpp
  VERSION 0.2
  DESCRIPTION "Tetris in cpp"
  LANGUAGES CXX)

find_package(Curses REQUIRED)
include_directories(${CMAKE_SOURCE_DIR}/include)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(app)
add_subdirectory(lib)