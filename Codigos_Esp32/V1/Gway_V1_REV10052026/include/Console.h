
#ifndef CONSOLE_H
#define CONSOLE_H

#include <Arduino.h>

class Console {
private:
  static int debug;
  

public:
  // Inicializa UART DEBUG con los pines por defecto
  static void start(unsigned long baud = 115200);

  static void write(const char* format, ...);

  static void writeln(const char* format, ...);

  static void write(const String& texto);

  static void writeln(const String& texto);

  static void setDebug(bool debugLevel);
};


#endif  // CONSOLE_H*/
