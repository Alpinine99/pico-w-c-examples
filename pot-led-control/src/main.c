#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <pico/stdlib.h>
#include <hardware/adc.h>
#include <hardware/pwm.h>


void led_dimmer(void*);

typedef struct {
  uint adc_pin;
  uint adc_channel;
  uint pwm_pin;
} param_t;

int main(void) {
  stdio_init_all();

  param_t param = {
    .adc_pin = 26,
    .adc_channel = 0,
    .pwm_pin = 17
  };

  xTaskCreate(
      led_dimmer, "led_dimmer", 256, &param, 1, NULL
  );

  vTaskStartScheduler();

  while (true);
}

void led_dimmer(void* pvParam) {
  param_t* param = (param_t*) pvParam;
  
  adc_init(); // initialize adc
  adc_gpio_init(param->adc_pin);
  adc_select_input(param->adc_channel);

  gpio_set_function(param->pwm_pin, GPIO_FUNC_PWM);
  uint slice = pwm_gpio_to_slice_num(param->pwm_pin);
  pwm_config config = pwm_get_default_config();
  pwm_config_set_clkdiv(&config, 125.f);
  pwm_config_set_wrap(&config, 999);
  pwm_init(slice, &config, true);

  uint16_t pot_read, led_write;
  while (true) {
    pot_read = adc_read();
    led_write = (1000 * pot_read) / (1 << 12);
    printf("pot_read=%zu led_write=%zu\n", pot_read, led_write);
    pwm_set_gpio_level(param->pwm_pin, led_write);
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

