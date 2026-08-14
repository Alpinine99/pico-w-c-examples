#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <pico/stdlib.h>
#include <hardware/pio.h>
#include <hardware/clocks.h>
#include "crude_math.pio.h"


void interaction(void*);

int main(void) {
  stdio_init_all();

  xTaskCreate(
      interaction, "interaction", 256, NULL, 1, NULL
  );

  vTaskStartScheduler();

  while (true);
}

void interaction(void* pvParam) {
  PIO pio = pio0;
  uint sm = pio_claim_unused_sm(pio, true);
  
  float div = (float) clock_get_hz(clk_sys) / 2000.f;
  uint base_pin = 16;
  uint count = 4;

  crude_math_init(pio, sm, base_pin, count, div);

  while (!stdio_usb_connected());

  while (true) {
    for (uint32_t i=0; i<10; i++) {
      pio_sm_put_blocking(pio, sm, i);
      uint32_t a = pio_sm_get_blocking(pio, sm);
      uint32_t b = pio_sm_get_blocking(pio, sm);
      uint32_t c = pio_sm_get_blocking(pio, sm);
      uint32_t d = pio_sm_get_blocking(pio, sm);
      printf("n=%zu x2=%zu x4=%zu x2+1=%zu x4+3=%zu\n", i, a, b, c, d);
      sleep_ms(1000);
    }
  }
}
