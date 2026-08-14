#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <pico/stdlib.h>
#include <hardware/clocks.h>
#include <hardware/pio.h>
#include "pio_counter.pio.h"


int main(void) {
  stdio_init_all();

  float div = (float) clock_get_hz(clk_sys) / 2000.f;
  uint base_pin = 16;
  uint count = 4;

  PIO pio = pio0;
  uint sm = pio_claim_unused_sm(pio, true);

  counter_init(pio, sm, base_pin, count, div);

  uint32_t i = 0;
  uint32_t d_out;
  while (true) {
    printf("d_in=%zu ", i);
    // write data to pio
    pio_sm_put_blocking(pio, sm, i);

    // read data from pio
    d_out = pio_sm_get_blocking(pio, sm);
    printf("d_out=%zu\n", d_out);
    i++;
    if (i > 15) i = 0;
    sleep_ms(250);
  }
}

