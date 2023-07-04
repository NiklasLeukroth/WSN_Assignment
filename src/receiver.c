#define ZOUL_CONF_USE_CC1200_RADIO 1

#include "contiki.h"

#include "net/netstack.h"
// #include "arch/dev/radio/cc1200/cc1200.c"

#include "wsn_global.h"

// just for logging / printing
#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

PROCESS(receiver_process, "receiver process");
AUTOSTART_PROCESSES(&receiver_process);

void rx_callback() {
  int pkg;

  // NETSTACK_RADIO.read(&pkg, sizeof(data_package));
  NETSTACK_RADIO.read(&pkg, sizeof(int));

  printf("received ");
  printf("%i\n", pkg);
  // print_data_package(&pkg, PACKAGE_PAYLOAD_LENGTH);
}

PROCESS_THREAD(receiver_process, ev, data)
{
    PROCESS_BEGIN();

    NETSTACK_RADIO.set_value(RADIO_PARAM_RX_MODE, 1);
    NETSTACK_RADIO.init();
    NETSTACK_RADIO.on();

    int packages_received = 0;

    while(1) {
      while(!NETSTACK_RADIO.pending_packet());
      printf("#### %i ####\n", packages_received);
      rx_callback();
      packages_received++;
    }

    PROCESS_END();
}
