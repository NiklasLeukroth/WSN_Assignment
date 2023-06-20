#include "contiki.h"

#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"

#include "../wsn_global.h"

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

void acknowlege(char seq, struct simple_udp_connection *c, const uip_ipaddr_t *sender_addr) {
    short_package acknowlegement = {0xFF, seq};

    printf("Sending Acknowlegement with SEQ %i.\n", seq);
    simple_udp_sendto(&udp_conn, &acknowlegement, 2, sender_addr);
}

static void udp_rx_callback(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{
  data_package *pck = (data_package *) data;
  LOG_INFO_6ADDR(sender_addr);
  printf(" => \n");

  if (pck->ack == 0x00) {
    print_data_package(pck, datalen);

    short_package acknowlegement = {0xFF, pck->seq};

    printf("Sending Acknowlegement with SEQ %i.\n", pck->seq);
    simple_udp_sendto(&udp_conn, &acknowlegement, 2, sender_addr);
  } else {
    printf("Received Acknowlegement with SEQ %i.\n", pck->seq);
  }
}

PROCESS_THREAD(receiver_process, ev, data)
{
    // static struct etimer timer;

    PROCESS_BEGIN();

    // etimer_set(&timer, CLOCK_SECOND * 1);

    LOG_INFO("registering UDP connection\n");
    NETSTACK_ROUTING.root_start();
    simple_udp_register(&udp_conn, SENDER_PORT, NULL, RECEIVER_PORT, udp_rx_callback);
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
