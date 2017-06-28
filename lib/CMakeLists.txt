cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_STANDARD 11)

aux_source_directory(. SOURCE_FILES)
add_library(OpenRM SHARED ${SOURCE_FILES})

find_package(ruby 2.4.0 REQUIRED)

target_include_directories(OpenRM PUBLIC ${RUBY_INCLUDE_DIRS})

target_link_libraries(OpenRM ${RUBY_LIBRARY})
