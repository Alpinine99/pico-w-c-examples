#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <pico/stdlib.h>
#include <hardware/pio.h>
#include <hardware/clocks.h>
#include "pio_counter.pio.h"


int main(void) {
  stdio_init_all();

  float div = (float) clock_get_hz(clk_sys) / 2000.f;
  uint base_pin = 16;
  uint count = 4;

  counter_init(base_pin, count, div);

  while (true);
}

