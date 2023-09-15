
#include "transmitter/transmit_process.c"
#include "transmitter/transmitter_udp_connect.c"
#include "wsn_global.h"

#include <stdio.h>
#include <stdlib.h>


/*---------------------------------------------------------------------------*/
PROCESS(transmitter, "transmitter");
PROCESS(reader, "reader");
AUTOSTART_PROCESSES(&transmitter);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(transmitter, ev, data)
{
	static struct etimer timer;

	PROCESS_BEGIN();
	NETSTACK_RADIO.init();
	NETSTACK_RADIO.on();

	etimer_set(&timer, CLOCK_SECOND * 1);

	LOG_INFO("MAIN: Started the transmitter node\n");

	static char seq = 0;

	init_print_full_log();

	while (1)
	{
		/* Wait for the periodic timer to expire and then restart the timer. */
		// PROCESS_YIELD_UNTIL(etimer_expired(&timer));
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
			data_package * pck = (data_package *)malloc(sizeof(data_package));
			pck->ack = 0x00;
			pck->seq = seq;
			snprintf(pck->payload, sizeof(pck->payload), "hello");

			LOG_INFO("MAIN: Start sending process\n");
			// print_full_log(pck, sizeof(data_package), clock_time());
			process_start(&transmit_process, pck);
			seq += 1;

		etimer_reset(&timer);
	}

	PROCESS_END();
}
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(reader, ev, data)
{
	PROCESS_BEGIN();
	while(1)
	{
		data_package * pck = (data_package *)malloc(sizeof(data_package));
		if(NETSTACK_CONF_RADIO.read(pck, 32))
		{
			print_full_log(pck, sizeof(data_package), clock_time());
		}
		free(pck);

		// wsn_radio.read()
	}
	PROCESS_END();
}
