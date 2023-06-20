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

#endif // ifndef WSN_GLOBAL