#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <pico/stdlib.h>


typedef struct {
  uint pin;
  long delay;
} param_t;

void led_task(void*);

int main(void) {
  stdio_init_all();

  param_t yellow_param = {
    .pin = 17, 
    .delay = 1000
  };

  param_t red_param = {
    .pin = 16, 
    .delay = 600
  };

  xTaskCreate(
      led_task, "yellow-led", 256, &yellow_param, 1, NULL
  );

  xTaskCreate(
      led_task, "red-led", 256, &red_param, 1, NULL
  );

  vTaskStartScheduler();

  while(true);
}

void led_task(void* pvParams) {
  param_t* param = (param_t*) pvParams;
  uint pin = param->pin;
  long delay = param->delay;

  gpio_init(pin);
  gpio_set_dir(pin, GPIO_OUT);

  while (true) {
    gpio_put(pin, 1);
    printf("[*] led [%ld] on\n", pin);
    vTaskDelay(pdMS_TO_TICKS(delay));

    gpio_put(pin, 0);
    printf("[!] led [%ld] off\n", pin);
    vTaskDelay(pdMS_TO_TICKS(delay));
  }
}
