#include "SerialService.h"
#include "Stream.h"
#include "Events.h"
#include "Globals.h"
#include <string>

uint8_t buffRx[100]; // bytes crudos del UART
char buffRxLoc[100]; // buffer local para validaciones

volatile int SerialService::ste = CONFIG;
volatile int SerialService::idx = 0;

void SerialService::begin(unsigned long baudRate)
{
    Serial2.begin(baudRate, SERIAL_8N1, RX_PIN, TX_PIN);
}

int SerialService::update()
{

    if (Serial2.available() > 0)
    {

        int  byte = Serial2.read();
        if (byte == '\n')  // Ya procesa el comando delimitador
        {
            buffRx[idx] = '\0';
            Receive(buffRx, idx); // el idx es la cantidad de BYTES reales que llegaron
            idx = 0;
            return 1;
        }
        else
        {
            if(idx>sizeof(buffRx)-1){
                idx = 0;
                return -1;
            } //OVERFLOW
            buffRx[idx++] = (uint8_t)byte;
        }
    }
    return 0;
}

int SerialService::Receive(uint8_t *data, size_t length)
{
 AppEvent event;

    event.type = EVENT_UART_RX;

    event.length = length;

    memcpy(event.data, data, length);

    if (xQueueSend(g_eventQueue, &event, 0) != pdPASS)
    {
        return -1;
    }

    return 1;
}
