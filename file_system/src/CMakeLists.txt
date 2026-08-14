add_executable(${PROJECT_NAME}
  main.c
)

target_link_libraries(${PROJECT_NAME}
  pico_stdlib
  littlefs-lib
  stdinit-lib
  FreeRTOS-Kernel
  FreeRTOS-Kernel-Heap4
)

target_include_directories(${PROJECT_NAME}
  PRIVATE ${CMAKE_CURRENT_LIST_DIR}
)

pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

pico_add_extra_outputs(${PROJECT_NAME})

