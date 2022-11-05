/*
 */

#include "rv32i_hart.h"
#include <cstdint>

void rv32i_hart::reset() {
  pc = 0;
  regs.reset();
  insn_counter = 0;
  halt = false;
  halt_reason = "none";
}

void rv32i_hart::dump(const std::string &hdr) const {
  regs.dump(hdr);
  std::cout << hdr << std::right << std::setw(3) << "pc"
            << "  " << to_hex32(pc) << std::endl;
}

void rv32i_hart::tick(const std::string &hdr) {
  if (is_halted())
    return;

  if (show_registers)
    dump(hdr);

  if ((pc % 4) != 0) {
    halt = true;
    halt_reason = "PC alignment error";
    return;
  }

  insn_counter++;
  //  std::cout << "test in tick" << std::endl;

  uint32_t insn = mem.get32(pc);

  if (show_instructions) {
    // print hdr, pc register in hex and 32bit instruction in hex
    std::cout << hdr << std::right << std::setw(9) << to_hex32(pc) << ": "
              << to_hex32(insn) << "  ";
    // decode(pc, insn);
    exec(insn, &std::cout);
  } else {
    exec(insn, nullptr);
  }
}

void rv32i_hart::exec(uint32_t insn, std::ostream *pos) {

  switch (get_opcode(insn)) {
  default:
    exec_illegal_insn(insn, pos);
    return;
  case opcode_system:
    switch (get_funct3(insn)) {
    default:
      exec_illegal_insn(insn, pos);
      return;
    case funct3_csrrw:
      exec_csrrw(insn, pos);
      return;
    case funct3_csrrs:
      exec_csrrs(insn, pos);
      return;
    case funct3_csrrc:
      exec_csrrc(insn, pos);
      return;
    case funct3_csrrwi:
      exec_csrrwi(insn, pos);
      return;
    case funct3_csrrsi:
      exec_csrrsi(insn, pos);
      return;
    case funct3_csrrci:
      exec_csrrci(insn, pos);
      return;
    case funct3_add:
      switch (insn) {
      default:
        exec_illegal_insn(insn, pos);
        return;
      case insn_ebreak:
        exec_ebreak(insn, pos);
        return;
      }
    }
  case opcode_lui:
    exec_lui(insn, pos);
    return;
  case opcode_auipc:
    exec_auipc(insn, pos);
    return;
  case opcode_btype:
    switch (get_funct3(insn)) {
    default:
      exec_illegal_insn(insn, pos);
      return;
    case funct3_beq:
      exec_beq(insn, pos);
      return;
    case funct3_bge:
      exec_bge(insn, pos);
      return;
    case funct3_bgeu:
      exec_bgeu(insn, pos);
      return;
    case funct3_blt:
      exec_blt(insn, pos);
      return;
    case funct3_bltu:
      exec_bltu(insn, pos);
      return;
    case funct3_bne:
      exec_bne(insn, pos);
      return;
    }
  case opcode_jal:
    exec_jal(insn, pos);
    return;
  case opcode_jalr:
    exec_jalr(insn, pos);
    return;
  case opcode_alu_imm:
    switch (get_funct3(insn)) {
    default:
      exec_illegal_insn(insn, pos);
      return;
    case funct3_add:
      exec_addi(insn, pos);
      return;
    case funct3_slt:
      exec_slti(insn, pos);
      return;
    case funct3_sltu:
      exec_sltiu(insn, pos);
      return;
    case funct3_xor:
      exec_xori(insn, pos);
      return;
    case funct3_or:
      exec_ori(insn, pos);
      return;
    case funct3_and:
      exec_andi(insn, pos);
      return;
    case funct3_sll:
      exec_slli(insn, pos);
      return;
    case funct3_srx:
      switch (get_funct7(insn)) {
      default:
        exec_illegal_insn(insn, pos);
        return;
      case funct7_srl:
        exec_srli(insn, pos);
        return;
      case funct7_sra:
        exec_srai(insn, pos);
        return;
      }
    }
  case opcode_load_imm:
    switch (get_funct3(insn)) {
    default:
      exec_illegal_insn(insn, pos);
      return;
    case funct3_lb:
      exec_lb(insn, pos);
      return;
    case funct3_lbu:
      exec_lbu(insn, pos);
      return;
    case funct3_lh:
      exec_lh(insn, pos);
      return;
    case funct3_lhu:
      exec_lhu(insn, pos);
      return;
    case funct3_lw:
      exec_lw(insn, pos);
      return;
    }
  case opcode_stype:
    switch (get_funct3(insn)) {
    default:
      exec_illegal_insn(insn, pos);
      return;
    case funct3_sb:
      exec_sb(insn, pos);
      return;
    case funct3_sh:
      exec_sh(insn, pos);
      return;
    case funct3_sw:
      exec_sw(insn, pos);
      return;
    }
  case opcode_rtype:
    switch (get_funct3(insn)) {
    default:
      exec_illegal_insn(insn, pos);
      return;
    case funct3_sll:
      exec_sll(insn, pos);
      return;
    case funct3_slt:
      exec_slt(insn, pos);
      return;
    case funct3_sltu:
      exec_sltu(insn, pos);
      return;
    case funct3_or:
      exec_or(insn, pos);
      return;
    case funct3_and:
      exec_and(insn, pos);
      return;
    case funct3_xor:
      exec_xor(insn, pos);
      return;
    case funct3_add:
      switch (get_funct7(insn)) {
      default:
        exec_illegal_insn(insn, pos);
        return;
      case funct7_add:
        exec_add(insn, pos);
        return;
      case funct7_sub:
        exec_sub(insn, pos);
        return;
      }
    case funct3_srx:
      switch (get_funct7(insn)) {
      default:
        exec_illegal_insn(insn, pos);
        return;
      case funct7_srl:
        exec_srl(insn, pos);
        return;
      case funct7_sra:
        exec_sra(insn, pos);
        return;
      }
    }
  }
}

void rv32i_hart::exec_ebreak(uint32_t insn, std::ostream *pos) {

  if (pos) {
    std::string s = render_ebreak(insn);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// HALT";
  }

  halt = true;
  halt_reason = "EBREAK instruction";
}

void rv32i_hart::exec_lui(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t imm = get_imm_u(insn);
  //

  regs.set(rd, imm);

  if (pos) {
    std::string s = render_utype(insn, "lui");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << to_hex0x32(regs.get(rd))
         << std::endl;
  }

  pc += 4;
}

void rv32i_hart::exec_auipc(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t imm = get_imm_u(insn);

  regs.set(rd, (imm + pc));
  if (pos) {
    std::string s = render_utype(insn, "auipc");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << to_hex0x32(pc) << " + "
         << to_hex0x32(imm) << " = " << to_hex0x32(regs.get(rd)) << std::endl;
  }

  pc += 4;
}

void rv32i_hart::exec_beq(uint32_t insn, std::ostream *pos) {
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);
  uint32_t imm = get_imm_b(insn);

  if (pos) {
    std::string s = render_btype(pc, insn, "beq");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << "pc"
         << " += "
         << "(" << to_hex0x32(rs1) << " == " << to_hex0x32(rs2) << " ? "
         << to_hex0x32(imm) << " : "
         << "4)"
         << " = " << to_hex0x32(pc + (regs.get(rs1) == regs.get(rs2) ? imm : 4))
         << std::endl;
  }

  if (regs.get(rs1) == regs.get(rs2)) {
    pc += imm;
  } else {
    pc += 4;
  }
}

void rv32i_hart::exec_bge(uint32_t insn, std::ostream *pos) {
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);
  uint32_t imm = get_imm_b(insn);

  if (pos) {
    std::string s = render_btype(pc, insn, "bge");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << "pc"
         << " += "
         << "(" << to_hex0x32(regs.get(rs1))
         << " >= " << to_hex0x32(regs.get(rs2)) << " ? " << to_hex0x32(imm)
         << " : "
         << "4)"
         << " = " << to_hex0x32(pc + (regs.get(rs1) >= regs.get(rs2) ? imm : 4))
         << std::endl;
  }

  if (regs.get(rs1) >= regs.get(rs2)) {
    pc += imm;
  } else {
    pc += 4;
  }
}

void rv32i_hart::exec_bgeu(uint32_t insn, std::ostream *pos) {
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);
  uint32_t imm = get_imm_b(insn);

  if (pos) {
    std::string s = render_btype(pc, insn, "bgeu");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << "pc"
         << " += "
         << "(" << to_hex0x32(regs.get(rs1)) << " >=U "
         << to_hex0x32(regs.get(rs2)) << " ? " << to_hex0x32(imm) << " : "
         << "4)"
         << " = "
         << to_hex0x32(pc + ((uint32_t)regs.get(rs1) >= (uint32_t)regs.get(rs2)
                                 ? imm
                                 : 4))
         << std::endl;
  }

  if ((uint32_t)regs.get(rs1) >= (uint32_t)regs.get(rs2)) {
    pc += imm;
  } else {
    pc += 4;
  }
}

void rv32i_hart::exec_blt(uint32_t insn, std::ostream *pos) {
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);
  uint32_t imm = get_imm_b(insn);

  if (pos) {
    std::string s = render_btype(pc, insn, "blt");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << "pc"
         << " += "
         << "(" << to_hex0x32(regs.get(rs1)) << " < "
         << to_hex0x32(regs.get(rs2)) << " ? " << to_hex0x32(imm) << " : "
         << "4)"
         << " = " << to_hex0x32(pc + (regs.get(rs1) < regs.get(rs2) ? imm : 4))
         << std::endl;
  }

  if (regs.get(rs1) < regs.get(rs2)) {
    pc += imm;
  } else {
    pc += 4;
  }
}

void rv32i_hart::exec_bltu(uint32_t insn, std::ostream *pos) {
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);
  uint32_t imm = get_imm_b(insn);

  if (pos) {
    std::string s = render_btype(pc, insn, "bltu");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << "pc"
         << " += "
         << "(" << to_hex0x32((uint32_t)regs.get(rs1)) << " <U "
         << to_hex0x32((uint32_t)regs.get(rs2)) << " ? " << to_hex0x32(imm)
         << " : "
         << "4)"
         << " = "
         << to_hex0x32(pc + ((uint32_t)regs.get(rs1) < (uint32_t)regs.get(rs2)
                                 ? imm
                                 : 4))
         << std::endl;
  }

  if ((uint32_t)regs.get(rs1) < (uint32_t)regs.get(rs2)) {
    pc += imm;
  } else {
    pc += 4;
  }
}

void rv32i_hart::exec_bne(uint32_t insn, std::ostream *pos) {
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);
  uint32_t imm = get_imm_b(insn);

  if (pos) {
    std::string s = render_btype(pc, insn, "bne");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << "pc"
         << " += "
         << "(" << to_hex0x32(regs.get(rs1))
         << " != " << to_hex0x32(regs.get(rs2)) << " ? " << to_hex0x32(imm)
         << " : "
         << "4)"
         << " = "
         << to_hex0x32((pc + (regs.get(rs1) != regs.get(rs2) ? imm : 4)))
         << std::endl;
  }

  if (regs.get(rs1) != regs.get(rs2)) {
    pc += imm;
  } else {
    pc += 4;
  }
}

void rv32i_hart::exec_jal(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t imm = get_imm_j(insn);
  regs.set(rd, (pc + 4));

  if (pos) {
    std::string s = render_jal((imm), insn);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << to_hex0x32(regs.get(rd))
         << ",  "
         << "pc"
         << " = " << to_hex0x32(pc) << " + " << to_hex0x32(imm) << " = "
         << to_hex0x32(pc + imm) << std::endl;
  }

  pc = (imm + pc);
}

void rv32i_hart::exec_jalr(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t imm = get_imm_i(insn);
  regs.set(rd, (pc + 4));

  if (pos) {
    std::string s = render_itype_load(insn, "jalr");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << to_hex0x32(regs.get(rd))
         << ",  "
         << "pc"
         << " = "
         << "(" << to_hex0x32(regs.get(rs1)) << " + " << to_hex0x32(imm) << ")"
         << " & "
         << "0xfffffffe"
         << " = " << to_hex0x32((regs.get(rs1) + imm) & 0xfffffffe)
         << std::endl;
  }

  pc = (regs.get(rs1) + imm) & 0xfffffffe;
}

void rv32i_hart::exec_addi(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t imm = get_imm_i(insn);
  regs.set(rd, (regs.get(rs1) + imm));

  if (pos) {
    std::string s = render_itype_alu(insn, "addi", imm);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << to_hex0x32(regs.get(rs1))
         << " + " << to_hex0x32(regs.get(rs1) + imm) << " = "
         << to_hex0x32(regs.get(rs1) + imm) << std::endl;
  }

  pc += 4;
}

void rv32i_hart::exec_slti(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t imm = get_imm_i(insn);

  if (pos) {
    std::string s = render_itype_alu(insn, "slti", imm);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = "
         << "(" << to_hex0x32((int32_t)regs.get(rs1)) << " < " << imm << ")"
         << " ? "
         << "1"
         << " : "
         << "0"
         << " = " << to_hex0x32(((int32_t)regs.get(rs1) < imm ? 1 : 0))
         << std::endl;
  }

  if ((int32_t)regs.get(rs1) < imm) {
    regs.set(rd, 1);
  } else {
    regs.set(rd, 0);
  }

  pc += 4;
}

void rv32i_hart::exec_sltiu(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t imm = get_imm_i(insn);

  if (pos) {
    std::string s = render_itype_alu(insn, "sltiu", imm);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = "
         << "(" << to_hex0x32((uint32_t)regs.get(rs1)) << " <U "
         << (uint32_t)imm << ")"
         << " ? "
         << "1"
         << " : "
         << "0"
         << " = "
         << to_hex0x32(((uint32_t)regs.get(rs1) < (uint32_t)imm ? 1 : 0))
         << std::endl;
  }

  if ((uint32_t)regs.get(rs1) < (uint32_t)imm) {
    regs.set(rd, 1);
  } else {
    regs.set(rd, 0);
  }

  pc += 4;
}

void rv32i_hart::exec_xori(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t imm = get_imm_i(insn);

  if (pos) {
    std::string s = render_itype_alu(insn, "xori", imm);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << to_hex0x32(regs.get(rs1))
         << " ^ " << to_hex0x32(imm) << " = "
         << to_hex0x32((regs.get(rs1) ^ imm)) << std::endl;
  }

  regs.set(rd, (regs.get(rs1) ^ imm));

  pc += 4;
}

void rv32i_hart::exec_ori(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t imm = get_imm_i(insn);

  if (pos) {
    std::string s = render_itype_alu(insn, "ori", imm);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << to_hex0x32(regs.get(rs1))
         << " | " << to_hex0x32(imm) << " = "
         << to_hex0x32((regs.get(rs1) | imm)) << std::endl;
  }

  regs.set(rd, (regs.get(rs1) | imm));

  pc += 4;
}

void rv32i_hart::exec_andi(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t imm = get_imm_i(insn);

  if (pos) {
    std::string s = render_itype_alu(insn, "andi", imm);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << to_hex0x32(regs.get(rs1))
         << " & " << to_hex0x32(imm) << " = "
         << to_hex0x32((regs.get(rs1) & imm)) << std::endl;
  }

  regs.set(rd, (regs.get(rs1) & imm));

  pc += 4;
}

void rv32i_hart::exec_slli(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t imm = get_imm_i(insn);

  if (pos) {
    std::string s = render_itype_alu(insn, "slli", imm % XLEN);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << to_hex0x32(regs.get(rs1))
         << " << " << (imm % XLEN) << " = "
         << to_hex0x32((regs.get(rs1) << (imm % XLEN))) << std::endl;
  }

  regs.set(rd, (regs.get(rs1) << (imm % XLEN)));

  pc += 4;
}

void rv32i_hart::exec_srli(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t imm = get_imm_i(insn);

  if (pos) {
    std::string s = render_itype_alu(insn, "srli", imm % XLEN);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << to_hex0x32(regs.get(rs1))
         << " >> " << (imm % XLEN) << " = "
         << to_hex0x32(((uint32_t)regs.get(rs1) >> (uint32_t)(imm % XLEN)))
         << std::endl;
  }

  regs.set(rd, ((uint32_t)regs.get(rs1) >> (uint32_t)(imm % XLEN)));

  pc += 4;
}

void rv32i_hart::exec_srai(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t imm = get_imm_i(insn);

  if (pos) {
    std::string s = render_itype_alu(insn, "srai", imm % XLEN);
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << to_hex0x32(regs.get(rs1))
         << " >> " << (imm % XLEN) << " = "
         << to_hex0x32((regs.get(rs1) >> (imm % XLEN))) << std::endl;
  }

  regs.set(rd, (regs.get(rs1) >> (imm % XLEN)));

  pc += 4;
}

void rv32i_hart::exec_lb(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t imm = get_imm_i(insn);

  if (pos) {
    std::string s = render_itype_load(insn, "lb");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = "
         << "sx(m8(" << to_hex0x32(regs.get(rs1)) << " + " << to_hex0x32(imm)
         << "))"
         << " = " << to_hex0x32(mem.get8_sx(regs.get(rs1) + imm)) << std::endl;
  }

  regs.set(rd, (mem.get8_sx(regs.get(rs1) + imm)));
  pc += 4;
}

void rv32i_hart::exec_lbu(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t imm = get_imm_i(insn);

  if (pos) {
    std::string s = render_itype_load(insn, "lbu");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = "
         << "zx(m8(" << to_hex0x32(regs.get(rs1)) << " + " << to_hex0x32(imm)
         << "))"
         << " = " << to_hex0x32(mem.get8(regs.get(rs1) + imm)) << std::endl;
  }

  regs.set(rd, (mem.get8(regs.get(rs1) + imm)));
  pc += 4;
}

void rv32i_hart::exec_lh(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t imm = get_imm_i(insn);

  if (pos) {
    std::string s = render_itype_load(insn, "lh");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = "
         << "sx(m16(" << to_hex0x32(regs.get(rs1)) << " + " << to_hex0x32(imm)
         << "))"
         << " = " << to_hex0x32(mem.get16_sx(regs.get(rs1) + imm)) << std::endl;
  }

  regs.set(rd, (mem.get16_sx(regs.get(rs1) + imm)));
  pc += 4;
}

void rv32i_hart::exec_lhu(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t imm = get_imm_i(insn);

  if (pos) {
    std::string s = render_itype_load(insn, "lhu");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = "
         << "zx(m16(" << to_hex0x32(regs.get(rs1)) << " + " << to_hex0x32(imm)
         << "))"
         << " = " << to_hex0x32(mem.get16(regs.get(rs1) + imm)) << std::endl;
  }

  regs.set(rd, (mem.get16(regs.get(rs1) + imm)));
  pc += 4;
}

void rv32i_hart::exec_lw(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  int32_t imm = get_imm_i(insn);

  if (pos) {
    std::string s = render_itype_load(insn, "lw");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = "
         << "sx(m32(" << to_hex0x32(regs.get(rs1)) << " + " << to_hex0x32(imm)
         << "))"
         << " = " << to_hex0x32(mem.get32_sx(regs.get(rs1) + imm)) << std::endl;
  }

  regs.set(rd, (mem.get32_sx(regs.get(rs1) + imm)));
  pc += 4;
}

void rv32i_hart::exec_sb(uint32_t insn, std::ostream *pos) {
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);
  int32_t imm = get_imm_s(insn);

  if (pos) {
    std::string s = render_stype(insn, "sb");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << "m8(" << to_hex0x32(regs.get(rs1)) << " + " << to_hex0x32(imm)
         << ")"
         << " = " << to_hex0x32(regs.get(rs2) & 0xff) << std::endl;
  }

  mem.set8((regs.get(rs1) + imm), (regs.get(rs2) & 0xff));
  pc += 4;
}

void rv32i_hart::exec_sh(uint32_t insn, std::ostream *pos) {
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);
  int32_t imm = get_imm_s(insn);

  if (pos) {
    std::string s = render_stype(insn, "sh");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << "m16(" << to_hex0x32(regs.get(rs1)) << " + " << to_hex0x32(imm)
         << ")"
         << " = " << to_hex0x32(regs.get(rs2) & 0xffff) << std::endl;
  }

  mem.set16((regs.get(rs1) + imm), (regs.get(rs2) & 0xffff));
  pc += 4;
}

void rv32i_hart::exec_sw(uint32_t insn, std::ostream *pos) {
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);
  int32_t imm = get_imm_s(insn);

  if (pos) {
    std::string s = render_stype(insn, "sw");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// "
         << "m32(" << to_hex0x32(regs.get(rs1)) << " + " << to_hex0x32(imm)
         << ")"
         << " = " << to_hex0x32(regs.get(rs2) & 0xffffffff) << std::endl;
  }

  mem.set32((regs.get(rs1) + imm), (regs.get(rs2) & 0xffffffff));
  pc += 4;
}

void rv32i_hart::exec_add(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);

  if (pos) {
    std::string s = render_rtype(insn, "add");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << to_hex0x32(regs.get(rs1))
         << " + " << to_hex0x32(regs.get(rs2)) << " = "
         << to_hex0x32(regs.get(rs1) + regs.get(rs2)) << std::endl;
  }

  regs.set(rd, (regs.get(rs1) + (regs.get(rs2))));

  pc += 4;
}

void rv32i_hart::exec_sub(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);

  if (pos) {
    std::string s = render_rtype(insn, "sub");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << to_hex0x32(regs.get(rs1))
         << " - " << to_hex0x32(regs.get(rs2)) << " = "
         << to_hex0x32(regs.get(rs1) - regs.get(rs2)) << std::endl;
  }

  regs.set(rd, (regs.get(rs1) - (regs.get(rs2))));

  pc += 4;
}

void rv32i_hart::exec_sll(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);

  if (pos) {
    std::string s = render_rtype(insn, "sll");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << to_hex0x32(regs.get(rs1))
         << " << " << (regs.get(rs2) & 0x1f) << " = "
         << to_hex0x32(regs.get(rs1) << (regs.get(rs2) & 0x1f)) << std::endl;
  }

  regs.set(rd, (regs.get(rs1) << ((regs.get(rs2) & 0x1f))));

  pc += 4;
}

void rv32i_hart::exec_slt(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);

  int32_t val = (regs.get(rs1) < regs.get(rs2)) ? 1 : 0;

  if (pos) {
    std::string s = render_rtype(insn, "slt");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = "
         << "(" << to_hex0x32(regs.get(rs1)) << " < "
         << to_hex0x32(regs.get(rs2)) << ") ? 1 : 0 = " << to_hex0x32(val)
         << std::endl;
  }

  regs.set(rd, val);

  pc += 4;
}

void rv32i_hart::exec_sltu(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);

  int32_t val = ((uint32_t)regs.get(rs1) < (uint32_t)regs.get(rs2)) ? 1 : 0;

  if (pos) {
    std::string s = render_rtype(insn, "sltu");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = "
         << "(" << to_hex0x32((uint32_t)regs.get(rs1)) << " <U "
         << to_hex0x32((uint32_t)regs.get(rs2))
         << ") ? 1 : 0 = " << to_hex0x32(val) << std::endl;
  }

  regs.set(rd, val);

  pc += 4;
}

void rv32i_hart::exec_xor(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);

  int32_t val = (regs.get(rs1) ^ regs.get(rs2));

  if (pos) {
    std::string s = render_rtype(insn, "xor");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << to_hex0x32(regs.get(rs1))
         << " ^ " << to_hex0x32(regs.get(rs2)) << " = " << to_hex0x32(val)
         << std::endl;
  }

  regs.set(rd, val);

  pc += 4;
}

void rv32i_hart::exec_srl(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);

  if (pos) {
    std::string s = render_rtype(insn, "srl");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << to_hex0x32(regs.get(rs1))
         << " >> " << (regs.get(rs2) & 0x1f) << " = "
         << to_hex0x32((uint32_t)regs.get(rs1) >>
                       ((uint32_t)regs.get(rs2) & 0x1f))
         << std::endl;
  }

  regs.set(rd, (uint32_t)(regs.get(rs1) << ((uint32_t)(regs.get(rs2) & 0x1f))));

  pc += 4;
}

void rv32i_hart::exec_sra(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);

  if (pos) {
    std::string s = render_rtype(insn, "sra");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << to_hex0x32(regs.get(rs1))
         << " >> " << (regs.get(rs2) & 0x1f) << " = "
         << to_hex0x32(regs.get(rs1) >> (regs.get(rs2) & 0x1f)) << std::endl;
  }

  regs.set(rd, (regs.get(rs1) << ((regs.get(rs2) & 0x1f))));

  pc += 4;
}

void rv32i_hart::exec_or(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);

  if (pos) {
    std::string s = render_rtype(insn, "or");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << to_hex0x32(regs.get(rs1))
         << " | " << to_hex0x32(regs.get(rs2)) << " = "
         << to_hex0x32(regs.get(rs1) | (regs.get(rs2))) << std::endl;
  }

  regs.set(rd, (regs.get(rs1) | ((regs.get(rs2)))));

  pc += 4;
}

void rv32i_hart::exec_and(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t rs2 = get_rs2(insn);

  if (pos) {
    std::string s = render_rtype(insn, "and");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << to_hex0x32(regs.get(rs1))
         << " & " << to_hex0x32(regs.get(rs2)) << " = "
         << to_hex0x32(regs.get(rs1) & (regs.get(rs2))) << std::endl;
  }

  regs.set(rd, (regs.get(rs1) & ((regs.get(rs2)))));

  pc += 4;
}

void rv32i_hart::exec_csrrw(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t csr = mhartid;
  regs.set(rd, csr);
  csr = regs.get(rs1);
  mhartid = csr;

  if (pos) {
    std::string s = render_csrrx(insn, "csrrw");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << (int32_t)regs.get(rd)
         << std::endl;
  }

  pc += 4;
}

void rv32i_hart::exec_csrrs(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t csr = mhartid;
  regs.set(rd, csr);
  csr = (csr | regs.get(rs1));
  mhartid = csr;

  if (pos) {
    std::string s = render_csrrx(insn, "csrrs");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << (int32_t)regs.get(rd)
         << std::endl;
  }

  pc += 4;
}

void rv32i_hart::exec_csrrc(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t csr = mhartid;
  regs.set(rd, csr);
  csr = (csr | (~regs.get(rs1)));
  mhartid = csr;

  if (pos) {
    std::string s = render_csrrx(insn, "csrrc");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << (int32_t)regs.get(rd)
         << std::endl;
  }

  pc += 4;
}

void rv32i_hart::exec_csrrwi(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t csr = mhartid;
  int32_t zimm = regs.get(rs1);
  regs.set(rd, csr);
  csr = zimm;
  mhartid = csr;

  if (pos) {
    std::string s = render_csrrxi(insn, "csrrwi");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << (int32_t)regs.get(rd)
         << std::endl;
  }

  pc += 4;
}

void rv32i_hart::exec_csrrsi(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t csr = mhartid;
  int32_t zimm = regs.get(rs1);
  regs.set(rd, csr);
  csr = zimm;
  mhartid = csr;

  if (pos) {
    std::string s = render_csrrxi(insn, "csrrsi");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << (int32_t)regs.get(rd)
         << std::endl;
  }

  pc += 4;
}

void rv32i_hart::exec_csrrci(uint32_t insn, std::ostream *pos) {
  uint32_t rd = get_rd(insn);
  uint32_t rs1 = get_rs1(insn);
  uint32_t csr = mhartid;
  int32_t zimm = regs.get(rs1);
  regs.set(rd, csr);
  csr = zimm;
  mhartid = csr;

  if (pos) {
    std::string s = render_csrrxi(insn, "csrrci");
    *pos << std::setw(instruction_width) << std::setfill(' ') << std::left << s;
    *pos << "// " << render_reg(rd) << " = " << (int32_t)regs.get(rd)
         << std::endl;
  }

  pc += 4;
}

void rv32i_hart::exec_illegal_insn(uint32_t insn, std::ostream *pos) {
  if (pos)
    *pos << render_illegal_insn(insn);
  halt = true;
  halt_reason = "Illegal instruction";
}
