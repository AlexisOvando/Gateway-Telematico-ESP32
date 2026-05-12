#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <Arduino.h>
#include <WiFi.h>

class TcpClient
{
public:

    static void begin();
    static void update();

    static bool connect();
    static bool isConnected();

    static void send(const String& msg);

    static void setServer(const char* host, uint16_t port);

private:

    static WiFiClient client;

    static const char* serverIP;
    static uint16_t serverPort;

    static bool connected;
};

#endif