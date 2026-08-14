#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
#include <pico/stdlib.h>


void read(void*);
void led(void*);

typedef struct {
  uint red_pin;
  uint yellow_pin;
} param_t;

SemaphoreHandle_t xSemphr;

int main(void) {
  stdio_init_all();

  xSemphr = xSemaphoreCreateCounting(1, 1);

  uint button_pin = 18;
  param_t param = {
    .red_pin = 16,
    .yellow_pin = 17,
  };

  xTaskCreate(
      read, "read-pin", 256, &button_pin, 2, NULL
  );

  xTaskCreate(
      led, "led", 256, &param, 1, NULL
  );

  vTaskStartScheduler();

  while(1);
}

void read(void* pvParam) {
  uint pin = *(uint*) pvParam;

  gpio_init(pin);
  gpio_set_dir(pin, GPIO_IN);

  int old_state = 0;
  while (true) {
    int state = gpio_get(pin);
    if (old_state == 0 && state == 1) {
      printf("\033[34;1m[button] pressed\033[0m\n");
    } else if (old_state == 1 && state == 0) {
      printf("\033[34;1m[button] released\033[0m\n");
      xSemaphoreGive(xSemphr);
    }
    old_state = state;
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void led(void* pvParam) {
  param_t* param = (param_t*) pvParam;

  uint red_pin = param->red_pin;
  uint yellow_pin = param->yellow_pin;

  gpio_init(red_pin);
  gpio_init(yellow_pin);

  gpio_set_dir(red_pin, GPIO_OUT);
  gpio_set_dir(yellow_pin, GPIO_OUT);

  while (true) {
    if (xSemaphoreTake(xSemphr, (TickType_t) 100) == pdTRUE) {
      if (gpio_get(red_pin) == 0) {
        printf("[led] \033[31;1mred\033[0m on\n");
        gpio_put(red_pin, 1);
        gpio_put(yellow_pin, 0);
      } else {
        printf("[led] \033[33;1myellow\033[0m on\n");
        gpio_put(red_pin, 0);
        gpio_put(yellow_pin, 1);
      }
    }
    vTaskDelay(100);
  }
}

