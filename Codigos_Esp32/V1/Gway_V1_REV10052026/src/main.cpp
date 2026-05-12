#include <Arduino.h>
#include <WiFi.h>
#include "TcpClient.h"
#include "WiFiManager.h"
#include "Console.h"

#define LED_RED 25
#define LED_GREEN 26
#define LED_BLUE 27



/*
=========================================================
                APPLICATION STATES
=========================================================
*/

enum AppState
{
    APP_SETUP = 0,
    APP_START,
    APP_RUN
};

volatile AppState appState = APP_SETUP;

/*
=========================================================
                WIFI CREDENTIALS
=========================================================
*/

const char *ssid     = "R E D";
const char *password = "ferrariyhamilton44";


/*
=========================================================
                    TASK HANDLES
=========================================================
*/

TaskHandle_t TaskNetworkHandle = NULL;
TaskHandle_t TaskAppHandle     = NULL;
TaskHandle_t TaskLoggerHandle  = NULL;

/*
=========================================================
                FUNCTION PROTOTYPES
=========================================================
*/

void TaskNetwork(void *pvParameters);
void TaskApplication(void *pvParameters);
void TaskLogger(void *pvParameters);

void initializeHardware();
void initializeServices();



void setup()
{

    Console::setDebug(true);
    Console::start(115200);

    Console::writeln("\n==============================");
    Console::writeln("BOOTING SYSTEM...");
    Console::writeln("==============================");

    /*
    =========================================================
                    NETWORK TASK
                    CORE 0
    =========================================================
    */

    xTaskCreatePinnedToCore(TaskNetwork,"TaskNetwork",10000,NULL,2,&TaskNetworkHandle,0);

    /*
    =========================================================
                    APPLICATION TASK
                    CORE 1
    =========================================================
    */

    xTaskCreatePinnedToCore(TaskApplication,"TaskApplication",10000,NULL,3,&TaskAppHandle,1);

    /*
    =========================================================
                    LOGGER TASK
                    CORE 1
    =========================================================
    */

    xTaskCreatePinnedToCore(TaskLogger,"TaskLogger",4000,NULL,1,&TaskLoggerHandle,1);
}

/*
=========================================================
                        LOOP
=========================================================
*/

void loop()
{
    /*
        Empty.
        FreeRTOS handles everything.
    */
}

/*
=========================================================
                NETWORK TASK
                CORE 0
=========================================================
*/

void TaskNetwork(void *pvParameters)
{
    Console::writeln("NET> Task started on Core 0");

    /*
        WiFi Configuration
    */
    WiFiManager::setSSID(ssid);
    WiFiManager::setPassword(password);
    TcpClient::setServer("10.175.140.169",9876);

    /*
        Start WiFi
    */
    WiFiManager::begin();

    bool tcpStarted = false;   

    for (;;)
    {
        /*
        ==========================================
                WIFI MANAGEMENT
        ==========================================
        */
        WiFiManager::loop();

        if (WiFiManager::isConnected())
        {
          Console::writeln("NET> WiFi Connected");
          Console::write("NET> IP: ");
                Console::writeln(WiFi.localIP().toString());

            if (!tcpStarted)
            {
                Console::writeln("NET> Starting TCP Client...");
                TcpClient::connect();
                tcpStarted = true;
            }
            TcpClient::update();
            if(TcpClient::isConnected())
            {
                Console::writeln("NET> TCP Connected");
            }
            else
            {
                Console::writeln("NET> TCP Disconnected");
                tcpStarted = false;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

/*
=========================================================
                APPLICATION TASK
                CORE 1
=========================================================
*/

void TaskApplication(void *pvParameters)
{
    Console::writeln("APP> Task started on Core 1");

    for (;;)
    {
        switch (appState)
        {

        case APP_SETUP:

            Console::writeln("APP> STATE_SETUP");
            initializeHardware();
            initializeServices();

            appState = APP_START;
            break;

        case APP_START:

            Console::writeln("APP> STATE_START");
            appState = APP_RUN;

            break;

        case APP_RUN:
        if (TcpClient::isConnected())
    {
        TcpClient::send("ESP32 MESSAGE");
    }
            digitalWrite(LED_RED, !digitalRead(LED_RED));
            vTaskDelay(pdMS_TO_TICKS(1000));

            break;

        default:
            break;
        }

        /*
            Yield CPU
        */
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

/*
=========================================================
                LOGGER TASK
=========================================================
*/

void TaskLogger(void *pvParameters)
{
    Console::writeln("LOG> Logger task started");

    for (;;)
    {
        /*
            Future:
            - Save logs
            - SD card
            - Remote debug
            - Ring buffers
        */
       

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

/*
=========================================================
            HARDWARE INITIALIZATION
=========================================================
*/

void initializeHardware()
{
    Console::writeln("HW> Initializing GPIOs");

    pinMode(LED_RED, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
    vTaskDelay(pdMS_TO_TICKS(100));

    digitalWrite(LED_RED,HIGH);
    digitalWrite(LED_GREEN,HIGH);
    digitalWrite(LED_BLUE, HIGH);
    vTaskDelay  (pdMS_TO_TICKS(1000)); 
}

/*
=========================================================
            SERVICES INITIALIZATION
=========================================================
*/

void initializeServices()
{
    Console::writeln("SRV> Initializing services");
}