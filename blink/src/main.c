#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <pico/stdlib.h>
#include <pico/cyw43_arch.h>


void led_task(void*);

int main() {
  stdio_init_all();

  xTaskCreate(
      led_task, "LED_TASK", 256, NULL, 1, NULL
  );

  vTaskStartScheduler();

  while (true);
}

void led_task(void* pvParams){
  cyw43_arch_init();
  
  while (true) {
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    printf("[*] led on\n");
    vTaskDelay(pdMS_TO_TICKS(500));

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    printf("[!] led off\n");
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}
