#include "rv32i_decode.h"
#include <assert.h>
#include <cstdint>

std::string rv32i_decode::decode(uint32_t addr, uint32_t insn) {
  //
  switch (get_opcode(insn)) {
  default:
    return render_illegal_insn(insn);
  case opcode_lui:
    return render_lui(insn);
  case opcode_auipc:
    return render_auipc(insn);
  case opcode_rtype:
    switch (get_funct3(insn)) {
    default:
      return render_illegal_insn(insn);
    case funct3_sll:
      return render_rtype(insn, "sll");
    case funct3_slt:
      return render_rtype(insn, "slt");
    case funct3_sltu:
      return render_rtype(insn, "sltu");
    case funct3_xor:
      return render_rtype(insn, "xor");
    case funct3_or:
      return render_rtype(insn, "or");
    case funct3_and:
      return render_rtype(insn, "and");
    case funct3_srx:
      switch (get_funct7(insn)) {
      default:
        return render_illegal_insn(insn);
      case funct7_srl:
        return render_rtype(insn, "srl");
      case funct7_sra:
        return render_rtype(insn, "sra");
      }
    case funct3_add:
      switch (get_funct7(insn)) {
      default:
        return render_illegal_insn(insn);
      case funct7_add:
        return render_rtype(insn, "add");
      case funct7_sub:
        return render_rtype(insn, "sub");
      }
    }
  }
  assert(0 && "unrecognized op, shouldnt be here"); // it should be impossible
                                                    // to ever get here
}

uint32_t rv32i_decode::get_opcode(uint32_t insn) { return (insn & 0x0000007f); }

uint32_t rv32i_decode::get_rd(uint32_t insn) {
  uint32_t rd = (insn & 0x00000FFF) >> 7;

  return rd;
}

uint32_t rv32i_decode::get_funct3(uint32_t insn) {
  uint32_t funct3 = (insn & 0x00007000) >> 12;

  return funct3;
}

uint32_t rv32i_decode::get_funct7(uint32_t insn) {
  uint32_t funct7 = (insn & 0xFE000000) >> 25;

  return funct7;
}

uint32_t rv32i_decode::get_rs1(uint32_t insn) {
  uint32_t rs1 = (insn & 0x000F8000) >> 15;
  return rs1;
}

uint32_t rv32i_decode::get_rs2(uint32_t insn) {
  uint32_t rs2 = (insn & 0x01F00000) >> 20;

  return rs2;
}

// get imm_x funcs
int32_t rv32i_decode::get_imm_u(uint32_t insn) {
  int32_t imm_u = (insn & 0xFFFFF000);
  return imm_u;
}

int32_t rv32i_decode::get_imm_s(uint32_t insn) {
  int32_t imm_s = (insn & 0xFE000000) >> (25 - 5);
  imm_s |= (insn & 0x00000F80) >> (7 - 0);

  if (insn & 0x80000000)
    imm_s |= 0xFFFFF000; // sign-extend the left

  return imm_s;
}

// render funcs
std::string rv32i_decode::render_illegal_insn(uint32_t insn) {
  return "ERROR: UNIMPLEMENTED INSTRUCTION";
}

std::string rv32i_decode::render_lui(uint32_t insn) {
  uint32_t rd = get_rd(insn);
  int32_t imm_u = get_imm_u(insn);

  std::ostringstream os;
  os << render_mnemonic("lui") << render_reg(rd) << ","
     << to_hex0x20((imm_u >> 12) & 0x0fffff);

  return os.str();
}

std::string rv32i_decode::render_auipc(uint32_t insn) {
  //
  return render_utype(insn, "auipc");
}

// helper render funcs
std::string rv32i_decode::render_utype(uint32_t insn, const char *mnemonic) {
  // render all utype insns
  uint32_t rd = get_rd(insn);
  int32_t imm_u = get_imm_u(insn);

  std::ostringstream os;

  os << render_mnemonic(mnemonic) << render_reg(rd) << ","
     << to_hex0x20((imm_u >> 12) & 0x0FFFFF);

  return os.str();
}

std::string rv32i_decode::render_rtype(uint32_t insn, const char *mnemonic) {
  uint32_t rd = get_rd(insn);

  std::ostringstream os;

  os << render_mnemonic(mnemonic) << render_reg(rd) << ","
     << render_reg(get_rs1(insn)) << "," << render_reg(get_rs2(insn));

  return os.str();
}

// last render funcs
std::string rv32i_decode::render_reg(int r) {
  std::ostringstream os;
  os << 'x' << r;

  return os.str();
}

std::string rv32i_decode::render_mnemonic(const std::string &m) {
  std::ostringstream os;

  os << std::left << std::setw(8) << std::setfill(' ') << m;

  return os.str();
}
