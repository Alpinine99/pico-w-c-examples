#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <pico/stdlib.h>
#include <hardware/pwm.h>
#include <hardware/adc.h>


typedef struct {
  uint pwm;
  uint adc;
} param_t;

void pot_servo_control(void*);

int main(void) {
  stdio_init_all();

  param_t param = {
    .pwm = 16,
    .adc = 26
  };

  xTaskCreate(
      pot_servo_control, "pot-control", 256, &param, 1, NULL
  );

  vTaskStartScheduler();

  while (true);
}

void pot_servo_control(void* pvParam) {
  param_t* param = (param_t*) pvParam;

  // servo pin
  gpio_set_function(param->pwm, GPIO_FUNC_PWM);
  uint pwm_slice = pwm_gpio_to_slice_num(param->pwm);
  uint channel = pwm_gpio_to_channel(param->pwm);
  pwm_set_clkdiv(pwm_slice, 125.f);
  pwm_set_wrap(pwm_slice, 19999);
  pwm_set_enabled(pwm_slice, true);

  // potentiometer pin
  adc_init();
  adc_gpio_init(param->adc);
  adc_select_input(0);

  uint16_t pot_read, servo_write;
  while (true) {
    pot_read = adc_read();
    servo_write = (uint16_t) (500.f + (pot_read / 4096.f) * 2000.f);
    pwm_set_chan_level(pwm_slice, channel, servo_write);
    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

