#include "../../include/transmitter.h"
#include <stdlib.h>

/*---------------------------------------------------------------------------*/
PROCESS(transmit_process, "transmit_process");
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(transmit_process, ev, data)
{
	PROCESS_BEGIN();
	NETSTACK_CONF_RADIO.send(data, 32);
	// printf("::%d", remains);
	// simple_udp_send(&udp_conn, data, 32);
	LOG_INFO("TRANS: Sent package\n");
	free(data);
	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
