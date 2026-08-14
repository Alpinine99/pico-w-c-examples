set(NAME pio-ssd1306)

include($ENV{PICO_LIB_PATH}/ssd1306/CMakeLists.txt)

add_executable(${NAME}
  main.c
)

pico_generate_pio_header(${NAME}
  ${CMAKE_CURRENT_LIST_DIR}/distance.pio
)

target_link_libraries(${NAME}
  ssd1306
  pico_stdlib
  hardware_pio
  FreeRTOS-Kernel
  FreeRTOS-Kernel-Heap4
)

target_include_directories(${NAME}
  PRIVATE ${CMAKE_CURRENT_LIST_DIR}
  PRIVATE ${CMAKE_SOURCE_DIR}
)

pico_enable_stdio_usb(${NAME} 1)
pico_enable_stdio_uart(${NAME} 0)

pico_add_extra_outputs(${NAME})

