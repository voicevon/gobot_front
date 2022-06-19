#include "calculator.h"


// https://stackoverflow.com/questions/9949935/calculate-number-of-bits-set-in-byte

uint8_t Calculator::count_ones (uint8_t byte)
{
  static const uint8_t NIBBLE_LOOKUP [16] =
  {
    0, 1, 1, 2, 1, 2, 2, 3, 
    1, 2, 2, 3, 2, 3, 3, 4
  };

  return NIBBLE_LOOKUP[byte & 0x0F] + NIBBLE_LOOKUP[byte >> 4];
}