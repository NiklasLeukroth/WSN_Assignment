#include "../include/transmitter.h"
#include "net/netstack.h"
#include "net/routing/routing.h"

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

	LOG_INFO("TUDPC: Received response '%.*s' from ", datalen, (char *)data);
	LOG_INFO_6ADDR(sender_addr);
#if LLSEC802154_CONF_ENABLED
	LOG_INFO_("TUDPC: LLSEC LV:%d", uipbuf_get_attr(UIPBUF_ATTR_LLSEC_LEVEL));
#endif
	LOG_INFO_("\n");
	rx_count++;
}

PROCESS_THREAD(transmitter_udp_connect, ev, data)
{
	static struct etimer timer;

	PROCESS_BEGIN();

	etimer_set(&timer, CLOCK_SECOND * 5);

	LOG_INFO("TUDPC: Establishing UDP Connection\n");
	rx_count = 0;
	simple_udp_register(&udp_conn, UDP_CLIENT_PORT, NULL,
						UDP_SERVER_PORT, udp_rx_callback);
	while (1)
	{
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
		if (NETSTACK_ROUTING.node_is_reachable() &&
			NETSTACK_ROUTING.get_root_ipaddr(&dest_ipaddr))
		{
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
