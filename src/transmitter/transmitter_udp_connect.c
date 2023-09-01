#include "../../include/transmitter.h"
#include "net/netstack.h"
#include "net/routing/routing.h"
#include "../wsn_global.h"

static bool udp_connection_established = false;
/*---------------------------------------------------------------------------*/
PROCESS(transmitter_udp_connect, "transmitter_udp_connect");
/*---------------------------------------------------------------------------*/
static void
udp_rx_callback(struct simple_udp_connection *c,
				const uip_ipaddr_t *sender_addr,
				uint16_t sender_port,
				const uip_ipaddr_t *receiver_addr,
				uint16_t receiver_port,
				const uint8_t *data,
				uint16_t datalen)
{
	data_package * pck = (data_package *)data;
	if(pck->ack == 0x00)
	{
		LOG_INFO("TREC: received response '%.*s' from ", datalen - 2, (char *)pck->payload);
		LOG_INFO_6ADDR(sender_addr);
		LOG_INFO(" with seq number %u", pck->seq);

		print_full_log(pck, datalen, -1);
	}
	else 
	{
		LOG_INFO("TREC: received ack package %u from ", pck->seq);
		LOG_INFO_6ADDR(sender_addr);
		print_full_log(pck, datalen, -1);
	}
}


PROCESS_THREAD(transmitter_udp_connect, ev, data)
{
	static struct etimer timer;

	PROCESS_BEGIN();

	etimer_set(&timer, CLOCK_SECOND * 2);

	LOG_INFO("TUDPC: Establishing UDP Connection\n");
	rx_count = 0;

	while (1)
	{
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
		if (NETSTACK_ROUTING.node_is_reachable() &&
			NETSTACK_ROUTING.get_root_ipaddr(&dest_ipaddr))
		{
			simple_udp_register(&udp_conn, UDP_CLIENT_PORT, &dest_ipaddr,
			UDP_SERVER_PORT, udp_rx_callback);
			LOG_INFO("TUDPC: Established a connection\n");
			udp_connection_established = true;

			break;
		}
		else
		{
			LOG_INFO("TUDPC: Not reachable yet\n");
		}
		etimer_reset(&timer);
	}

	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
