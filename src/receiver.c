#include "contiki.h"

#include "arch/dev/radio/cc1200/cc1200.c"

#include "wsn_global.h"

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
AUTOSTART_PROCESSES(&receiver_process);

// void acknowlege(char seq, struct simple_udp_connection *c, const uip_ipaddr_t *sender_addr) {
//     short_package acknowlegement = {0xFF, seq};

//     printf("Sending Acknowlegement with SEQ %i.\n", seq);
//     simple_udp_sendto(&udp_conn, &acknowlegement, 2, sender_addr);
// }

// static void udp_rx_callback(struct simple_udp_connection *c,
//          const uip_ipaddr_t *sender_addr,
//          uint16_t sender_port,
//          const uip_ipaddr_t *receiver_addr,
//          uint16_t receiver_port,
//          const uint8_t *data,
//          uint16_t datalen)
// {
//   data_package *pck = (data_package *) data;
//   LOG_INFO_6ADDR(sender_addr);
//   printf(" => \n");

//   if (pck->ack == 0x00) {
//     print_data_package(pck, datalen);

//     short_package acknowlegement = {0xFF, pck->seq};

//     printf("Sending Acknowlegement with SEQ %i.\n", pck->seq);
//     simple_udp_sendto(&udp_conn, &acknowlegement, 2, sender_addr);
//   } else {
//     printf("Received Acknowlegement with SEQ %i.\n", pck->seq);
//   }
// }

void rx_callback() {

}

PROCESS_THREAD(receiver_process, ev, data)
{
    PROCESS_BEGIN();

    cc1200_driver.init();
    cc1200_driver.on();

    while(1) {
      PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_MSG);

      rx_callback();
    }

    PROCESS_END();
}
