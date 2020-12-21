cmake_minimum_required(VERSION 3.17)
project(labTrit)

set(CMAKE_CXX_STANDARD 14)

add_executable(labTrit main.cpp tritset.cpp tritset.h test.cpp)

add_subdirectory(googletest-master)
#add_subdirectory(googletest-master)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
target_link_libraries(labTrit gtest gtest_main)