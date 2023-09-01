#include <stdlib.h>

#include "wsn_global.h"

// communication
#define REMOTE_PORT	8765
#define LOCAL_PORT	5678
static struct simple_udp_connection udp_conn;

PROCESS(receiver_process, "receiver process");
AUTOSTART_PROCESSES(&receiver_process);

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

  if (pck->ack == 0x00) {
    short_package acknowlegement = {0xFF, pck->seq};
    simple_udp_sendto(&udp_conn, &acknowlegement, 2, sender_addr);

    print_full_log(pck, datalen, -1);
  }
}


PROCESS_THREAD(receiver_process, ev, data)
{
  static struct etimer periodic_timer;

  PROCESS_BEGIN();

  uip_ipaddr_t root_ip;
  etimer_set(&periodic_timer, 2*CLOCK_SECOND);

  // NETSTACK_ROUTING.root_start();

  while (true) {
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

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

  // etimer_reset_with_new_interval(&periodic_timer, 5*CLOCK_SECOND);
  // PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&periodic_timer));

  short_package hello_pkg = {0x00, (char) -1};
  simple_udp_send(&udp_conn, &hello_pkg, 2);

  init_print_full_log();
  
  PROCESS_END();
}
