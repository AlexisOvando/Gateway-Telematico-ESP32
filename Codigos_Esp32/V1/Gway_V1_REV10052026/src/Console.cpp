#include "Console.h"

int Console::debug = 0;

// Inicializa UART DEBUG con los pines por defecto
void Console::start(unsigned long baud)
{
    Serial.begin(baud, SERIAL_8N1);
}

void Console::write(const char *format, ...)
{
    if (debug == 0)
        return;
    char buffer[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    Serial.print(buffer);
}

void Console::writeln(const char *format, ...)
{
    if (debug == 0)
        return;
    char buffer[128];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    Serial.println(buffer);
}

void Console::write(const String &texto)
{
    if (debug == 0)
        return;
    Serial.print(texto);
}

void Console::writeln(const String &texto)
{
    if (debug == 0)
        return;
    Serial.println(texto);
}

void Console::setDebug(bool debugLevel)
{
    debug = debugLevel;
}