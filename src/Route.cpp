#include "Route.h"

Route::Route()
{
  // knh todo - load routes from flash
  // knh todo - load possiables form flash
}

int8_t Route::addSpeedEntry(uint16_t startTenthMile, uint8_t speed)
{
  // create new route entry, can we create it right in flash? 
  // then set the array value to point to it?
  // not really, we can read and write one byte at a time to / from flash by address

  // create on stack,
  // copy byte by byte to flash.
  // use Route Entry to Bytes
  // returns byte array and length
  // read flash for x bytes and convert to Route Entry with goofy constructor that take a byte array


  // need flash manager to know next open memory location?? maybe

  return 0;  
}

