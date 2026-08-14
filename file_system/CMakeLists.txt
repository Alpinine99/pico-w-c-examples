cmake_minimum_required(VERSION 3.15)

set(PICO_BOARD pico_w)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})
set(FREERTOS_KERNEL_PATH $ENV{FREERTOS_KERNEL_PATH})

include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(pico-fs C CXX ASM)

pico_sdk_init()

include($ENV{PICO_LIB_PATH}/littlefs/CMakeLists.txt)
add_subdirectory(src)

