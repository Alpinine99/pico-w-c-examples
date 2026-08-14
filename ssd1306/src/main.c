#include <stdio.h>
#include <FreeRTOS.h>
#include <task.h>
#include <pico/stdlib.h>
#include <ssd1306.h>
#include <example/BMSPA_font.h>
#include <example/acme_5_outlines_font.h>
#include <example/crackers_font.h>
#include <example/bubblesstandard_font.h>


void display();

int main(void) {
  stdio_init_all();

  xTaskCreate(
      display, "display", 256, NULL, 1, NULL
  );

  vTaskStartScheduler();

  while (true);

}

void display() {
  uint SDA = 18, SCL = 19;
  i2c_inst_t* i2c = i2c1;
  i2c_init(i2c, 400 * 1000);
  gpio_set_function(SDA, GPIO_FUNC_I2C);
  gpio_set_function(SCL, GPIO_FUNC_I2C);
  gpio_pull_up(SDA);
  gpio_pull_up(SCL);

  ssd1306_t disp;
  disp.external_vcc = false;
  ssd1306_init(&disp, 128, 64, 0x3C, i2c);

  // ssd1306_contrast(&disp, 50);
  ssd1306_clear(&disp);
  ssd1306_draw_string(&disp, 0, 0, 1, "ssd1306 oled test");
  ssd1306_draw_line(&disp, 0, 8, 100, 8);

  char *points[4] = {
    "What's your name?",
    "What about age?",
    "gender too!",
    "finally height"
  };

  uint s = 10;
  char num[2];
  for (int i=0; i<4; i++) {
    char* point = points[i];
    uint y = s * (i+1) + (2*i);
    ssd1306_draw_empty_square(&disp, 0, y, s, s);
    sprintf(num, "%d", i+1);
    ssd1306_draw_string(&disp, 3, y+2, 1, num);
    ssd1306_draw_string(&disp, 3 + s + 2, y+2, 1, point);
  }
  // ssd1306_draw_square(&disp, 2, 10, 124, 52);
  // ssd1306_invert(&disp, 1);
  ssd1306_show(&disp);

  sleep_ms(3000);
  ssd1306_poweroff(&disp);
  
  sleep_ms(1000);
  ssd1306_poweron(&disp);
  ssd1306_clear(&disp);

  ssd1306_draw_string_with_font(&disp, 2, 1, 2, BMSPA_font, "Good bye");
  ssd1306_draw_string_with_font(&disp, 2, 18, 2, acme_font, "Good bye");
  ssd1306_draw_string_with_font(&disp, 2, 30, 2, crackers_font, "later");
  ssd1306_draw_string_with_font(&disp, 2, 47, 2, bubblesstandard_font, "homieh");
  ssd1306_show(&disp);

  while (true);
}

