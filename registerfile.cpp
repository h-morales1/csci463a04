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
#include "registerfile.h"

registerfile::registerfile() { reset(); }

void registerfile::reset() {
  registers.assign(32, 0xF0F0F0F0);
  registers.at(0) = 0;
}

void registerfile::set(uint32_t r, int32_t val) {
  if (r == 0)
    return;

  registers.at(r) = val;
}

int32_t registerfile::get(uint32_t r) const {
  if (r == 0)
    return 0;

  return registers[r];
}

void registerfile::print_mem_range(uint32_t start, uint32_t end) const {
  int counter = 0;
  for (; start < end; start++) {
    if (start == (end - 1)) {
      // its the end of the line
      std::cout << hex::to_hex32(get(start));
    } else {
      // its not the end of the line, needs space
      std::cout << hex::to_hex32(get(start)) << " ";
    }
    counter++;
    if (counter == 4) {
      std::cout << " ";
    }
  }
}

void registerfile::dump(const std::string &hdr) const {
  int count = 7;
  for (size_t i = 0; i < registers.size(); i++) {
    if (count == 7) {
      if (hdr.length() != 0) { // you have a hdr
        std::cout << hdr << " ";
        std::cout << 'x' << std::left << std::setw(3) << i;
        print_mem_range(i, i + 8);
        std::cout << std::endl;
        count = 0;
      } else { // no hdr
        std::ostringstream ss;
        ss << "x" << i;
        std::cout << std::right << std::setw(3) << ss.str() << " ";
        print_mem_range(i, i + 8);
        std::cout << std::endl;
        count = 0;
      }

    } else {
      count++;
    }
  }
}
