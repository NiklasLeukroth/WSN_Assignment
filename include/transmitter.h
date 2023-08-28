#include "contiki.h"
#include "net/ipv6/simple-udp.h"

static uint32_t rx_count;
static struct simple_udp_connection udp_conn;
static uip_ipaddr_t dest_ipaddr;

#define WITH_SERVER_REPLY 1
#define UDP_CLIENT_PORT 8765
#define UDP_SERVER_PORT 5678
