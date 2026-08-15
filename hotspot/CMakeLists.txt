cmake_minimum_required(VERSION 3.15)

set(PICO_BOARD pico_w)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})
set(FREERTOS_KERNEL_PATH $ENV{FREERTOS_KERNEL_PATH})

include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include(${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/FreeRTOS_Kernel_import.cmake)

project(hotspot C CXX ASM)

pico_sdk_init()

foreach(name IN ITEMS pico_net pico_monitor pico_sniffer)
  set(SOURCES
    ${name}/main.c
    dnsserver/dnsserver.c
    dhcpserver/dhcpserver.c
  )
  
  if(${name} STREQUAL "pico_monitor")
    list(APPEND SOURCES ${name}/monitor/monitor.c)
  elseif(${name} STREQUAL "pico_sniffer")
    list(APPEND SOURCES ${name}/sniffer/sniffer.c)
  endif()

  add_executable(${name}
    ${SOURCES}
  )

  target_include_directories(${name}
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/dnsserver
    PRIVATE ${CMAKE_CURRENT_LIST_DIR}/dhcpserver
  )
  
  if (${name} STREQUAL "pico_monitor")
    target_include_directories(${name}
      PRIVATE ${name}/monitor
    )
  elseif (${name} STREQUAL "pico_sniffer")
    target_include_directories(${name}
      PRIVATE ${name}/sniffer
    )
  endif()

  target_link_libraries(${name}
    pico_stdlib
    pico_cyw43_arch_lwip_sys_freertos
    FreeRTOS-Kernel
    FreeRTOS-Kernel-Heap4
  )

  target_compile_definitions(${name} PRIVATE
    SSID="pico net"
    PASSWD="password123"
  )

  pico_enable_stdio_usb(${name} 1)
  pico_enable_stdio_uart(${name} 0)

  pico_add_extra_outputs(${name})

endforeach()

