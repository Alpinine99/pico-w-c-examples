#include <FreeRTOS.h>
#include <task.h>

#include <pico/stdio.h>
#include <pico/stdlib.h>
#include <pico/cyw43_arch.h>
#include <lwip/sockets.h>


void wifi_con();

int main(void) {
  stdio_init_all();

  xTaskCreate(
      wifi_con, "wifi_con", 1024 * 10, NULL, 1, NULL
  );

  vTaskStartScheduler();

  while (true);
}

void wifi_con() {
  while (!stdio_usb_connected());

  if (cyw43_arch_init()) {
    printf("[!] Wifi init failed!\n");
    vTaskDelete(NULL);
  }
  cyw43_arch_enable_sta_mode();

  printf("[+] Connecting to wifi network: %s ... \n", WIFI_SSID);
  if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASSWD, CYW43_AUTH_WPA2_AES_PSK, 30000)) {
    printf("[!] Wifi connection failed!\n");
    vTaskDelete(NULL);
  }

  printf("[+] Connected. IP: [%s]\n", ip4addr_ntoa(netif_ip4_addr(netif_list)));

  int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (fd < 0) {
    printf("[!] Socket connection failed\n");
    vTaskDelete(NULL);
  }

  struct sockaddr_in server = {0};
  server.sin_family = AF_INET;
  server.sin_port = htons(8080);
  server.sin_addr.s_addr = inet_addr("192.168.41.6");
  printf("[+] connecting to server ... \n");
  if (connect(fd, (struct sockaddr*) &server, sizeof(server)) < 0) {
    close(fd);
    printf("[!] Connect failed\n");
    vTaskDelete(NULL);
  }

  printf("[+] connected to server\n");
  char msg[] = "Hello mate from pico w side!";
  send(fd, msg, strlen(msg), 0);
  printf("[+] request sent to server\n");

  char buf[64];
  int n = recv(fd, buf, sizeof(buf)-1, 0);
  if (n > 0) {
    buf[n] = '\0';
    printf("[*] data: %s\n", buf);
  } else {
    printf("[!] recv failed\n");
  }

  close(fd);
  printf("[+] session ended\n");

  cyw43_arch_disable_sta_mode();
  cyw43_arch_deinit();

  vTaskDelete(NULL);
}

