#include "EepromIic.h"
#include <i2c_t3.h>
#include <avr/io.h>

// Atmel AT42C32 on ZS-042 has address 0x57 with no jumpers on
const int deviceaAddr = 0x57;

void EepromIic::write_byte (
    unsigned int eeaddress,
    uint8_t data) 
{
    int rdata = data;
    Wire.beginTransmission(deviceaAddr);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.write(rdata);
    Wire.endTransmission();
}

uint8_t EepromIic::read_byte(
    unsigned int eeaddress)
{
    byte rdata = 0xFF;
    Wire.beginTransmission(deviceaAddr);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaAddr, 1);
    if (Wire.available()) rdata = Wire.read();
    return rdata;
}