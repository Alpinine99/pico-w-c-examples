#ifndef _LWIPOPTS_H
#define _LWIPOPTS_H

// Common settings used in most Pico W FreeRTOS projects
#define NO_SYS                          0
#define LWIP_SOCKET                     1
#define LWIP_NETCONN                    1
#define LWIP_NETIF_API                  1

// Core locking and threading configuration required for FreeRTOS
#define TCPIP_CORE_LOCKING              1
#define SYS_LIGHTWEIGHT_PROT            1

// Memory settings
#define MEM_LIBC_MALLOC                 0
#define MEM_ALIGNMENT                   4
#define MEM_SIZE                        16000
#define MEMP_NUM_TCP_SEG                32
#define MEMP_NUM_ARP_QUEUE              10

// Pbuf pool settings
#define PBUF_POOL_SIZE                  24
#define PBUF_POOL_BUFSIZE               1514

// TCP/IP thread specifications
#define TCPIP_THREAD_NAME               "TCP/IP"
#define TCPIP_THREAD_STACKSIZE          1024
#define TCPIP_THREAD_PRIO               3

// Protocol features
#define ARP_QUEUEING                    1
#define ICMP                            1
#define LWIP_DHCP                       1
#define LWIP_UDP                        1
#define LWIP_TCP                        1

// TCP tuning parameters
#define TCP_MSS                         1460
#define TCP_WND                         (8 * TCP_MSS)
#define TCP_SND_BUF                     (8 * TCP_MSS)
#define TCP_SND_QUEUELEN                16

// Loopback settings
#define LWIP_NETIF_LOOPBACK             1

// Prevent lwIP from redefining struct timeval, which conflicts with Newlib
#define LWIP_TIMEVAL_PRIVATE 0

#endif /* _LWIPOPTS_H */

