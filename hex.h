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

#ifndef HEX_H
#define HEX_H

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <ostream>

class hex {
public:
  /**
   * This function will take in an unsigned number and return its
   * hex equivalent as a string which will be formatted to have
   * leading zeros.
   *
   * @brief convert param into hex and return as string.
   *
   * @param i number to process.
   *
   * @return Formatted string containing hex number.
   *
   ********************************************************************************/
  static std::string to_hex8(uint8_t i);
  /**
   * This function will take in an unsigned number and return its
   * hex equivalent as a string which will be formatted to have
   * leading zeros.
   *
   * @brief convert param into hex and return as string.
   *
   * @param i number to process.
   *
   * @note this is like to_hex8() except for 32-bit.
   *
   * @return Formatted string containing hex number.
   *
   ********************************************************************************/
  static std::string to_hex32(uint32_t i);
  /**
   * This function will take in an unsigned number and return its
   * hex equivalent as a string which will be formatted to have
   * leading zeros and will add on '0x' as a prefix to the string.
   *
   * @brief convert param into hex and return as string.
   *
   * @param i number to process.
   *
   * @return Formatted string containing hex number.
   *
   ********************************************************************************/
  static std::string to_hex0x12(uint32_t i);
  /**
   * This function will take in an unsigned number and return its
   * hex equivalent as a string which will be formatted to have
   * leading zeros and will add on '0x' as a prefix to the string.
   *
   * @brief convert param into hex and return as string.
   *
   * @param i number to process.
   *
   * @return Formatted string containing hex number.
   *
   ********************************************************************************/
  static std::string to_hex0x20(uint32_t i);
  /**
   * This function will take in an unsigned number and return its
   * hex equivalent as a string which will be formatted to have
   * leading zeros and will add on '0x' as a prefix to the string.
   *
   * @brief convert param into hex and return as string.
   *
   * @param i number to process.
   *
   * @note This is like to_hex0x20 but for 32-bit.
   *
   * @return Formatted string containing hex number.
   *
   ********************************************************************************/
  static std::string to_hex0x32(uint32_t i);
};
#endif
