#include "../include/transmitter.h"

/*---------------------------------------------------------------------------*/
PROCESS(transmit_process, "transmit_process");
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(transmit_process, ev, data)
{
	static char str[32];
	PROCESS_BEGIN();
	snprintf(str, sizeof(str), "hello %" PRIu32 "", rx_count);
	simple_udp_sendto(&udp_conn, str, strlen(str), &dest_ipaddr);
	LOG_INFO("TRANS: Sent package\n");
	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
