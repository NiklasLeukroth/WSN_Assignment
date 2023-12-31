#include <stdio.h>
#include <stdlib.h>

#include "wsn_global.h"
#include "../include/transceiver.h"

/*---------------------------------------------------------------------------*/
PROCESS(transceiver_process_receiver, "transceiver_process_receiver");
AUTOSTART_PROCESSES(&transceiver_process_receiver);
/*---------------------------------------------------------------------------*/

static void udp_tc_callback(struct simple_udp_connection *c,
         const uip_ipaddr_t *sender_addr,
         uint16_t sender_port,
         const uip_ipaddr_t *receiver_addr,
         uint16_t receiver_port,
         const uint8_t *data,
         uint16_t datalen)
{
    data_package *pck = (data_package *) data;

    if (pck->seq == (uint32_t)0)
    {
        printf("connected\n");
        receiver_address = *sender_addr;
        return;
    }

    if (pck->ack == 0x00) 
    {
        print_full_log(pck, datalen, clock_time());
        transmitter_address = *sender_addr;
        short_package acknowlegement = {0xF0, pck->seq};
        simple_udp_sendto(&udp_conn_transmitter, &acknowlegement, 5, &transmitter_address);
        // printf("%d", datalen);
        print_short_log(&acknowlegement, 5, clock_time());
        simple_udp_sendto(&udp_conn_receiver, pck, datalen, &receiver_address);
        print_full_log(pck, datalen, clock_time());
    }
    else if(pck->ack == 0xFF)
    {
        print_short_log((short_package*)pck, datalen, clock_time());
        simple_udp_sendto(&udp_conn_transmitter, pck, datalen, &transmitter_address);
        print_short_log((short_package*)pck, datalen, clock_time());
    }
    else
    {
        printf("error\n");
    }
}



PROCESS_THREAD(transceiver_process_receiver, ev, data)
{
    PROCESS_BEGIN();

    LOG_INFO("registering UDP connection\n");

    NETSTACK_ROUTING.root_start();

    init_print_full_log();
    simple_udp_register(&udp_conn_transmitter, SENDER_PORT, NULL, RECEIVER_PORT, udp_tc_callback);
    simple_udp_register(&udp_conn_receiver, UDP_CLIENT_PORT, NULL, UDP_SERVER_PORT, udp_tc_callback);
    LOG_INFO("registered UDP connection\n");

    PROCESS_END();
}