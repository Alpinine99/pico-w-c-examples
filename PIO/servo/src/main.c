#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <pico/stdlib.h>
#include <hardware/pio.h>
#include <hardware/clocks.h>
#include "pio_servo.pio.h"


int main(void) {
  stdio_init_all();

  uint servo_pin = 26;
  uint pio_freq = 2000000.f;

  PIO pio = pio0;
  
  uint sm = pio_claim_unused_sm(pio, true);
  uint offset = pio_add_program(pio, &pio_servo_program);
  float div = (float) clock_get_hz(clk_sys) / pio_freq;

  pio_gpio_init(pio, servo_pin);
  pio_sm_set_consecutive_pindirs(pio, sm, servo_pin, 1, true);
  
  pio_sm_config config = pio_servo_program_get_default_config(offset);
  sm_config_set_set_pins(&config, servo_pin, 1);
  sm_config_set_clkdiv(&config, div);
  sm_config_set_in_shift(&config, false, false, 1);
  
  pio_sm_init(pio, sm, offset, &config);
  pio_sm_set_enabled(pio, sm, true);

  int steps = -1;
  int angle = 0;
  while (true) {
    if (pio_sm_get_tx_fifo_level(pio, sm) == 0) {
      if (angle <= 0 || angle >= 180) {
        steps *= -1;
        sleep_ms(500);
      }
      angle += steps;
      uint32_t pw = (uint32_t) ((2000.f*angle)/180.f + 500);
      pio_sm_put_blocking(pio, sm, pw);
    }
    sleep_us(10);
  }
}

