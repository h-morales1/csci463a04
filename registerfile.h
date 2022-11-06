/*
 */
#ifndef REGISTERFILE_H
#define REGISTERFILE_H

#include "hex.h"
#include <vector>

class registerfile {
public:
  /**
   * This will construct an object to hold registers
   * and their values, it uses the reset() function.
   *
   * @brief construct an object to hold registers.
   *
   ********************************************************************************/
  registerfile();
  /**
   * This function will set all registers to 0xF0F0F0F0
   * and x0 will be set to 0.
   *
   * @brief reset all the registers.
   *
   ********************************************************************************/
  void reset();
  /**
   * This function will check to see if the register
   * is 0x, if so then nothing is done, otherwise
   * the value is placed in the r register.
   *
   * @brief set a register to a value.
   *
   * @param r which register to set.
   * @param val value to set register to.
   *
   ********************************************************************************/
  void set(uint32_t r, int32_t val);
  /**
   * This function will check if the register r is
   * x0, if it is then 0 is returned, otherwise
   * the value in register r is returned.
   *
   * @brief get a value from register r.
   *
   * @param r register to get value from.
   *
   * @return value stored in register.
   *
   ********************************************************************************/
  int32_t get(uint32_t r) const;
  /**
   * This function will take in a starting index
   * and loop until reaching the end index. Inside the loop
   * the start index is used as param for the get() function
   * to print out values of registers. Inside the loop
   * there are checks to see if it is time to place a space
   * or a double space, as well as making sure not to place
   * a space on the ending register value of each line.
   *
   * @brief print formatted range of values.
   *
   * @param start index to begin at.
   * @param end index to stop at.
   *
   ********************************************************************************/
  void print_mem_range(uint32_t start, uint32_t end) const;
  /**
   * This function will loop through the vector containing
   * all the register values, each index being a register.
   * it will check to see if a hdr was passed in, if a hdr
   * was passed in then it will print it before printing
   * the line of register values in a range i+8. This is
   * achieved by calling print_mem_range() to print each
   * line of register values. If no hdr is detected then
   * the same process is used except no hdr is printed.
   *
   * @brief dump the contents of registers.
   *
   * @param hdr prefix to print in dump lines.
   *
   ********************************************************************************/
  void dump(const std::string &hdr) const;

private:
  std::vector<int32_t> registers;
};
#endif
