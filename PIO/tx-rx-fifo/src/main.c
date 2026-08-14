#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <pico/stdlib.h>
#include <hardware/clocks.h>
#include <hardware/pio.h>
#include "tx_rx.pio.h"


int main(void) {
  stdio_init_all();

  PIO pio = pio0;
  uint sm = pio_claim_unused_sm(pio, true);

  float div = (float) clock_get_hz(clk_sys) / 2000.f;
  uint base_pin = 16;
  uint count = 4;

  tx_rx_init(pio, sm, base_pin, count, div);

  while (!stdio_usb_connected());

  while (true) {
    printf("\033[33;1m[*] writing data ... \033[0m\n");
    for (uint32_t i=0; i<8; i++) {
      if (pio_sm_is_tx_fifo_full(pio, sm)) {
        printf("\033[31;1m[!] TX fifo full\033[0m\n");
        while (pio_sm_is_tx_fifo_full(pio, sm))
          sleep_ms(100);
        printf("\033[32;1m[!] TX fifo not full\033[0m\n");
      }
      uint32_t value = i + 7;
      pio_sm_put(pio, sm, value);
      // pio_sm_put_blocking(pio, sm, i); // used to wait for fifo to be free
      printf("[index=%d value=%zu] TX=%d RX=%d\n", i, value, pio_sm_get_tx_fifo_level(pio, sm), pio_sm_get_rx_fifo_level(pio, sm));
    }
   
    printf("\033[33;1m[*] reading data ... \033[0m\n");
    for (int i=0; i<8; i++) {
      if (pio_sm_is_rx_fifo_empty(pio, sm)) {
        printf("\033[31;1m[!] RX fifo empty\033[0m\n");
        while (pio_sm_is_rx_fifo_empty(pio, sm))
          sleep_ms(100);
        printf("\033[32;1m[!] RX fifo not empty\033[0m\n");
      }
      uint32_t value = pio_sm_get(pio, sm);
      printf("[index=%d value=%zu] TX=%d RX=%d\n", i, value, pio_sm_get_tx_fifo_level(pio, sm), pio_sm_get_rx_fifo_level(pio, sm));
    }
  }
}

