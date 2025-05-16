
cmake_minimum_required(VERSION 3.25)

project(test_project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

find_package(GTest REQUIRED)

enable_testing()

include_directories(
  ../include
  ../src
  ../../../src
)

add_executable(test_pmbus)

target_sources(test_pmbus PRIVATE
  test_pmbus.cpp
  ../src/pmbus.c
)

target_link_libraries(test_pmbus GTest::gtest_main)

add_test(NAME test_pmbus COMMAND test_pmbus)
