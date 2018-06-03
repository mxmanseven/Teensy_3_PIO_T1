#ifndef BUTTONS_H
#define BUTTONS_H


#include <avr/io.h>

class ButtonInfo
{   
    public:
    uint32_t lastReadTime;
    bool down;
    bool pressedShort;
    bool pressedLong;
    uint8_t pin;
}; 

class Buttons
{
    public:
    Buttons();
    static const uint8_t PIN_COUNT = 4;
    static ButtonInfo pins[PIN_COUNT];

    int ReadButtons();


};

#endif