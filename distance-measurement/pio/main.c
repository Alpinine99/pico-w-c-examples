#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <pico/stdlib.h>
#include <hardware/pio.h>
#include <hardware/clocks.h>
#include "distance.pio.h"


TaskHandle_t handler = NULL;

void recorder();
void irq_handler();

PIO _pio;
uint _sm;

int main(void) {
  stdio_init_all();

  xTaskCreate(
      recorder, "recorder", 256, NULL, 1, &handler
  );

  PIO pio = pio0;
  uint sm = pio_claim_unused_sm(pio, true);

  _pio = pio;
  _sm = sm;

  uint TRIG_PIN = 17;
  uint ECHO_PIN = 16;

  float div = (float) clock_get_hz(clk_sys) / 10000000.f;

  distance_init(pio, sm, TRIG_PIN, ECHO_PIN, div);

  pio_set_irq0_source_enabled(pio, pis_interrupt0, true);
  irq_set_exclusive_handler(PIO0_IRQ_0, irq_handler);
  irq_set_enabled(PIO0_IRQ_0, true);

  vTaskStartScheduler();

  while (true);
}

void recorder() {
  while (true) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    uint32_t ticks = pio_sm_get_blocking(_pio, _sm);
    float time_us = (float) ticks * .3f / 2.f;
    float dist_cm = 0.0334f * time_us;

    printf("ticks=%zu time_us=%.3f dist_cm=%.3f\n", ticks, time_us, dist_cm);
  }
}

void irq_handler() {
  if (pio_interrupt_get(_pio, 0)) {
    pio_interrupt_clear(_pio, 0);

    BaseType_t xTaskHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(handler, &xTaskHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xTaskHigherPriorityTaskWoken);
  }
}

