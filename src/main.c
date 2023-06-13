
#include "transmit_process.c"
#include "transmitter_udp_connect.c"

#include <stdio.h>

/*---------------------------------------------------------------------------*/
PROCESS(main_process, "main_process");
AUTOSTART_PROCESSES(&main_process, &transmitter_udp_connect);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(main_process, ev, data)
{
	static struct etimer timer;

	PROCESS_BEGIN();

	etimer_set(&timer, CLOCK_SECOND * 5);

	LOG_INFO("MAIN: Started the transmitter node\n");

	while (1)
	{
		/* Wait for the periodic timer to expire and then restart the timer. */
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
		if (udp_connection_established)
		{
			LOG_INFO("MAIN: Start sending process\n");
			process_start(&transmit_process, NULL);
			process_post(&transmit_process, 30, &data);
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
