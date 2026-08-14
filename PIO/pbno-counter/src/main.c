#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <pico/stdlib.h>
#include <hardware/clocks.h>
#include "pbno_counter.pio.h"


int main(void) {
  stdio_init_all();

  float div = (float) clock_get_hz(clk_sys) / 2000.f;
  pbno_counter_init(16, 4, 14, 2, div);

  while (true);
}
