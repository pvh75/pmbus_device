cmake_minimum_required(VERSION 3.25)

project(pmbus_smbus_stack VERSION 1.0.0.0)

set(TARGET pmbus)

add_library(${TARGET} OBJECT)

target_sources(${TARGET} PRIVATE
    ./src/smbus.c
    ./src/pmbus.c
)

target_include_directories(${TARGET} PUBLIC
    ./include
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/src/version.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/version.h")
