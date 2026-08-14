cmake_minimum_required(VERSION 3.15)

set(PICO_BOARD pico_w)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})
set(FREERTOS_KERNEL_PATH $ENV{FREERTOS_KERNEL_PATH})

include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include(${PICO_SDK_PATH}/tools/CMakeLists.txt)
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(2_pbno C CXX ASM)

pico_sdk_init()

add_subdirectory(led-control)
add_subdirectory(led-counter-with-reset)
add_subdirectory(led-counter-up-down)

