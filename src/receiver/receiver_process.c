#include "contiki.h"

#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"

#define PROCESS_EVENT_RECEIVER 101

// just for logging / printing
#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

// communication
#define RECEIVER_PORT	8765
#define SENDER_PORT	5678
static struct simple_udp_connection udp_conn;

PROCESS(receiver_process, "receiver process");

static void udp_rx_callback(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{

  LOG_INFO("Received response '%.*s' from ", datalen, (char *) data);
  LOG_INFO_6ADDR(sender_addr);
  LOG_INFO_("\n");
}

PROCESS_THREAD(receiver_process, ev, data)
{
    // static struct etimer timer;

    PROCESS_BEGIN();

    // etimer_set(&timer, CLOCK_SECOND * 1);

    LOG_INFO("registering UDP connection\n");
    NETSTACK_ROUTING.root_start();
    simple_udp_register(&udp_conn, RECEIVER_PORT, NULL, SENDER_PORT, udp_rx_callback);
    LOG_INFO("registered UDP connection\n");

    // while(1) {
    //     PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));

    //     uip_ipaddr_t recv_ipaddr;
    //     if (NETSTACK_ROUTING.node_is_reachable() && NETSTACK_ROUTING.get_root_ipaddr(&recv_ipaddr)) {
    //         // printf("Thanks for calling the receiver!\n");
    //         printf("ip address: ");
    //         LOG_INFO_6ADDR(&recv_ipaddr);
    //         printf("\n\n");
    //     }

    //     etimer_reset(&timer);
    // }

    PROCESS_END();
}
