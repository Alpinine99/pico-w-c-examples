add_executable(${PROJECT_NAME}
  main.c
)

target_include_directories(${PROJECT_NAME}
  PRIVATE ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(${PROJECT_NAME}
  ssd1306
  pico_stdlib
  FreeRTOS-Kernel
  FreeRTOS-Kernel-Heap4
)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

pico_add_extra_outputs(${PROJECT_NAME})

