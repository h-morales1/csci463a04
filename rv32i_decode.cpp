#include "rv32i_decode.h"
#include <assert.h>
#include <cstdint>

std::string rv32i_decode::decode(uint32_t addr, uint32_t insn) {
  //
  switch (get_opcode(insn)) {
  default:
    return render_illegal_insn(insn);
  }
  assert(0 && "unrecognized op, shouldnt be here"); // it should be impossible
                                                    // to ever get here
}

uint32_t rv32i_decode::get_opcode(uint32_t insn) { return (insn & 0x0000007f); }
