
#include "memory.h"
#include <cstddef>
#include <cstdint>
#include <fstream>

memory::memory(uint32_t siz) {
  siz = (siz + 15) & 0xfffffff0; // round the length up, mod-16

  mem.assign(siz, 0xa5);
}

memory::~memory() {
  // TODO clean up
}

bool memory::check_illegal(uint32_t addr) const {
  // check if addr is within range
  if (addr > get_size()) {
    std::cout << "ERROR: OUT OF RANGE MEMORY" << std::endl;
    return true;
  }
  return false;
}

uint32_t memory::get_size() const { return ((mem.size() + 15) & 0xfffffff0); }

uint8_t memory::get8(uint32_t addr) const {
  if (check_illegal(addr)) {
    return 0x00;
  }
  return mem.at(addr);
}

uint16_t memory::get16(uint32_t addr) const {
  uint8_t msb = get8(addr + 1); // addr +1
  uint8_t lsb = get8(addr);     // just addr

  return ((lsb << 8) | (msb));
}

uint32_t memory::get32(uint32_t addr) const {
  uint16_t msb = get16(addr + 2);
  uint16_t lsb = get16(addr);

  return ((lsb << 8) | (msb));
}

int32_t memory::get8_sx(uint32_t addr) const {
  // TODO
  std::cout << addr;
  return 0x00;
}

int32_t memory::get16_sx(uint32_t addr) const {
  // TODO
  std::cout << addr;
  return 0x00;
}

int32_t memory::get32_sx(uint32_t addr) const {
  // TODO
  std::cout << addr;
  return 0x00;
}

void memory::set8(uint32_t addr, uint8_t val) {
  if (check_illegal(addr)) {
    // do nothing
  } else {
    // set byte to val
    mem.at(addr) = val;
  }
}

void memory::set16(uint32_t addr, uint16_t val) {
  uint8_t lsb = (val & 0xFF);
  uint8_t msb = ((val & 0xFF00) >> 8);
  set8(addr, lsb);
  set8(addr + 1, msb);
}

void memory::set32(uint32_t addr, uint32_t val) {
  uint16_t lsb = (val & 0xFFFF);
  uint16_t msb = ((val & 0xFF00) >> 16);
  set16(addr, lsb);
  set16(addr + 2, msb);
}

void memory::dump() const {
  int count = 0;
  for (std::size_t i = 0; i < mem.size(); i++) {
    if (i % 16 == 0) {
      // every 15 bytes
      if (i != 0) {
        std::cout << std::endl;
      }
      std::cout << to_hex32(i) << ":";
      if (i == 0)
        std::cout << to_hex8(get8(i)) << " ";
    } else {
      // print elements
      std::cout << to_hex8(get8(i)) << " ";
      count++;
      if (count == 16) {
        // std::cout << "test";
        count = 0;
      }
    }
  }
  std::cout << std::endl;
}

bool memory::load_file(const std::string &fname) {
  //
  uint8_t i;
  std::ifstream infile(fname, std::ios::in | std::ios::binary);
  if (!infile) {
    std::cerr << "ERROR OPENING FILE";
    return false;
  }

  infile >> std::noskipws;
  for (uint32_t addr = 0; infile >> i; ++addr) {
    if (check_illegal(addr)) {
      std::cerr << "Program too big.";
      return false;
    }
    mem.at(addr) = i;
  }
  return true;
}
