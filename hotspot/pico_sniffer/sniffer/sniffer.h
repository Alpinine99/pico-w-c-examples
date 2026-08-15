#pragma once

#include <FreeRTOS.h>
#include <queue.h>

#include <pico/stdlib.h>
#include <pico/cyw43_arch.h>

#include <lwip/prot/ip4.h>
#include <lwip/prot/tcp.h>
#include <lwip/prot/udp.h>
#include <lwip/prot/ethernet.h>

#define MIRROR_SNAPLEN 512


typedef struct {
  uint32_t timestamp_ms;
  uint8_t  src_mac[6];
  uint8_t  dst_mac[6];
  ip4_addr_t src_ip;
  ip4_addr_t dst_ip;
  uint16_t src_port;
  uint16_t dst_port;
  uint8_t  protocol;
  uint16_t len;
  uint8_t  tcp_flags;
} packet_log_t;

typedef struct {
    uint16_t len;
    uint8_t data[MIRROR_SNAPLEN];
} mirror_frame_t;

typedef struct {
    ip4_addr_t ip;
    uint16_t port;
} endpoint_t;

extern QueueHandle_t mirror_queue;
extern QueueHandle_t sniffer_queue;

void install_sniffer_hooks();
void mirror_endpoint_init(const char *ip_str, uint16_t port);
void endpoint_init(endpoint_t *ep, const char *ip_str, uint16_t port);
bool endpoint_matches(const endpoint_t *ep, const ip4_addr_t *ip, uint16_t port);

