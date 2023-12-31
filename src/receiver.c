#include <stdlib.h>

#include "wsn_global.h"

// communication
#define REMOTE_PORT	8765+1
#define LOCAL_PORT	5678+1
static struct simple_udp_connection udp_conn;

PROCESS(receiver_process, "receiver process");
PROCESS(receiver_connect, "receiver connect process");
AUTOSTART_PROCESSES(&receiver_connect);

struct etimer receiver_timeout;

static void udp_rx_callback(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{
  etimer_reset(&receiver_timeout);

  data_package *pck = (data_package *) data;

  if (pck->ack == 0x00) {
    short_package acknowlegement = {0xFF, pck->seq};
    print_full_log(pck, datalen, -1);
    simple_udp_sendto(&udp_conn, &acknowlegement, 2, sender_addr);
    print_short_log(&acknowlegement, 2, -1);
  }
}

PROCESS_THREAD(receiver_connect, ev, data)
{
  static struct etimer periodic_timer;

  PROCESS_BEGIN();

  uip_ipaddr_t root_ip;
  etimer_set(&periodic_timer, 2*CLOCK_SECOND);

  while (true) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

    LOG_INFO("searching for connection ...");

    if (NETSTACK_ROUTING.node_is_reachable() && NETSTACK_ROUTING.get_root_ipaddr(&root_ip)) {

      LOG_INFO("registering UDP connection\n");
      simple_udp_register(&udp_conn, LOCAL_PORT, &root_ip, REMOTE_PORT, udp_rx_callback);

      break;
    }

    etimer_reset(&periodic_timer);
  }

  LOG_INFO("######## registered ...\n");
  LOG_INFO("root_ip ");
  LOG_INFO_6ADDR(&root_ip);
  LOG_INFO("\n");

  short_package hello_pkg = {0x00, 0};
  simple_udp_send(&udp_conn, &hello_pkg, 2);
  etimer_set(&receiver_timeout, 2*CLOCK_SECOND);

  init_print_full_log();

  PROCESS_END();
}


PROCESS_THREAD(receiver_process, ev, data)
{
  PROCESS_BEGIN();

  // while (true)
  // {
  //   PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&receiver_timeout));

  //   if (!NETSTACK_ROUTING.node_is_reachable()) {
  //     connected_to_transceiver = false;
  //     LOG_INFO("Lost connection. Reestablishing ...");
  //     if (!connecting_to_transceiver) {
  //       process_start(&receiver_connect, NULL);
  //     }
  //   }

  //   etimer_reset(&receiver_timeout);
  // }
  
  
  PROCESS_END();
}
