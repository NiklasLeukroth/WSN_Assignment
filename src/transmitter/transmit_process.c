#include "../../include/transmitter.h"
#include <stdlib.h>

/*---------------------------------------------------------------------------*/
PROCESS(transmit_process, "transmit_process");
/*---------------------------------------------------------------------------*/

PROCESS_THREAD(transmit_process, ev, data)
{
	PROCESS_BEGIN();
	simple_udp_send(&udp_conn, data, PACKAGE_PAYLOAD_LENGTH + 5);
	LOG_INFO("TRANS: Sent package\n");
	free(data);
	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
