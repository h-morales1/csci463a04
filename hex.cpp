//***************************************************************************
//
//  Herbert Morales
//  Z1959955
//  CSCI-463-MSTR
//
//  I certify that this is my own work and where appropriate an extension
//  of the starter code provided for the assignment.
//
//***************************************************************************

#include "hex.h"
#include <cstdint>

std::string hex::to_hex8(uint8_t i) {
  std::ostringstream os;
  os << std::hex << std::setfill('0') << std::setw(2)
     << static_cast<uint16_t>(i);

  return os.str();
}

std::string hex::to_hex32(uint32_t i) {
  std::ostringstream os;
  os << std::hex << std::setfill('0') << std::setw(8) << i;

  return os.str();
}

std::string hex::to_hex0x12(uint32_t i) {
  std::ostringstream os;
  i = (i & 0x00000fff);

  os << "0x" << std::hex << std::setfill('0') << std::setw(3) << i;

  return os.str();
}

std::string hex::to_hex0x20(uint32_t i) {
  std::ostringstream os;

  os << "0x" << std::hex << std::setfill('0') << std::setw(5) << i;

  return os.str();
}

std::string hex::to_hex0x32(uint32_t i) {
  return std::string("0x") + to_hex32(i);
}
