set(SUB_NAME counter)

add_executable(${SUB_NAME}
  main.c
)

pico_generate_pio_header(${SUB_NAME}
  ${CMAKE_CURRENT_LIST_DIR}/pio_counter.pio
)

target_link_libraries(${SUB_NAME}
  pico_stdlib
  hardware_pio
  FreeRTOS-Kernel
  FreeRTOS-Kernel-Heap4
)

target_include_directories(${SUB_NAME}
  PRIVATE ${CMAKE_CURRENT_LIST_DIR}
  PRIVATE ${CMAKE_SOURCE_DIR}
)

pico_enable_stdio_usb(${SUB_NAME} 1)
pico_enable_stdio_uart(${SUB_NAME} 0)

pico_add_extra_outputs(${SUB_NAME})

