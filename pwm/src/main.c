#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <pico/stdlib.h>
#include <hardware/pwm.h>


void write_pwm(void*);

int main(void) {
  stdio_init_all();

  uint pin = 17;
  xTaskCreate(
      write_pwm, "pwm", 256, &pin, 1, NULL
  );

  vTaskStartScheduler();

  while (true);
}

void write_pwm(void* pvParam) {
  uint pin = *(uint*) pvParam;

  gpio_set_function(pin, GPIO_FUNC_PWM);
  uint slice = pwm_gpio_to_slice_num(pin);

  // config
  pwm_config config = pwm_get_default_config();
  // 125MHz / 125.0 =~ 1MHz
  pwm_config_set_clkdiv(&config, 125.f);
  pwm_config_set_wrap(&config, 999);
  pwm_init(slice, &config, true);

  uint16_t level = 0;
  int dir = 1;
  while (true) {
    pwm_set_gpio_level(pin, level);
    vTaskDelay(pdMS_TO_TICKS(100));
    level += dir * 100;
    if (level >= 1000 || level <= 0) dir *= -1;
  }
}
