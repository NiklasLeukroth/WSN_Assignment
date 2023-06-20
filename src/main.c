#include "contiki.h"

#include <stdio.h>

#include "receiver/receiver_process.c"
/*---------------------------------------------------------------------------*/
PROCESS(main_process, "main_process");
AUTOSTART_PROCESSES(&main_process, &receiver_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(main_process, ev, data)
{
  // static struct etimer timer;

  PROCESS_BEGIN();

  // etimer_set(&timer, CLOCK_SECOND * 4);

  // while(1) {    
  //   PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&timer));
  //   process_post(&receiver_process, PROCESS_EVENT_RECEIVER, &data);

  //   etimer_reset(&timer);
  // }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
