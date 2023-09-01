#include "contiki.h"
#include "net/ipv6/simple-udp.h"
#include "../src/wsn_global.h"

static struct simple_udp_connection udp_conn_receiver;
static struct simple_udp_connection udp_conn_transmitter;
static uip_ipaddr_t receiver_address;
static uip_ipaddr_t transmitter_address;


#define WITH_SERVER_REPLY 1
#define RECEIVER_PORT 8765
#define SENDER_PORT	5678
#define UDP_CLIENT_PORT 8765+1
#define UDP_SERVER_PORT 5678+1
