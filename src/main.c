#include "contiki.h"

#include <stdio.h>
/*---------------------------------------------------------------------------*/
PROCESS(main_process, "main_process");
AUTOSTART_PROCESSES(&main_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(main_process, ev, data)
{
  static struct etimer timer;

  PROCESS_BEGIN();

  /* Setup a periodic timer that expires after 10 seconds. */
  etimer_set(&timer, CLOCK_SECOND * 10);

  while(1) {
    printf("Hello, world\n");

    /* Wait for the periodic timer to expire and then restart the timer. */
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
    etimer_reset(&timer);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
