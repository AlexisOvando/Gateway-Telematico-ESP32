#include <Arduino.h>
#include "esp32-hal-timer.h"

int ledPin = 2; // Pin del LED integrado en la mayoría de las placas ESP32




void setup() {
  // put your setup code here, to run once:
pinMode(ledPin, OUTPUT); // Configura el pin del LED como salida
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(ledPin, HIGH); // Enciende el LED
  delay(100); // Espera 1 segundo      
  digitalWrite(ledPin, LOW); // Apaga el LED
  delay(100); // Espera 1 segundo  
}

