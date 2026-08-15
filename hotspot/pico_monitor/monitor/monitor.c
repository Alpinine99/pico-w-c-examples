#include "monitor.h"

static netif_input_fn original_input;
static netif_linkoutput_fn original_linkoutput;

static client_stats_t *get_or_create_entry(const uint8_t *mac) {
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (stats[i].used && memcmp(stats[i].mac, mac, 6) == 0) return &stats[i];
  }
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (!stats[i].used) {
      stats[i].used = true;
      memcpy(stats[i].mac, mac, 6);
      return &stats[i];
    }
  }
  return NULL; // table full
}

static err_t traffic_input_hook(struct pbuf *p, struct netif *inp) {
  if (p->len >= sizeof(struct eth_hdr)) {
    struct eth_hdr *hdr = (struct eth_hdr *)p->payload;
    uint8_t *src_mac = (uint8_t *)&hdr->src;

    xSemaphoreTake(stats_mutex, portMAX_DELAY);
    client_stats_t *entry = get_or_create_entry(src_mac);
    if (entry) entry->bytes_rx += p->tot_len;
    xSemaphoreGive(stats_mutex);
  }
  return original_input(p, inp);
}

static err_t traffic_output_hook(struct netif *nif, struct pbuf *p) {
  if (p->len >= sizeof(struct eth_hdr)) {
    struct eth_hdr *hdr = (struct eth_hdr *)p->payload;
    uint8_t *dst_mac = (uint8_t *)&hdr->dest;

    xSemaphoreTake(stats_mutex, portMAX_DELAY);
    client_stats_t *entry = get_or_create_entry(dst_mac);
    if (entry) entry->bytes_tx += p->tot_len;
    xSemaphoreGive(stats_mutex);
  }
  return original_linkoutput(nif, p);
}

void install_traffic_hooks(void) {
  struct netif *ap_netif = &cyw43_state.netif[CYW43_ITF_AP];

  original_input = ap_netif->input;
  ap_netif->input = traffic_input_hook;

  original_linkoutput = ap_netif->linkoutput;
  ap_netif->linkoutput = traffic_output_hook;
}

