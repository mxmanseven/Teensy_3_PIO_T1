#ifndef EEPROM_IIC_H_
#define EEPROM_IIC_H_

#include <avr/io.h>

class EepromIic
{
    public:
        static void write_byte(
            unsigned int eeaddress, 
            uint8_t data);

        static uint8_t read_byte(
            unsigned int eeaddress);
};

    // store wheel size at address 26 and use 4 bytes (32bits)
    const int WHEEL_SIZE_FIRST_ADDRESS = 26;

    // Start storing rout data at address 30
    const int ROUTE_FIRST_ADDRESS = 30;
    const int ROUTE_COUNT_ADDRESS = 29;

#endif
