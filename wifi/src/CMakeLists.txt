add_executable(${PROJECT_NAME}
  main.c
)

target_include_directories(${PROJECT_NAME}
  PRIVATE ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(${PROJECT_NAME}
  pico_stdlib
  pico_cyw43_arch_lwip_sys_freertos
  pico_lwip_freertos
  FreeRTOS-Kernel
  FreeRTOS-Kernel-Heap4
)

target_compile_definitions(${PROJECT_NAME}
  PRIVATE WIFI_SSID=\"$ENV{WIFI_SSID}\"
  WIFI_PASSWD=\"$ENV{WIFI_PASSWD}\"
)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

pico_add_extra_outputs(${PROJECT_NAME})

