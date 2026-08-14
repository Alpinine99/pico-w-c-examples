#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <pico/stdlib.h>
#include <pico/time.h>


void recorder();

int main(void) {
  stdio_init_all();

  xTaskCreate(
      recorder, "recorder", 256, NULL, 1, NULL
  );

  vTaskStartScheduler();

  while (true);
}

void recorder() {
  uint TRIG_PIN = 17;
  uint ECHO_PIN = 16;

  gpio_init(TRIG_PIN);
  gpio_init(ECHO_PIN);

  gpio_set_dir(TRIG_PIN, GPIO_OUT);
  gpio_set_dir(ECHO_PIN, GPIO_IN);

  while (true) {
    gpio_put(TRIG_PIN, 0);
    sleep_us(2);
    gpio_put(TRIG_PIN, 1);
    sleep_us(10);
    gpio_put(TRIG_PIN, 0);

    while (gpio_get(ECHO_PIN) == 0);
    absolute_time_t startT = get_absolute_time();

    while (gpio_get(ECHO_PIN) == 1);
    absolute_time_t stopT = get_absolute_time();

    int64_t diff_us = absolute_time_diff_us(startT, stopT);
    float dist_cm = (0.0334f * diff_us) / 2.f;

    printf("distance=%.3f us=%lld\n", dist_cm, diff_us);
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

