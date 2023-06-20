#ifndef WSN_GLOBAL
#define WSN_GLOBAL

typedef struct _data_package {
  char ack;
  char seq;
  char payload[30];
} data_package;

typedef struct _short_package {
  char ack;
  char seq;
} short_package;

void print_data_package(data_package *pck, uint16_t datalen) {
    printf("data package:\n\tACK: %u\n\tSEQ: %u\n\tPAYLOAD: %.*s\n", pck->ack, pck->seq, datalen-2, pck->payload);
}

#endif // ifndef WSN_GLOBAL
