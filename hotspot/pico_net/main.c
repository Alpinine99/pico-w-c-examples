#include <FreeRTOS.h>
#include <task.h>

#include <pico/stdlib.h>
#include <pico/stdio.h>
#include <pico/cyw43_arch.h>
#include <dhcpserver.h>


void hotspot();

int main(void) {
  stdio_init_all();

  xTaskCreate(
      hotspot, "hotspot", 4096, NULL, 1, NULL
  );

  vTaskStartScheduler();

  while (true);
}

void hotspot() {
  if (cyw43_arch_init()) {
    printf("[!] arch init failed\n");
    vTaskDelete(NULL);
  }
  cyw43_arch_enable_ap_mode(SSID, PASSWD, CYW43_AUTH_WPA2_AES_PSK);

  ip4_addr_t gw, mask;
  IP4_ADDR(&gw, 192, 168, 4, 1);
  IP4_ADDR(&mask, 255, 255, 255, 248);
  dhcp_server_t dhcp_server;
  dhcp_server_init(&dhcp_server, &cyw43_state.netif[CYW43_ITF_AP], &gw, &mask); 

  while (!stdio_usb_connected());
  printf("[+] Hotspot active\n");

  while (true);
}

