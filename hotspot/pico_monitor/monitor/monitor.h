#pragma once

#include <FreeRTOS.h>
#include <semphr.h>

#include <pico/stdio.h>
#include <pico/stdlib.h>
#include <pico/cyw43_arch.h>

#include <lwip/netif.h>
#include <lwip/pbuf.h>
#include <lwip/prot/ethernet.h>

#define MAX_CLIENTS 8

typedef struct {
  uint8_t mac[6];
  uint32_t bytes_rx;   // from client -> Pico
  uint32_t bytes_tx;   // Pico -> client
  bool used;
} client_stats_t;

extern client_stats_t stats[MAX_CLIENTS];
extern SemaphoreHandle_t stats_mutex;

void install_traffic_hooks(void);

