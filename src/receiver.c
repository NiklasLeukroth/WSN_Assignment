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
  PROCESS_BEGIN();

  LOG_INFO("registering UDP connection\n");

  NETSTACK_ROUTING.root_start();

  init_print_full_log();
  do {
    simple_udp_register(&udp_conn, LOCAL_PORT, NULL, REMOTE_PORT, udp_rx_callback);

    LOG_INFO("######## registered ...");
  } while (!NETSTACK_ROUTING.node_is_reachable());

  PROCESS_END();
}
