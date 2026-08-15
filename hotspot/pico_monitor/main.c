#include <FreeRTOS.h>
#include <task.h>

#include <pico/stdlib.h>
#include <pico/stdio.h>
#include <pico/cyw43_arch.h>

#include <dhcpserver.h>
#include <monitor.h>


void monitor_task();

client_stats_t stats[MAX_CLIENTS];
SemaphoreHandle_t stats_mutex;

int main(void) {
  stdio_init_all();

  xTaskCreate(
      monitor_task, "monitor_task", 4096, NULL, 1, NULL
  );

  vTaskStartScheduler();

  while (true);
}

void monitor_task() {
  while (!stdio_usb_connected());

  if (cyw43_arch_init()) {
    printf("[!] arch init failed\n");
    vTaskDelete(NULL);
  }
  cyw43_arch_enable_ap_mode(SSID, PASSWD, CYW43_AUTH_WPA2_AES_PSK);

  ip4_addr_t gw, mask;
  IP4_ADDR(&gw, 192, 168, 4, 1);
  IP4_ADDR(&mask, 255, 255, 255, 248);

  dhcp_server_t dhcps;
  dhcp_server_init(&dhcps, &cyw43_state.netif[CYW43_ITF_AP], &gw, &mask);

  stats_mutex = xSemaphoreCreateMutex();
  install_traffic_hooks();

  printf("[+] Hotspot active !\n");
  
  while (true) {
    vTaskDelay(pdMS_TO_TICKS(5000));

    client_stats_t snapshot[MAX_CLIENTS];
    xSemaphoreTake(stats_mutex, portMAX_DELAY);
    memcpy(snapshot, stats, sizeof(snapshot));
    xSemaphoreGive(stats_mutex);

    printf("\033[H\033[J");
    printf(
        "+---------------------+---------+---------+\n"
        "|   MAC               |RX       |TX       |\n"
        "+---------------------+---------+---------+\n"
        );
    char mac[19];
    for (int i = 0; i < MAX_CLIENTS; i++) {
      if (snapshot[i].used) {
        sprintf(mac, "%02x:%02x:%02x:%02x:%02x:%02x",
            snapshot[i].mac[0], snapshot[i].mac[1], snapshot[i].mac[2],
            snapshot[i].mac[3], snapshot[i].mac[4], snapshot[i].mac[5]);
        printf("|%-21s|%-9lu|%-9lu|\n", mac,
            snapshot[i].bytes_rx, snapshot[i].bytes_tx);
        printf("+---------------------+---------+---------+\n");
      }
    }
  }
}

