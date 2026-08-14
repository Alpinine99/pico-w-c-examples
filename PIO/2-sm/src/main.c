#include <stdio.h>
#include <FreeRTOS.h>
#include <pico/stdlib.h>
#include <hardware/pio.h>
#include <hardware/clocks.h>
#include "2_servo.pio.h"


int main(void) {
  stdio_init_all();

  float div = (float) clock_get_hz(clk_sys) / 2000000.f; // 2 000 000 hz

  PIO pio = pio0;
  uint sm1 = pio_claim_unused_sm(pio, true);
  uint sm2 = pio_claim_unused_sm(pio, true);

  servo_init(pio, sm1, 26, div);
  servo_init(pio, sm2, 27, div);

  uint angle = 0;
  int steps = 1;
  while (true) {
    uint32_t pw1 = (uint32_t) (2000.f * angle / 180) + 500;
    uint32_t pw2 = (uint32_t) (2000.f * (180 - angle) / 180) + 500;

    if (pio_sm_get_tx_fifo_level(pio, sm1) == 0) {
      pio_sm_put_blocking(pio, sm1, pw1);
      printf("pw1=%zu", pw1);

      angle += steps;
      if (angle <= 0 || angle >= 180)
        steps *= -1;
    }

    if (pio_sm_get_tx_fifo_level(pio, sm2) == 0) {
      pio_sm_put_blocking(pio, sm2, pw2);
      printf(", pw2=%zu", pw2);
    }

    printf(", angle=%d\n", angle);
    sleep_us(50);
  }

}

