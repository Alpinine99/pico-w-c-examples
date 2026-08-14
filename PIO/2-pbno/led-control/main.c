#include <stdio.h>
#include <FreeRTOS.h>
#include <pico/stdlib.h>
#include <hardware/pio.h>
#include <hardware/clocks.h>
#include "led_control.pio.h"

int main(void) {
  stdio_init_all();

  float div = (float) clock_get_hz(clk_sys) / 2000.f;

  led_control_init(16, 4, 14, 2, div);

  while (true);
}

