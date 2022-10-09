#ifndef HEX_H
#define HEX_H

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <ostream>

class hex {
public:
  static std::string to_hex8(uint8_t i);
  static std::string to_hex32(uint32_t i);
  static std::string to_hex0x20(uint32_t i);
  static std::string to_hex0x32(uint32_t i);
};
#endif
