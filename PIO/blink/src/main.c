#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <pico/stdlib.h>
#include <hardware/pio.h>
#include <hardware/clocks.h>
#include "pio_blink.pio.h"


int main(void) {
  stdio_init_all();

  uint base_pin = 16;
  float pio_freq = 2000.f;

  // choose pio block (0 / 1)
  PIO pio = pio0;

  // get the next free state machine
  uint sm = pio_claim_unused_sm(pio, true);

  // add pio program to pio instruction memory
  uint offset = pio_add_program(pio, &pio_blink_program);

  // calculate pio clock divider
  float div = (float) clock_get_hz(clk_sys) / pio_freq;

  // call helper in pio_blink.pio to finish setup
  blink_init(pio, sm, offset, base_pin, div);

  // start running the PIO program
  pio_sm_set_enabled(pio, sm, true);

  while (true);
}
