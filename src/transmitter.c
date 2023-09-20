
#include "transmitter/transmit_process.c"
#include "transmitter/transmitter_udp_connect.c"
#include "wsn_global.h"

#include <stdio.h>
#include <stdlib.h>


/*---------------------------------------------------------------------------*/
PROCESS(transmitter, "transmitter");
AUTOSTART_PROCESSES(&transmitter, &transmitter_udp_connect);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(transmitter, ev, data)
{
	static struct etimer timer;

	PROCESS_BEGIN();

	etimer_set(&timer, CLOCK_SECOND * 1);

	LOG_INFO("MAIN: Started the transmitter node\n");

	static uint32_t seq = 1;

	init_print_full_log();

	while (1)
	{
		/* Wait for the periodic timer to expire and then restart the timer. */
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
		if (dropped_counter >= 3)
		{
			udp_connection_established = false;
			process_start(&transmitter_udp_connect, NULL);
		}
		if (udp_connection_established)
		{
			dropped_counter ++;
			data_package * pck = (data_package *)malloc(sizeof(data_package));
			pck->ack = 0x00;
			pck->seq = seq;
			snprintf(pck->payload, sizeof(pck->payload), "hello");
			LOG_INFO("MAIN: Start sending process\n");
			print_full_log(pck, sizeof(data_package), clock_time());
			process_start(&transmit_process, pck);
			seq += 1;
		}
		else 
		{
			LOG_INFO("MAIN: Waiting on udp connection\n");
		}
		etimer_reset(&timer);
	}

	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
