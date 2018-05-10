#include "EepromIic.h"
#include <i2c_t3.h>
#include <avr/io.h>

// Atmel AT42C32 on ZS-042 has address 0x57 with no jumpers on
const int deviceaAddr = 0x57;

void EepromIic::write_byte (
    unsigned int eeaddress,
    uint8_t data) 
{


    
    // Construct EEPROM address from page and entry input
    // There are 128 pages and 32 entries (bytes) per page
    // EEPROM address are 16-bit (2 byte) address where the MS four bits are zero (or don't care)
    // the next seven MS bits are the page and the last five LS bits are the entry location on the page
    int page = 1;
    uint16_t pageEntryAddress = (uint16_t) ((uint16_t) page << 5) | eeaddress;
    uint8_t  highAddressByte  = (uint8_t) (pageEntryAddress >> 8);  // byte with the four MSBits of the address
    uint8_t  lowAddressByte   = (uint8_t) (pageEntryAddress - ((uint16_t) highAddressByte << 8)); // byte with the eight LSbits of the address
    
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

    int page = 1;
    uint16_t pageEntryAddress = (uint16_t) ((uint16_t) page << 5) | eeaddress;
    uint8_t  highAddressByte  = (uint8_t) (pageEntryAddress >> 8);  // byte with the four MSBits of the address
    uint8_t  lowAddressByte   = (uint8_t) (pageEntryAddress - ((uint16_t) highAddressByte << 8)); // byte with the eight LSbits of the address
    

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