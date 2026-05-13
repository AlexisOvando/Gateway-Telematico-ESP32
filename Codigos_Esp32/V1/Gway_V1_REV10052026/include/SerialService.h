#ifndef SERIALSERVICE_H
#define SERIALSERVICE_H 

/*
    El sistema debe ser capaz de recibir mensajes por UART, 
    procesarlos, y mandarlso en Queue al servidor TCP, 
    Para ello, este sistema de uart debe funcionar por EVENTOS
*/



#include <Arduino.h>
#include "Console.h"

#define RX_PIN 16
#define TX_PIN 17


enum SerialSte{
    CONFIG, 
    Rx_Event, 
    Tx_Process,
    LOOP
};


class SerialService
{
private:
    /* data */
    static volatile int ste;
    static volatile int idx;
public:

    static void begin(unsigned long baudRate); 
    static int update();


    static int Receive(uint8_t *data, size_t length); // Si se detecta un dato en el buffer uart, se debe disparar esta funcion

    





};




#endif