cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_PROJECT_NAME pmbus_device_h7)

include("cmake/gcc-arm-none-eabi.cmake")

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

enable_language(C ASM)

project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

add_executable(${CMAKE_PROJECT_NAME})

add_subdirectory(./cmake/stm32cubemx)
add_subdirectory(./lib/freertos-addons)
add_subdirectory(./lib/pmbus_smbus_stack)

target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    ./src/system.cpp
    ./src/logger.cpp
    ./src/uart_stream_out.cpp
    ./src/hardware.cpp
    ./src/application.cpp
    ./src/pmbus_logger.cpp
    ./src/pmbus_cmd.c
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ./src
)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
)

target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
    freertos_addons
    pmbus
)
