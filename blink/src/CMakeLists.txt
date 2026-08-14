add_executable(${NAME}
  main.c
)

target_link_libraries(${NAME}
  pico_stdlib
  pico_cyw43_arch_none
  FreeRTOS-Kernel
  FreeRTOS-Kernel-Heap4
)

target_include_directories(${NAME}
  PRIVATE ${CMAKE_CURRENT_LIST_DIR}
)

pico_enable_stdio_usb(${NAME} 1)
pico_enable_stdio_uart(${NAME} 0)

pico_add_extra_outputs(${NAME})

