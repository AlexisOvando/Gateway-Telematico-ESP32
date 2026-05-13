#ifndef EVENT_H
#define EVENT_H

#include <Arduino.h>

enum EventType
{
    EVENT_NONE = 0,

    EVENT_UART_RX,

    EVENT_TCP_CONNECTED,
    EVENT_TCP_DISCONNECTED,

    EVENT_WIFI_CONNECTED,
    EVENT_WIFI_DISCONNECTED
};

typedef struct
{
    EventType type;

    uint8_t data[100];

    size_t length;

} AppEvent;

#endif