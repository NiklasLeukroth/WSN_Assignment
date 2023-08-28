#ifndef WSN_GLOBAL
#define WSN_GLOBAL

// just for logging / printing
#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_NONE

#define PACKAGE_PAYLOAD_LENGTH 30

typedef struct _data_package {
  char ack;
  char seq;
  char payload[PACKAGE_PAYLOAD_LENGTH];
} data_package;

typedef struct _short_package {
  char ack;
  char seq;
} short_package;

void print_data_package(data_package *pck, uint16_t datalen) {
    printf("data package:\n\tACK: %u\n\tSEQ: %u\n\tPAYLOAD: %.*s\n", pck->ack, pck->seq, datalen, pck->payload);
}



#endif // ifndef WSN_GLOBAL
