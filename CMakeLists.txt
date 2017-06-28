cmake_minimum_required(VERSION 3.0)
project(OpenRM)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(./lib)

aux_source_directory(. SOURCE_FILES)
add_executable(orm ${SOURCE_FILES})

target_include_directories(orm PUBLIC ./lib)

target_link_libraries(orm OpenRM)