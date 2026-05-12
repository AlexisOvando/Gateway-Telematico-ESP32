#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#define MAXDISCONNECTIONS 2
#define WIFI_MSG 51

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <esp_wifi.h>
#include <esp_event.h>
#include <HTTPClient.h>

class WiFiManager
{
public:
  typedef void (*WiFiCallback)(int evt, const String &message);

  static void setWiFiCallback(WiFiCallback callback);
  static void begin();
  static void loop();

  static void WiFiEvent(WiFiEvent_t event);
  static void disconnect();

  static int getStatusWifi();

  static bool isConnected();

  static bool isLocalServer();

  // --- Reemplaza esta función completa ---
  static const char *toString(int _status);

  static const char *eventToString(int event);

  static const char *getHostname();

  static const char *getSSID(int index = 1);

  static const char *getPassword(int index = 1);

  static void setSSID(const char *_ssid, int index = 0);

  static void setPassword(const char *_password, int index = 0);
  static String getMac();

  static String getAPIP();

  static int getSSIDNumber();
  static void setDebug(int debug_type = 0);
  static bool searchSSID(const char *_ssid);

  static bool initCredentials();

  static bool activateStation();

  static void restartRetries();

  static void changeSSID();

  static void connect();

  static bool activateAccessPoint();

  static void handleTickerToRetry();

private:
  static int debug;
  static String ssid;
  static String password;
  static bool isWifi;
  static int statusWifi;
  static WiFiCallback wifiCallback;
  static int ssidNumber;
  static IPAddress softAPIP;
  static int event_1;
  static int eventId;
  static bool isEvent;
  static int countDisConnections;
  static const char *passSoftAP;
  static int retryConnections;
  static long timeToRetry;
  static bool isTimeToRetry;
};

#endif
