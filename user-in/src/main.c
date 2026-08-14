#include <stdio.h>
#include <stdlib.h>
#include <FreeRTOS.h>
#include <task.h>
#include <pico/stdlib.h>


void runner(void*);
void serial_read(char*, uint);

int main(void) {
  stdio_init_all();

  xTaskCreate(
      runner, "runner", 256, NULL, 1, NULL
  );

  vTaskStartScheduler();

  while(true);
}

void serial_read(char* result, uint max) {
  char c;
  int i = 0;
  while (i < max -1 && !((c = getchar()) == '\r' || c == '\n')) {
    if (c == '\b' || c == 127) {
      if (i > 0) {
        i--;
        result[i] = '\0';
        putchar('\b');
        putchar(' ');
        putchar('\b');
      }
    } else {
      // printf("key=%d\n", c);
      putchar(c);
      result[i] = c;
      i++;
    }
  }
  result[i] = '\0';
  putchar('\n');
}

void runner(void* pvParam) {
  while (!stdio_usb_connected())
    vTaskDelay(pdMS_TO_TICKS(100));

  char name[10];
  int age;
  while(true) {
    printf("Whats your name? ");
    serial_read(name, sizeof(name));

    printf("Hello %s, how old are you? ", name);
    char age_s[4];
READ_AGE:
    serial_read(age_s, sizeof(age_s));
    if ((age = atoi(age_s)) <= 0) {
      printf("Invalid input, Enter age: ");
      goto READ_AGE;
    }

    printf("======================\nNAME: %s\nAGE: %d\nADULT: ", name, age);
    if (age >= 18)
      printf("YES\n");
    else
      printf("NO\n");
    printf("======================\n\n");
  }
}

