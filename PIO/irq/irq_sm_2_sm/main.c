#include <stdio.h>
#include <FreeRTOS.h>
#include <pico/stdlib.h>
#include <hardware/pio.h>
#include <hardware/clocks.h>
#include "irq.pio.h"


int main(void) {
  stdio_init_all();

  float div = (float) clock_get_hz(clk_sys) / 2000.f;

  irq_init(13, 16, div);

  while (true);
}

