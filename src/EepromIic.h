#ifndef EEPROM_IIC_H_
#define EEPROM_IIC_H_

#include <avr/io.h>

class EepromIic
{
    public:
        void write_byte(
            unsigned int eeaddress, 
            uint8_t data);

        uint8_t read_byte(
            unsigned int eeaddress);
};

#endif
