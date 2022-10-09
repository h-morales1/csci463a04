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

#ifndef MEMORY_H
#define MEMORY_H

#include "hex.h"
#include <algorithm>
#include <cstdint>
#include <vector>

class memory : public hex {
public:
  /**
   * Create a vector of the size of the param but
   * round the length up(mod-16). Then set all elements
   * to 0xa5.
   *
   * @brief create a vector of param size.
   *
   * @param s size of memory.
   *
   ********************************************************************************/
  memory(uint32_t s);
  ~memory();
  /**
   * Returns true if the address is not present inside the mem
   * vector. If the address is not within the range of valid addresses
   * then a warning message is printed to stdout and true is returned.
   *
   * @brief Check whether the memory address is legal or illegal.
   *
   * @param addr memory address to validate.
   *
   * @return is the memory address processed, legal or illegal.
   *
   ********************************************************************************/
  bool check_illegal(uint32_t addr) const;
  /**
   * Returns the rounded up number of bytes in
   * the mem vector.
   *
   * @brief return the size of the mem vector.
   *
   * @return size of the vector(rounded up).
   *
   ********************************************************************************/
  uint32_t get_size() const;
  /**
   * Validate address param, if the address is valid
   * then the byte at the given address is returned,
   * otherwise 0x0 is returned.
   *
   * @brief return byte from address in memory.
   *
   * @param addr address to retrieve byte from.
   *
   * @return byte from the address in memory.
   *
   ********************************************************************************/
  uint8_t get8(uint32_t addr) const;
  /**
   * Use get8() to retrieve two byte from the given
   * address. Once both bytes are retrieved from memory,
   * they are combined and returned.
   *
   * @brief return two bytes from address in memory.
   *
   * @param addr address to retrieve bytes from.
   *
   * @return Two bytes from the address in memory.
   *
   ********************************************************************************/
  uint16_t get16(uint32_t addr) const;
  /**
   * Use get16() to retrieve 4 byte from the given
   * address. Once both bytes are retrieved from memory,
   * they are combined in little-endian order and returned.
   *
   * @brief return 4 bytes from address in memory.
   *
   * @param addr address to retrieve bytes from.
   *
   * @return Four bytes from the address in memory.
   *
   ********************************************************************************/
  uint32_t get32(uint32_t addr) const;
  /**
   * Use get8() to retrieve a byte from memory
   * at the given address, then convert the byte
   * from unsigned into a signed value and return
   * as a 32bit signed value.
   *
   * @brief return a signed byte from address in memory.
   *
   * @param addr address to retrieve byte from.
   *
   * @return signed byte of memory.
   *
   ********************************************************************************/
  int32_t get8_sx(uint32_t addr) const;
  /**
   * Use get8() to retrieve a two bytes from memory
   * at the given address, then combine the two bytes
   * into a 32bit signed integer.
   *
   * @brief return signed bytes from address in memory.
   *
   * @param addr address to retrieve bytes from.
   *
   * @return signed bytes of memory.
   *
   ********************************************************************************/
  int32_t get16_sx(uint32_t addr) const;
  /**
   * Use get16() to retrieve a 4 bytes from memory
   * at the given address, then combine the 4 bytes
   * into a 32bit signed integer.
   *
   * @brief return signed bytes from address in memory.
   *
   * @param addr address to retrieve bytes from.
   *
   * @return signed bytes of memory.
   *
   ********************************************************************************/
  int32_t get32_sx(uint32_t addr) const;
  /**
   * First validate address using check_illegal(),
   * if the address is valid then set the byte in
   * the mem vector to the value passed in. If the
   * address is not valid then discard the data and return.
   *
   * @brief Change byte at address to val.
   *
   * @param addr address to of byte to modify.
   * @param val value to set byte at address to.
   *
   ********************************************************************************/
  void set8(uint32_t addr, uint8_t val);
  /**
   * Use set8() to attempt to modify two bytes in
   * memory at the given address. Separate the value
   * passed in into single bytes, lsb is sent to the
   * lowest address while the msb is sent to addr+1.
   *
   * @brief Change bytes at address to val.
   *
   * @param addr address to of bytes to modify.
   * @param val value to set bytes at address to.
   *
   ********************************************************************************/
  void set16(uint32_t addr, uint16_t val);
  /**
   * Use set16() to attempt to modify 4 bytes in
   * memory at the given address. Separate the value
   * into two groups of two bytes, lsb is sent to the lowest address
   * and msb is sent to addr+2.
   *
   * @brief Change bytes at address to val.
   *
   * @param addr address to of bytes to modify.
   * @param val value to set bytes at address to.
   *
   ********************************************************************************/
  void set32(uint32_t addr, uint32_t val);
  /**
   * Use get8() to and to_hex8() in a loop in order
   * to print the hex equivalent of the bytes at the
   * address of start up until the address at end.
   *
   * @brief Print only a range of bytes from memory vector.
   *
   * @param start address to start printing from.
   * @param end address to stop printing at.
   *
   ********************************************************************************/
  void print_mem_range(uint32_t start, uint32_t end) const;
  /**
   * Use get8() to and isprint() in a loop in order
   * to print the ascii equivalent of the bytes at the
   * address of start up until the address at end.
   *
   * @brief Print only a range of bytes from memory vector.
   *
   * @param start address to start printing from.
   * @param end address to stop printing at.
   *
   ********************************************************************************/
  void print_mem_ascii(uint32_t start, uint32_t end) const;
  /**
   * Loop through the entirety of the mem vector and
   * print the address every 15 elements, after the address
   * print_mem_range() is used to print 16 elements, then
   * print_mem_ascii() is used to print 16 elements, the
   * counter keeping track of when to print the address is
   * reset. This is done until the end of the memory vector is reached.
   *
   * @brief Print the contents of the memory vector.
   *
   ********************************************************************************/
  void dump() const;
  /**
   * Attempt to open file, if an error occurs then
   * an error is displayed and false is returned.
   * Otherwise the function iterates through the
   * contents of the file, using check_illegal()
   * to validate that it is possible to copy the
   * byte to the next address, if the address is not
   * valid then an error is displayed and false is returned.
   * Otherwise the entirety of the file is loaded into the
   * memory vector and true is returned.
   *
   * @brief Attempt to load file into mem vector.
   *
   * @param fname file name.
   *
   * @result if the file was able to be loaded or not.
   *
   ********************************************************************************/
  bool load_file(const std ::string &fname);

private:
  std ::vector<uint8_t> mem;
};
#endif
