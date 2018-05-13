#include "EepromIic.h"
#include <i2c_t3.h>
#include <avr/io.h>

// Atmel AT42C32 on ZS-042 has address 0x57 with no jumpers on
const int deviceaAddr = 0x57;

void EepromIic::write_byte (
    unsigned int eeaddress,
    uint8_t data) 
{    
    uint8_t  highAddressByte  = (uint8_t) (eeaddress >> 8);  // byte with the four MSBits of the address
    uint8_t  lowAddressByte   = (uint8_t) (eeaddress & 0xFF); // byte with the eight LSbits of the address
    
    // Serial.printf("EepromIic::write_byte eeaddress: %d 0x%x 0x%x 0x%x\n", 
    //     eeaddress, eeaddress, highAddressByte, lowAddressByte);

    int rdata = data;
    Wire.beginTransmission(deviceaAddr);
    Wire.write((int)highAddressByte); // MSB
    Wire.write((int)lowAddressByte); // LSB
    Wire.write(rdata);
    delay(10);                                // maximum write cycle time per data sheet
    Wire.endTransmission();
    delay(10);

    int error = Wire.getError();
    if(error)
    {
        Serial.println(
            "EepromIic::write_byte error: " 
            + String(error));
    }
}

uint8_t EepromIic::read_byte(
    unsigned int eeaddress)
{
    uint8_t  highAddressByte  = (uint8_t) (eeaddress >> 8);  // byte with the four MSBits of the address
    uint8_t  lowAddressByte   = (uint8_t) (eeaddress & 0xFF); // byte with the eight LSbits of the address

    // Serial.printf("EepromIic::read_byte eeaddress: %d 0x%x 0x%x 0x%x\n", 
    //     eeaddress, eeaddress, highAddressByte, lowAddressByte);
 
    byte rdata = 0xFF;
    Wire.beginTransmission(deviceaAddr);
    Wire.write((int)highAddressByte); // MSB
    Wire.write((int)lowAddressByte); // LSB
    // Send the Tx buffer, but send a restart to keep connection alive
    Wire.endTransmission(false);
    Wire.requestFrom(deviceaAddr, 1);
    delay(10);                                       // maximum write cycle time per data sheet
    if (Wire.available()) rdata = Wire.read();

    delay(10);

    int error = Wire.getError();
    if(error)
    {
        Serial.println(
            "EepromIic::read_byte error: " 
            + String(error));
    }

    return rdata;
}