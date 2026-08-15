#include "sniffer.h"


static endpoint_t mirror_endpoint;
static netif_input_fn original_input;
static netif_linkoutput_fn original_linkoutput;

static void sniff_packet(struct pbuf *p, const uint8_t *src_mac, const uint8_t *dst_mac) {
  if (p->len < sizeof(struct eth_hdr) + sizeof(struct ip_hdr)) return;

  uint8_t *payload = (uint8_t *)p->payload;
  struct ip_hdr *iph = (struct ip_hdr *)(payload + sizeof(struct eth_hdr));

  // Only handle IPv4 for now
  if (IPH_V(iph) != 4) return;

  uint8_t ihl = IPH_HL_BYTES(iph);       // header length in bytes
  uint8_t proto = IPH_PROTO(iph);

  packet_log_t entry = {0};
  entry.timestamp_ms = xTaskGetTickCount() * portTICK_PERIOD_MS;
  memcpy(entry.src_mac, src_mac, 6);
  memcpy(entry.dst_mac, dst_mac, 6);
  ip4_addr_copy(entry.src_ip, iph->src);
  ip4_addr_copy(entry.dst_ip, iph->dest);
  entry.protocol = proto;
  entry.len = lwip_ntohs(IPH_LEN(iph));

  uint8_t *l4 = payload + sizeof(struct eth_hdr) + ihl;

  if (proto == IP_PROTO_TCP && p->len >= (l4 - payload) + sizeof(struct tcp_hdr)) {
    struct tcp_hdr *tcph = (struct tcp_hdr *)l4;
    entry.src_port = lwip_ntohs(tcph->src);
    entry.dst_port = lwip_ntohs(tcph->dest);
    entry.tcp_flags = TCPH_FLAGS(tcph);
  } else if (proto == IP_PROTO_UDP && p->len >= (l4 - payload) + sizeof(struct udp_hdr)) {
    struct udp_hdr *udph = (struct udp_hdr *)l4;
    entry.src_port = lwip_ntohs(udph->src);
    entry.dst_port = lwip_ntohs(udph->dest);
  }

  xQueueSend(sniffer_queue, &entry, 0);
}

void endpoint_init(endpoint_t *ep, const char *ip_str, uint16_t port) {
  ip4addr_aton(ip_str, &ep->ip);
  ep->port = port;
}

void mirror_endpoint_init(const char *ip_str, uint16_t port) {
  endpoint_init(&mirror_endpoint, ip_str, port);
}

bool endpoint_matches(const endpoint_t *ep, const ip4_addr_t *ip, uint16_t port) {
  return ip4_addr_cmp(ip, &ep->ip) && ep->port == port;
}

static void mirror_packet(struct pbuf *p) {
  mirror_frame_t frame;
  frame.len = pbuf_copy_partial(p, frame.data, MIRROR_SNAPLEN, 0);
  xQueueSend(mirror_queue, &frame, 0);
}

static bool is_mirror_traffic(struct pbuf *p) {
  if (p->len < sizeof(struct eth_hdr) + sizeof(struct ip_hdr) + sizeof(struct udp_hdr)) {
    return false;   // headers span multiple pbufs (rare) - just don't treat as mirror traffic
  }

  uint8_t *data = (uint8_t *)p->payload;
  struct ip_hdr *iph = (struct ip_hdr *)(data + sizeof(struct eth_hdr));

  if (IPH_PROTO(iph) != IP_PROTO_UDP) return false;

  uint8_t ihl = IPH_HL_BYTES(iph);
  struct udp_hdr *udph = (struct udp_hdr *)(data + sizeof(struct eth_hdr) + ihl);

  ip4_addr_t dest_ip;
  ip4_addr_copy(dest_ip, iph->dest);

  return endpoint_matches(&mirror_endpoint, &dest_ip, lwip_ntohs(udph->dest));
}

static err_t sniffer_input_hook(struct pbuf *p, struct netif *inp) {
  if (p->len >= sizeof(struct eth_hdr)) {
    struct eth_hdr *hdr = (struct eth_hdr *)p->payload;
    uint8_t *src_mac = (uint8_t *)&hdr->src;
    uint8_t *dst_mac = (uint8_t *)&hdr->dest;
    sniff_packet(p, src_mac, dst_mac);
    mirror_packet(p);
  }
  return original_input(p, inp);
}

static err_t sniffer_output_hook(struct netif *nif, struct pbuf *p) {
  if (!is_mirror_traffic(p) && p->len >= sizeof(struct eth_hdr)) {
    struct eth_hdr *hdr = (struct eth_hdr *)p->payload;
    uint8_t *src_mac = (uint8_t *)&hdr->src;
    uint8_t *dst_mac = (uint8_t *)&hdr->dest;
    sniff_packet(p, src_mac, dst_mac);
    mirror_packet(p);
  }
  return original_linkoutput(nif, p);
}

void install_sniffer_hooks(void) {
  struct netif *ap_netif = &cyw43_state.netif[CYW43_ITF_AP];

  original_input = ap_netif->input;
  ap_netif->input = sniffer_input_hook;

  original_linkoutput = ap_netif->linkoutput;
  ap_netif->linkoutput = sniffer_output_hook;
}

