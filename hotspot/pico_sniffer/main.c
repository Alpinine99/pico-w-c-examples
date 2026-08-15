#include <FreeRTOS.h>
#include <task.h>

#include <pico/stdio.h>
#include <pico/stdlib.h>
#include <pico/cyw43_arch.h>
#include <lwip/sockets.h>

#include <dhcpserver.h>
#include <sniffer.h>


#define PC_IP   "192.168.4.17"
#define PC_PORT 8080

void sniffer_task();
void mirror_task();

QueueHandle_t sniffer_queue;
QueueHandle_t mirror_queue;


int main(void) {
  stdio_init_all();

  xTaskCreate(
      sniffer_task, "sniffer_task", 1024 * 8, NULL, 2, NULL
  );

  xTaskCreate(
      mirror_task, "mirror_task", 1024, NULL, 1, NULL
  );

  vTaskStartScheduler();

  while (true);
}

void mirror_task() {
  int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  struct sockaddr_in dest = {0};
  dest.sin_family = AF_INET;
  dest.sin_port = htons(PC_PORT);
  dest.sin_addr.s_addr = inet_addr(PC_IP);

  mirror_queue = xQueueCreate(32, sizeof(mirror_frame_t));
  mirror_endpoint_init(PC_IP, PC_PORT);
  mirror_frame_t frame;
  while (true) {
    if (xQueueReceive(mirror_queue, &frame, portMAX_DELAY)) {
      sendto(sock, frame.data, frame.len, 0,
             (struct sockaddr *)&dest, sizeof(dest));
    }
  }
}

void sniffer_task() {
  while (!stdio_usb_connected());

  if (cyw43_arch_init()) {
    printf("[!] arch init failed\n");
    vTaskDelete(NULL);
  }

  cyw43_arch_enable_ap_mode(SSID, PASSWD, CYW43_AUTH_WPA2_AES_PSK);

  sniffer_queue = xQueueCreate(32, sizeof(packet_log_t));
  install_sniffer_hooks();

  ip4_addr_t gw, mask;
  IP4_ADDR(&gw, 192, 168, 4, 1);
  IP4_ADDR(&mask, 255, 255, 255, 0);
  dhcp_server_t dhcps;
  dhcp_server_init(&dhcps, &cyw43_state.netif[CYW43_ITF_AP], &gw, &mask);

  printf("[+] hotspot active\n");
  packet_log_t entry;
  char src_addr[15+5+1] = {0}, dst_addr[15+5+1] = {0};
  char* color;

  while (true) {
    if (xQueueReceive(sniffer_queue, &entry, portMAX_DELAY)) {
      sprintf(src_addr, "%s:%u", ip4addr_ntoa(&entry.src_ip), entry.src_port);
      sprintf(dst_addr, "%s:%u", ip4addr_ntoa(&entry.dst_ip), entry.dst_port);

      const char *proto_str = entry.protocol == IP_PROTO_TCP ? "TCP" :
                               entry.protocol == IP_PROTO_UDP ? "UDP" :
                               entry.protocol == IP_PROTO_ICMP? "ICMP" :
                               entry.protocol == IP_PROTO_UDPLITE ? "UDPLITE" :
                               entry.protocol == IP_PROTO_IGMP ? "IGMP" :
                               "OTHER";
      
      switch (entry.protocol) {
        case IP_PROTO_IGMP:     color = strdup("\x1b[38;5;51m");  break;  // Neon Cyan
        case IP_PROTO_UDPLITE:  color = strdup("\x1b[38;5;220m"); break;  // Gold
        case IP_PROTO_TCP:      color = strdup("\x1b[38;5;202m"); break;  // Neon Orange
        case IP_PROTO_UDP:      color = strdup("\x1b[38;5;245m"); break;  // Grey
        case IP_PROTO_ICMP:     color = strdup("\x1b[38;5;46m");  break;  // Neon Green
        default:                color = strdup("\x1b[0m");
      }
      printf(
          "%s"
          "%-8lu %-21s -> %-21s %-9s len=%-5u",
          color, entry.timestamp_ms, src_addr, dst_addr, proto_str, entry.len
      );

      if (entry.protocol == IP_PROTO_TCP) {
        printf("flags=%s%s%s%s",
          (entry.tcp_flags & TCP_SYN) ? "S" : "",
          (entry.tcp_flags & TCP_ACK) ? "A" : "",
          (entry.tcp_flags & TCP_FIN) ? "F" : "",
          (entry.tcp_flags & TCP_RST) ? "R" : "");
      }
      printf("\033[0m\n");
    }
  }
}

