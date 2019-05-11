#ifndef YUNMESSENGER_H
#define YUNMESSENGER_H

#include <Bridge.h>
#include <Console.h>

#include "Arduino.h"

class Messenger : public ConsoleClass{
    public:
        Messenger();
        static void send(char *subscriber, char *message);
        static void open(char *subscriber);
        static void close();
};

#endif