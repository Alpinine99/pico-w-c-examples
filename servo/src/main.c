#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <pico/stdlib.h>
#include <hardware/pwm.h>


void servo_sweep(void*);
void servo_write(uint, uint);

int main() {
  stdio_init_all();

  uint servo_pin = 16;
  xTaskCreate(
      servo_sweep, "servo_sweep", 256, &servo_pin, 1, NULL
  );

  vTaskStartScheduler();

  while (true);
}

void servo_sweep(void* pvParam) {
  uint servo_pin = *(uint*) pvParam;

  gpio_set_function(servo_pin, GPIO_FUNC_PWM);
  uint slice = pwm_gpio_to_slice_num(servo_pin);
  pwm_set_clkdiv(slice, 125.f); // ~ 1MHz (1 tick = 1micro second)
  pwm_set_wrap(slice, 19999);   // 1MHz / 20000 50Hz = 20ms 
  pwm_set_enabled(slice, true);

  int angle = 0;
  int dir = 1;
  while (true) {
    angle += dir;
    servo_write(servo_pin, angle);
    if (angle >= 180 || angle <= 0) dir *= -1;
    vTaskDelay(pdMS_TO_TICKS(15));
  }
}

void servo_write(uint pin, uint angle) {
  float min_pulse = 0.5f, max_pulse = 2.5f;
  if (angle > 180) angle = 180;
  uint slice = pwm_gpio_to_slice_num(pin);
  uint channel = pwm_gpio_to_channel(pin);
  float pulse_ms = min_pulse + (angle / 180.f) * (max_pulse - min_pulse);
  uint16_t pulse_us = (uint16_t) (pulse_ms * 1000.f);
  // printf("angle=%u pulse_us=%zu\n", angle, pulse_us);
  pwm_set_chan_level(slice, channel, pulse_us);
}
