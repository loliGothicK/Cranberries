cmake_minimum_required(VERSION 2.8)

project(CranberriesLibrary)

if(${CXX} MATCHES "clang")
  add_definitions(-std=c++14 -stdlib=libc++ -O3 -Wall -Wextra -pedantic)
else()
  add_definitions(-std=c++14 -O3 -Wall -Wextra -pedantic)
endif()

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

enable_testing()
