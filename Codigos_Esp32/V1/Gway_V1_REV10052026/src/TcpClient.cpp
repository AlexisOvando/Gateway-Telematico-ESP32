#include "TcpClient.h"
#include "Console.h"

WiFiClient TcpClient::client;

const char* TcpClient::serverIP = "192.168.1.100";
uint16_t TcpClient::serverPort = 5000;

bool TcpClient::connected = false;

/*
=========================================================
                    CONFIG SERVER
=========================================================
*/

void TcpClient::setServer(const char* host, uint16_t port)
{
    serverIP = host;
    serverPort = port;
}

/*
=========================================================
                    BEGIN
=========================================================
*/

void TcpClient::begin()
{
    connect();
}

/*
=========================================================
                    CONNECT
=========================================================
*/

bool TcpClient::connect()
{
    if (client.connected())
    {
        connected = true;
        return true;
    }

    Console::writeln("TCP> Connecting...");

    connected = client.connect(serverIP, serverPort);

    if (connected)
    {
        Console::writeln("TCP> Connected!");
    }
    else
    {
        Console::writeln("TCP> Connection failed");
    }

    return connected;
}

/*
=========================================================
                    UPDATE
=========================================================
*/

void TcpClient::update()
{
    if (!client.connected())
    {
        connected = false;
        return;
    }

    while (client.available())
    {
        String rx = client.readStringUntil('\n');

        Console::writeln("TCP RX> " + rx);
    }
}

/*
=========================================================
                    SEND
=========================================================
*/

void TcpClient::send(const String& msg)
{
    if (!client.connected())
        return;

    client.println(msg);

    Console::writeln("TCP TX> " + msg);
}

/*
=========================================================
                    STATUS
=========================================================
*/

bool TcpClient::isConnected()
{
    return client.connected();
}