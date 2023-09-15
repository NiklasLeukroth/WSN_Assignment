#include <stdlib.h>

#include "wsn_global.h"

// communication
#define REMOTE_PORT	8765+1
#define LOCAL_PORT	5678+1
static struct simple_udp_connection udp_conn;

PROCESS(receiver_process, "receiver process");
AUTOSTART_PROCESSES(&receiver_process);

struct etimer receiver_timeout;

static void udp_rx_callback()
{
  etimer_reset(&receiver_timeout);

  data_package *pck = (data_package *) data;

  if (pck->ack == 0x00) {
    short_package acknowlegement = {0xFF, pck->seq};
    simple_udp_sendto(&udp_conn, &acknowlegement, 2, sender_addr);

    print_full_log(pck, datalen, -1);
  }
}


PROCESS_THREAD(receiver_process, ev, data)
{
  PROCESS_BEGIN();

  wsn_radio.init();
  wsn_radio.on();

  while (true)
  {
    PROCESS_YIELD_UNTIL(wsn_radio.pending_packet());
  }
  

  
  PROCESS_END();
}
