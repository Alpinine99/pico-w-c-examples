#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <pico/stdlib.h>
#include <hardware/pio.h>
#include <hardware/clocks.h>
#include "irq.pio.h"


void irq_handler();
void printT();
void interruptT();

PIO* _pio = NULL;
TaskHandle_t interruptT_h = NULL;

int main(void) {
  stdio_init_all();

  float div = (float) clock_get_hz(clk_sys) / 2000.f;
  PIO pio = pio0;
  _pio = &pio;
  uint sm = pio_claim_unused_sm(pio, true);

  uint button_pin = 15;
  pio_gpio_init(pio, button_pin);
  gpio_pull_down(button_pin);
  pio_sm_set_consecutive_pindirs(pio, sm, button_pin, 1, false);

  uint offset = pio_add_program(pio, &i_rq_program);
  pio_sm_config config = i_rq_program_get_default_config(offset);
  sm_config_set_clkdiv(&config, div);
  sm_config_set_in_pin_base(&config, button_pin);
  sm_config_set_in_pin_count(&config, 1);

  pio_sm_init(pio, sm, offset, &config);
  pio_sm_set_enabled(pio, sm, true);

  xTaskCreate(
      printT, "printT", 256, NULL, 2, NULL
  );

  xTaskCreate(
      interruptT, "interruptT", 256, NULL, 1, &interruptT_h
  );

  pio_set_irq0_source_enabled(pio, pis_interrupt0, true);
  irq_set_exclusive_handler(PIO0_IRQ_0, irq_handler);
  irq_set_enabled(PIO0_IRQ_0, true);

  vTaskStartScheduler();

  while (true);
}

void irq_handler() {
  if (pio_interrupt_get(*_pio, 0)) {
    printf("\033[31;1m [!] irq fired \033[0m\n");
    pio_interrupt_clear(*_pio, 0);

    if (interruptT_h == NULL)
      printf("interrupt is null\n");

    BaseType_t xTaskHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(interruptT_h, &xTaskHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xTaskHigherPriorityTaskWoken);
  }
}

void printT() {
  while (true) {
    printf("printT task is running ... \n");
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void interruptT() {
  while (true) {
    // Block here until the ISR notifies us
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    printf("\033[32;1m [*] irq handled \033[0m\n");
  }
}

