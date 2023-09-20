#ifndef WSN_GLOBAL
#define WSN_GLOBAL

// just for logging / printing
#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_NONE

#define ZOUL_CONF_USE_CC1200_RADIO 0

#include "net/routing/routing.h"
#include "net/netstack.h"
#include "net/ipv6/simple-udp.h"

#define PACKAGE_PAYLOAD_LENGTH 30

int dropped_counter = 0;

typedef struct _short_package {
  char ack;
  uint32_t seq;
} short_package;

typedef struct _data_package {
  char ack;
  uint32_t seq;
  char payload[PACKAGE_PAYLOAD_LENGTH];
} data_package;

void init_print_data_package(){
  printf("SEQ;ACK;TIMESTAMP;PAYLOAD;");
}

void print_data_package(data_package *pkg, uint16_t datalen, long timestamp) {
  printf("%ld;%d;%ld;%*.s;", pkg->seq, pkg->ack, timestamp, datalen, pkg->payload);
}

void print_short_package(short_package *pkg, uint16_t datalen, long timestamp) {
  printf("%ld;%d;%ld;;", pkg->seq, pkg->ack, timestamp);
}

void init_print_full_log() {
  init_print_data_package();
  printf("RSSI;LQI");
  printf("\n");
}

void print_quality_metrics() {
  radio_value_t rssi, lqi;
  if (NETSTACK_RADIO.get_value(RADIO_PARAM_LAST_RSSI, &rssi) != RADIO_RESULT_OK) {
    rssi = 0xFFFF;
  }

  if (NETSTACK_RADIO.get_value(RADIO_PARAM_LAST_LINK_QUALITY, &lqi) != RADIO_RESULT_OK) {
    lqi = 0xFFFF;
  }

  printf("%d;%d", rssi, lqi);
}

void print_full_log(data_package *pkg, uint16_t datalen, long timestamp) {
  if (timestamp == -1 && NETSTACK_RADIO.get_object(RADIO_PARAM_LAST_PACKET_TIMESTAMP, &timestamp, sizeof(long)) != RADIO_RESULT_OK) {
    timestamp = 0xFFFF;
  }

  print_data_package(pkg, datalen, timestamp);
  print_quality_metrics();

  printf("\n");
}

void print_short_log(short_package *pkg, uint16_t datalen, long timestamp) {
  if (timestamp == -1 && NETSTACK_RADIO.get_object(RADIO_PARAM_LAST_PACKET_TIMESTAMP, &timestamp, sizeof(long)) != RADIO_RESULT_OK) {
    timestamp = 0xFFFF;
  }

  print_short_package(pkg, datalen, timestamp);
  print_quality_metrics();

  printf("\n");
}

#endif // ifndef WSN_GLOBAL
