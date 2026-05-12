  #include "WiFiManager.h"
  #include "Console.h"

  // Static members
  String WiFiManager::ssid = "";
  String WiFiManager::password = "";
  const char *WiFiManager::passSoftAP = "password123";
  int WiFiManager::event_1 = -1;
  int WiFiManager::eventId = -1;
  bool WiFiManager::isEvent = false;
  int WiFiManager::countDisConnections = MAXDISCONNECTIONS;
  bool WiFiManager::isWifi = false;
  int WiFiManager::statusWifi = -1;
  int WiFiManager::ssidNumber = 1;
  IPAddress WiFiManager::softAPIP;
  int WiFiManager::debug = 0;
  WiFiManager::WiFiCallback WiFiManager::wifiCallback = nullptr;
  int WiFiManager::retryConnections = 5;
  long WiFiManager::timeToRetry = 0;
  bool WiFiManager::isTimeToRetry = false;

  void WiFiManager::setWiFiCallback(WiFiCallback callback)
  {
    wifiCallback = callback;
  }

  void WiFiManager::begin()
  {
    initCredentials();
    WiFi.mode(WIFI_STA);
    WiFi.onEvent(WiFiEvent);
    if (activateStation())
    {
      if (debug >= 1)
        Console::writeln("WFM> STA connected to: " + String(ssid));
    }
    else
    {
      if (debug >= 1)
        Console::writeln("WFM> STA Connecting...");
    }

    if (debug >= 1)
    {
      Console::write("WFM> Hostname: ");
      Console::writeln(WiFi.getHostname());
      Console::write("WFM> MAC: ");
      Console::writeln(getMac());
    }
  }

  void WiFiManager::loop()
  {
    if (isEvent)
    {
      isEvent = false;

      if (event_1 == eventId)
      {
        if (eventId == ARDUINO_EVENT_WIFI_STA_DISCONNECTED)
          changeSSID();
        return;
      }

      event_1 = eventId;

      switch (eventId)
      {
      case ARDUINO_EVENT_WIFI_STA_CONNECTED:
        if (WiFi.getMode() != WIFI_STA)
        {
          if (debug >= 2)
          Console::writeln("WFM> SoftAP disconnect!");
          WiFi.mode(WIFI_STA); //??? porque dos veces
        }
        if (debug >= 1)
        Console::writeln((String)WiFi.localIP());
        if (wifiCallback)
          wifiCallback(eventId, "");
        break;

      case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        changeSSID();
        if (wifiCallback)
          wifiCallback(eventId, "");
        break;

      case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        if (debug >= 1)
        Console::writeln((String)WiFi.localIP());
        if (wifiCallback)
          wifiCallback(eventId, "");
        break;

      case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
        if (debug >= 2)
          Console::writeln("WFM> Device connected to SoftAP");
        break;

      case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
        if (debug >= 2)
          Console::writeln("WFM> Device disconnected from SoftAP");
        break;

      default:
        break;
      }

      if (debug >= 2)
      {
      Console::write("WFM> ");
        Console::writeln(eventToString(eventId));
      }
    }
    else
    {
      handleTickerToRetry();
      restartRetries();
    }
  }

  void WiFiManager::WiFiEvent(WiFiEvent_t event)
  {
    isEvent = true;
    eventId = event;
  }

  void WiFiManager::disconnect()
  {
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
  }

  int WiFiManager::getStatusWifi()
  {
    return WiFi.status();
  }

  bool WiFiManager::isConnected()
  {
    return WiFi.status() == WL_CONNECTED;
  }

  bool WiFiManager::isLocalServer()
  {
    // Obtiene la IP del SoftAP
    softAPIP = WiFi.softAPIP();
    // Verifica si la IP no está vacía (0.0.0.0)
    return (softAPIP != 0);
  }

  const char *WiFiManager::toString(int _status)
  {
    switch (_status)
    {
    case WL_CONNECTED:
      return "Connected";
    case WL_NO_SSID_AVAIL:
      return "No SSID avail";
    case WL_CONNECT_FAILED:
      return "Conn failed";
    case WL_DISCONNECTED:
      return "Disconnected";
  #ifdef WL_WRONG_PASSWORD
    case WL_WRONG_PASSWORD:
      return "Wrong password";
  #endif
    default:
      return "Unknown";
    }
  }

  const char *WiFiManager::eventToString(int event)
  {
    switch (event)
    {
    case ARDUINO_EVENT_WIFI_STA_CONNECTED:
      return "STA Connected";
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      return "STA Disconnected";
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      return "STA Got IP";
    case ARDUINO_EVENT_WIFI_AP_START:
      return "SoftAP Start";
    case ARDUINO_EVENT_WIFI_AP_STOP:
      return "SoftAP Stop";
    case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
      return "SoftAP STA Connected";
    case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
      return "SoftAP STA Disconnected";
    default:
      return "Unknown Event";
    }
  }

  const char *WiFiManager::getHostname()
  {
    return WiFi.getHostname();
  }

  const char *WiFiManager::getSSID(int index)
  {
    // if (index == 1) return PropBag::ssid_1.c_str();
    // if (index == 2) return PropBag::ssid_2.c_str();
    return ssid.c_str();
  }

  const char *WiFiManager::getPassword(int index)
  {
    // if (index == 1) return PropBag::password_1.c_str();
    // if (index == 2) return PropBag::password_2.c_str();
    return password.c_str();
  }

  void WiFiManager::setSSID(const char *_ssid, int index)
  {
    // if (index == 1) PropBag::ssid_1 = _ssid;
    // else if (index == 2) PropBag::ssid_2 = _ssid;
    ssid = _ssid;
  }

  void WiFiManager::setPassword(const char *_password, int index)
  {
    // if (index == 1) PropBag::password_1 = _password;
    // else if (index == 2) PropBag::password_2 = _password;
    password = _password;
  }

  String WiFiManager::getMac()
  {
    return WiFi.macAddress();
  }

  String WiFiManager::getAPIP()
  {
    softAPIP = WiFi.localIP();
    return softAPIP.toString();
  }

  int WiFiManager::getSSIDNumber()
  {
    // if (ssid == PropBag::ssid_1) return 1;
    // if (ssid == PropBag::ssid_2) return 2;
    return 0;
  }

  void WiFiManager::setDebug(int debug_type)
  {
    debug = debug_type;
  }

  bool WiFiManager::searchSSID(const char *_ssid)
  {
    if (!_ssid || strlen(_ssid) == 0)
      return false;
    int count = WiFi.scanNetworks();
    for (int i = 0; i < count; ++i)
    {
      if (WiFi.SSID(i).equals(_ssid))
      {
        if (debug >= 1)
        Console::writeln("WFM> SSID found!");
        return true;
      }
    }
    if (debug >= 1)
      Console::writeln("WFM> SSID not found");
    return false;
  }

  bool WiFiManager::initCredentials()
  {

    if (debug >= 1)
    Console::writeln("WFM> Credentials are NULL");
    // ssid = "";
    // password = "";
  Console::writeln("WFM> Credentials assigned for: [" + ssid + "]");
    return true;
  }

  bool WiFiManager::activateStation()
  {
    // if (ssid == "" || password == "") return false;
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    return true;
  }

  void WiFiManager::restartRetries()
  {
    if (isTimeToRetry)
    {
      isTimeToRetry = false;
      countDisConnections = MAXDISCONNECTIONS;
      if (initCredentials())
        activateStation();
      else
        activateAccessPoint();
    }
  }

  void WiFiManager::changeSSID()
  {
    if (--countDisConnections > 0)
      return;
    countDisConnections = MAXDISCONNECTIONS;
    /* if ((PropBag::ssid_1 == "" && PropBag::ssid_2 == "") || --retryConnections <= 0) {
      retryConnections = 0;
      activateAccessPoint();
      return;
    }*/
    /*if (ssid == PropBag::ssid_1 && PropBag::ssid_2 != "") {
      ssid = PropBag::ssid_2;
      password = PropBag::password_2;
    } else if (ssid == PropBag::ssid_2 && PropBag::ssid_1 != "") {
      ssid = PropBag::ssid_1;
      password = PropBag::password_1;
    }*/
    if (debug >= 1)
    Console::writeln("WFM> Change to SSID: " + ssid);
    connect();
  }

  void WiFiManager::connect()
  {
    if (ssid == "")
    {
      if (debug >= 1)
      Console::writeln("WFM> SSID is null");
      return;
    }
    if (WiFi.softAPgetStationNum() == 0)
    {
      if (debug >= 1)
        Console::writeln("WFM> Connecting to SSID: " + ssid);
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid.c_str(), password.c_str());
    }
  }

  bool WiFiManager::activateAccessPoint()
  {
    bool activate = false;
    if (passSoftAP != "")
    {
      if (WiFi.getMode() != WIFI_AP)
      {
        activate = WiFi.mode(WIFI_AP);
        WiFi.softAP(WiFi.getHostname(), passSoftAP);
        if (debug >= 2)
          Console::writeln("WFM> Enable SoftAP Mode!");
        retryConnections = 0;
        timeToRetry = millis();
      }
      else
      {
        return !activate;
      }
    }
    return activate;
  }

  void WiFiManager::handleTickerToRetry()
  {
    if (timeToRetry > 0 && (millis() - timeToRetry) > 180000)
    {
      timeToRetry = 0;
      isTimeToRetry = true;
      if (debug >= 3)
      Console::writeln("WFM> Retry Connect!");
    }
  }