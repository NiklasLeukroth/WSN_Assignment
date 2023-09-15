#include <stdlib.h>

#include "wsn_global.h"

// communication
#define REMOTE_PORT	8765+1
#define LOCAL_PORT	5678+1

PROCESS(receiver_process, "receiver process");
AUTOSTART_PROCESSES(&receiver_process);

struct etimer receiver_timeout;

// static void udp_rx_callback(_data_package* pck)
// {
//   etimer_reset(&receiver_timeout);

//   data_package *pck = (data_package *) data;

//   if (pck->ack == 0x00) {
//     short_package acknowlegement = {0xFF, pck->seq};
//     simple_udp_sendto(&udp_conn, &acknowlegement, 2, sender_addr);

//     print_full_log(pck, PACKAGE_PAYLOAD_LENGTH, -1);
//   }
// }


PROCESS_THREAD(receiver_process, ev, data)
{
  static data_package received_package;

  PROCESS_BEGIN();

  NETSTACK_RADIO.init();
  NETSTACK_RADIO.on();

  while (true)
  {
    // PROCESS_YIELD_UNTIL(NETSTACK_RADIO.pending_packet());


    if (NETSTACK_RADIO.pending_packet()) {
      int length = NETSTACK_RADIO.read(&received_package, PACKAGE_LENGTH);
      LOG_INFO("received package of length %d\n", length);

      // printf((char*) &received_package);

      print_full_log(&received_package, PACKAGE_PAYLOAD_LENGTH, clock_time());
    }

  }
  

  
  PROCESS_END();
}
