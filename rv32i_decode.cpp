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
  case opcode_btype:
    switch (get_funct3(insn)) {
    default:
      return render_illegal_insn(insn);
    case funct3_beq:
      return render_btype(addr, insn, "beq");
    case funct3_bge:
      return render_btype(addr, insn, "bge");
    case funct3_bgeu:
      return render_btype(addr, insn, "bgeu");
    case funct3_blt:
      return render_btype(addr, insn, "blt");
    case funct3_bltu:
      return render_btype(addr, insn, "bltu");
    case funct3_bne:
      return render_btype(addr, insn, "bne");
    }
  case opcode_jal:
    return render_jal(addr, insn);
  case opcode_system:
    switch (get_funct3(insn)) { // TODO THIS IS FOR CSRRX
    default:
      return render_illegal_insn(insn);
    case funct3_add: // ebreak and ecall
      switch (insn) {
      default:
        return render_illegal_insn(insn);
      case insn_ebreak:
        return render_ebreak(insn);
      case insn_ecall:
        return render_ecall(insn);
      }
    case funct3_csrrw:
      return render_csrrx(insn, "csrrw");
    case funct3_csrrs:
      return render_csrrx(insn, "csrrs");
    case funct3_csrrc:
      return render_csrrx(insn, "csrrc");
    case funct3_csrrwi:
      return render_csrrxi(insn, "csrrwi");
    case funct3_csrrsi:
      return render_csrrxi(insn, "csrrsi");
    case funct3_csrrci:
      return render_csrrxi(insn, "csrrci");
    }
  case opcode_jalr:
    return render_itype_load(insn, "jalr");
  case opcode_load_imm:
    switch (get_funct3(insn)) {
    default:
      return render_illegal_insn(insn);
    case funct3_lb:
      return render_itype_load(insn, "lb");
    case funct3_lbu:
      return render_itype_load(insn, "lbu");
    case funct3_lh:
      return render_itype_load(insn, "lh");
    case funct3_lw:
      return render_itype_load(insn, "lw");
    case funct3_lhu:
      return render_itype_load(insn, "lhu");
    }
  case opcode_alu_imm:
    switch (get_funct3(insn)) {
    default:
      return render_illegal_insn(insn);
    case funct3_add:
      return render_itype_alu(insn, "addi", get_imm_i(insn));
    case funct3_slt:
      return render_itype_alu(insn, "slti", get_imm_i(insn));
    case funct3_sltu:
      return render_itype_alu(insn, "sltiu", get_imm_i(insn));
    case funct3_xor:
      return render_itype_alu(insn, "xori", get_imm_i(insn));
    case funct3_or:
      return render_itype_alu(insn, "ori", get_imm_i(insn));
    case funct3_and:
      return render_itype_alu(insn, "andi", get_imm_i(insn));
    case funct3_sll:
      return render_itype_alu(insn, "slli", get_imm_i(insn) % XLEN);
    case funct3_srx:
      switch (get_funct7(insn)) {
      default:
        return render_illegal_insn(insn);
      case funct7_sra:
        return render_itype_alu(insn, "srai", get_imm_i(insn) % XLEN);
      case funct7_srl:
        return render_itype_alu(insn, "srli", get_imm_i(insn) % XLEN);
      }
    }
  case opcode_stype:
    switch (get_funct3(insn)) {
    default:
      return render_illegal_insn(insn);
    case funct3_sb:
      return render_stype(insn, "sb");
    case funct3_sh:
      return render_stype(insn, "sh");
    case funct3_sw:
      return render_stype(insn, "sw");
    }
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

int32_t rv32i_decode::get_imm_i(uint32_t insn) {
  int32_t imm_i = ((int32_t)insn) >> 20;

  return imm_i;
}

int32_t rv32i_decode::get_imm_u(uint32_t insn) {
  int32_t imm_u = (insn & 0xFFFFF000);
  return imm_u;
}

int32_t rv32i_decode::get_imm_b(uint32_t insn) {
  int32_t imm_b =
      (((insn >> 8) & 0xF) | (((insn >> 25) & 0x3F) << 4) |
       (((insn >> 7) & 0x1) << 10) | ((((int32_t)insn) >> 31)) << 11)
      << 1;

  return imm_b;
}

int32_t rv32i_decode::get_imm_s(uint32_t insn) {
  int32_t imm_s = (insn & 0xFE000000) >> (25 - 5);
  imm_s |= (insn & 0x00000F80) >> (7 - 0);

  if (insn & 0x80000000)
    imm_s |= 0xFFFFF000; // sign-extend the left

  return imm_s;
}

int32_t rv32i_decode::get_imm_j(uint32_t insn) {
  int32_t bef = ((insn >> 21) & 0x3FF) | (((insn >> 20) & 0x1) << 10);

  int32_t second_half =
      (((insn >> 12) & 0xFF) << 11) | ((((int32_t)insn) >> 31)) << 19;

  return ((bef | second_half) << 1);
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

std::string rv32i_decode::render_jal(uint32_t addr, uint32_t insn) {
  int32_t imm_j = get_imm_j(insn);
  uint32_t pc = addr + imm_j;
  uint32_t rd = get_rd(insn);
  std::ostringstream os;

  os << render_mnemonic("jal") << render_reg(rd) << "," << to_hex0x32(pc);

  return os.str();
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

std::string rv32i_decode::render_btype(uint32_t addr, uint32_t insn,
                                       const char *mnemonic) {
  std::ostringstream os;
  uint32_t pc = addr; // TODO this is to be moved because it only deals with BEQ
  uint32_t offset; // = (get_rs1(insn) == get_rs2(insn) ? get_imm_b(insn) : 4);

  switch (get_funct3(insn)) {
  default:
    return render_illegal_insn(insn);
  case funct3_beq:
    offset = (get_rs1(insn) == get_rs2(insn) ? get_imm_b(insn) : 4);
    break;
  case funct3_bge:
    offset = (get_rs1(insn) >= get_rs2(insn) ? get_imm_b(insn) : 4);
    break;
  case funct3_bgeu:
    offset = (get_rs1(insn) >= get_rs2(insn) ? get_imm_b(insn) : 4);
    break;
  case funct3_blt:
    offset = (get_rs1(insn) < get_rs2(insn) ? get_imm_b(insn) : 4);
    break;
  case funct3_bltu:
    offset = (get_rs1(insn) < get_rs2(insn) ? get_imm_b(insn) : 4);
    break;
  case funct3_bne:
    offset = (get_rs1(insn) == get_rs2(insn) ? get_imm_b(insn) : 4);
    break;
  }

  pc += offset;

  os << render_mnemonic(mnemonic) << render_reg(get_rs1(insn)) << ","
     << render_reg(get_rs2(insn)) << "," << to_hex0x32(pc);

  return os.str();
}

std::string rv32i_decode::render_itype_load(uint32_t insn,
                                            const char *mnemonic) {
  uint32_t rd = get_rd(insn);
  int32_t imm_i = get_imm_i(insn);
  uint32_t rs1 = get_rs1(insn);
  std::ostringstream os;

  os << render_mnemonic(mnemonic) << render_reg(rd) << ","
     << render_base_disp(rs1, imm_i);

  return os.str();
}

std::string rv32i_decode::render_itype_alu(uint32_t insn, const char *mnemonic,
                                           int32_t imm_i) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  std::ostringstream os;

  os << render_mnemonic(mnemonic) << render_reg(rd) << "," << render_reg(rs1)
     << "," << imm_i;

  return os.str();
}

std::string rv32i_decode::render_stype(uint32_t insn, const char *mnemonic) {
  uint32_t imm_s = get_imm_s(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);
  std::ostringstream os;

  os << render_mnemonic(mnemonic) << render_reg(rs2) << "," << (imm_s) << "("
     << render_reg(rs1) << ")";

  return os.str();
}

std::string rv32i_decode::render_rtype(uint32_t insn, const char *mnemonic) {
  uint32_t rd = get_rd(insn);

  std::ostringstream os;

  os << render_mnemonic(mnemonic) << render_reg(rd) << ","
     << render_reg(get_rs1(insn)) << "," << render_reg(get_rs2(insn));

  return os.str();
}

std::string rv32i_decode::render_ecall(uint32_t insn) {
  std::ostringstream os;

  os << std::left << std::setw(5) << std::setfill(' ') << "ecall";

  return os.str();
}

std::string rv32i_decode::render_ebreak(uint32_t insn) {
  std::ostringstream os;

  os << std::left << std::setw(6) << std::setfill(' ') << "ebreak";

  return os.str();
}

std::string rv32i_decode::render_csrrx(uint32_t insn, const char *mnemonic) {
  uint32_t rd = get_rd(insn);
  uint32_t csr = get_imm_i(insn);
  uint32_t rs1 = get_rs1(insn);
  std::ostringstream os;

  os << render_mnemonic(mnemonic) << render_reg(rd) << "," << to_hex0x12(csr)
     << "," << render_reg(rs1);

  return os.str();
}

std::string rv32i_decode::render_csrrxi(uint32_t insn, const char *mnemonic) {
  uint32_t rd = get_rd(insn);
  uint32_t csr = get_imm_i(insn);
  uint32_t zimm = get_rs1(insn);
  std::ostringstream os;

  os << render_mnemonic(mnemonic) << render_reg(rd) << "," << to_hex0x12(csr)
     << "," << (int)(zimm);

  return os.str();
}

// last render funcs
std::string rv32i_decode::render_reg(int r) {
  std::ostringstream os;
  os << 'x' << r;

  return os.str();
}

std::string rv32i_decode::render_base_disp(uint32_t regr, int32_t imm) {
  std::ostringstream os;

  os << imm << '(' << 'x' << regr << ')';

  return os.str();
}

std::string rv32i_decode::render_mnemonic(const std::string &m) {
  std::ostringstream os;

  os << std::left << std::setw(8) << std::setfill(' ') << m;

  return os.str();
}
