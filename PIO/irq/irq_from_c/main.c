#include <stdio.h>
#include <FreeRTOS.h>
#include <pico/stdlib.h>
#include <hardware/pio.h>
#include <hardware/clocks.h>
#include "irq.pio.h"

int main(void) {
  stdio_init_all();

  PIO pio = pio0;
  uint sm = pio_claim_unused_sm(pio, true);

  float div = (float) clock_get_hz(clk_sys) / 2000.f;
  uint but_pin = 14;
  uint led_pin = 16;

  irq_init(pio, sm, led_pin, but_pin, div);

  while (true) {
    printf("waiting for interrupt\n");
    sleep_ms(1000);
  }
}

