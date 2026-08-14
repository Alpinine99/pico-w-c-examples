#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <pico/stdlib.h>
#include <hardware/clocks.h>
#include "toggle.pio.h"


int main(void) {
  stdio_init_all();

  float div = (float) clock_get_hz(clk_sys) / 2000.f;
  toggle_init(16, 4, 14, 2, div);

  while (true);
}
