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

#ifndef RV32I_DECODE_H
#define RV32I_DECODE_H

#include "hex.h"

class rv32i_decode : public hex {
public:
  /**
   * This function will take in an address of the current instruction as well
   * as the instruction itself, pass the insn into a large switch statement to
   * determine which instruction is to be rendered to the screen, depending
   * on the opcode or other sections of the bits in the insn, if no suitable
   *opcode is found then there will be an error displayed on screen.
   *
   * @brief This function will take an instruction and decode it to render
   *        the proper instruction onto the screen.
   *
   * @param addr current address of the instruction loaded.
   * @param insn instruction to decode.
   *
   * @return The final instruction string to be printed to screen.
   *
   ********************************************************************************/
  static std::string decode(uint32_t addr, uint32_t insn);

protected:
  static constexpr int mnemonic_width = 8;

  static constexpr uint32_t opcode_lui = 0b0110111;
  static constexpr uint32_t opcode_auipc = 0b0010111;
  static constexpr uint32_t opcode_jal = 0b1101111;
  static constexpr uint32_t opcode_jalr = 0b1100111;
  static constexpr uint32_t opcode_btype = 0b1100011;
  static constexpr uint32_t opcode_load_imm = 0b0000011;
  static constexpr uint32_t opcode_stype = 0b0100011;
  static constexpr uint32_t opcode_alu_imm = 0b0010011;
  static constexpr uint32_t opcode_rtype = 0b0110011;
  static constexpr uint32_t opcode_system = 0b1110011;

  static constexpr uint32_t funct3_beq = 0b000;
  static constexpr uint32_t funct3_bne = 0b001;
  static constexpr uint32_t funct3_blt = 0b100;
  static constexpr uint32_t funct3_bge = 0b101;
  static constexpr uint32_t funct3_bltu = 0b110;
  static constexpr uint32_t funct3_bgeu = 0b111;

  static constexpr uint32_t funct3_lb = 0b000;
  static constexpr uint32_t funct3_lh = 0b001;
  static constexpr uint32_t funct3_lw = 0b010;
  static constexpr uint32_t funct3_lbu = 0b100;
  static constexpr uint32_t funct3_lhu = 0b101;

  static constexpr uint32_t funct3_sb = 0b000;
  static constexpr uint32_t funct3_sh = 0b001;
  static constexpr uint32_t funct3_sw = 0b010;

  static constexpr uint32_t funct3_add = 0b000;
  static constexpr uint32_t funct3_sll = 0b001;
  static constexpr uint32_t funct3_slt = 0b010;
  static constexpr uint32_t funct3_sltu = 0b011;
  static constexpr uint32_t funct3_xor = 0b100;
  static constexpr uint32_t funct3_srx = 0b101;
  static constexpr uint32_t funct3_or = 0b110;
  static constexpr uint32_t funct3_and = 0b111;

  static constexpr uint32_t funct7_srl = 0b0000000;
  static constexpr uint32_t funct7_sra = 0b0100000;

  static constexpr uint32_t funct7_add = 0b0000000;
  static constexpr uint32_t funct7_sub = 0b0100000;

  static constexpr uint32_t insn_ecall = 0x00000073;
  static constexpr uint32_t insn_ebreak = 0x00100073;

  static constexpr uint32_t funct3_csrrw = 0b001;
  static constexpr uint32_t funct3_csrrs = 0b010;
  static constexpr uint32_t funct3_csrrc = 0b011;
  static constexpr uint32_t funct3_csrrwi = 0b101;
  static constexpr uint32_t funct3_csrrsi = 0b110;
  static constexpr uint32_t funct3_csrrci = 0b111;

  /**
   * This function will take in an instruction and remove
   * bits which are not necessary in order to reach the
   * instructions opcode.
   *
   * @brief This function will take an instruction and decode it to extract
   *        the respective opcode.
   *
   * @param insn instruction to decode.
   *
   * @return extracted opcode.
   *
   ********************************************************************************/
  static uint32_t get_opcode(uint32_t insn);
  /**
   * This function will take in an instruction and remove
   * bits which are not necessary in order to reach the
   * bits which represent the register number.
   *
   * @brief This function will take an instruction and extract
   *        the register number from it.
   *
   * @param insn instruction to process.
   *
   * @return extracted register.
   *
   ********************************************************************************/
  static uint32_t get_rd(uint32_t insn);
  /**
   * This function will take in an instruction and remove
   * bits which are not necessary in order to reach the
   * bits which represent funct3.
   *
   * @brief This function will take an instruction and extract
   *        funct3.
   *
   * @param insn instruction to process.
   *
   * @return extracted funct3.
   *
   ********************************************************************************/
  static uint32_t get_funct3(uint32_t insn);
  /**
   * This function will take in an instruction and remove
   * bits which are not necessary in order to reach the
   * bits which represent rs1.
   *
   * @brief This function will take an instruction and extract
   *        rs1.
   *
   * @param insn instruction to process.
   *
   * @note The same process is taken for get_rs2() except at a different
   *       spot in the instruction.
   *
   * @return extracted rs1.
   *
   ********************************************************************************/
  static uint32_t get_rs1(uint32_t insn);
  static uint32_t get_rs2(uint32_t insn);
  /**
   * This function will take in an instruction and remove
   * bits which are not necessary in order to reach the
   * bits which represent funct7.
   *
   * @brief This function will take an instruction and extract
   *        funct7.
   *
   * @param insn instruction to process.
   *
   * @note The same process is taken for get_funct3() except at a different
   *       spot in the instruction.
   *
   * @return extracted funct7.
   *
   ********************************************************************************/
  static uint32_t get_funct7(uint32_t insn);
  /**
   * This function will take in an instruction and remove
   * bits which are not necessary in order to reach the
   * bits which represent the imm_i.
   *
   * @brief This function will take an instruction and extract
   *        imm_i.
   *
   * @param insn instruction to process.
   *
   * @return extracted imm_i.
   *
   ********************************************************************************/
  static int32_t get_imm_i(uint32_t insn);
  /**
   * This function will take in an instruction and remove
   * bits which are not necessary in order to reach the
   * bits which represent the imm_u.
   *
   * @brief This function will take an instruction and extract
   *        imm_u.
   *
   * @param insn instruction to process.
   *
   * @return extracted imm_u.
   *
   ********************************************************************************/
  static int32_t get_imm_u(uint32_t insn);
  /**
   * This function will take in an instruction and remove
   * bits which are not necessary in order to reach the
   * bits which represent the imm_b.
   *
   * @brief This function will take an instruction and extract
   *        imm_b.
   *
   * @param insn instruction to process.
   *
   * @return extracted imm_b.
   *
   ********************************************************************************/
  static int32_t get_imm_b(uint32_t insn);
  /**
   * This function will take in an instruction and remove
   * bits which are not necessary in order to reach the
   * bits which represent the imm_s.
   *
   * @brief This function will take an instruction and extract
   *        imm_s.
   *
   * @param insn instruction to process.
   *
   * @return extracted imm_s.
   *
   ********************************************************************************/
  static int32_t get_imm_s(uint32_t insn);
  /**
   * This function will take in an instruction and remove
   * bits which are not necessary in order to reach the
   * bits which represent the imm_j.
   *
   * @brief This function will take an instruction and extract
   *        imm_j.
   *
   * @param insn instruction to process.
   *
   * @return extracted imm_j.
   *
   ********************************************************************************/
  static int32_t get_imm_j(uint32_t insn);

  static constexpr uint32_t XLEN = 32;
  /**
   * This function will return an error message to display
   * when the decode() function does not recognize the instruction
   *  passed in to decode.
   *
   * @brief This function will print an error message if
   *        the instruction is not recognized.
   *
   * @param insn instruction to process.
   *
   * @return error string to print.
   *
   ********************************************************************************/
  static std::string render_illegal_insn(uint32_t insn);
  /**
   * This function will return a string which will be
   * formatted to include the instruction mnemonic,
   * along with register and imm_u extracted from
   * the instruction.
   *
   * @brief This function will return a string to print
   *        the lui instruction.
   *
   * @param insn instruction to process.
   *
   * @return string to print out lui instruction.
   *
   ********************************************************************************/
  static std::string render_lui(uint32_t insn);
  /**
   * This function will return a string which will be
   * formatted to include the instruction mnemonic,
   * along with register and imm_u extracted from
   * the instruction.
   *
   * @brief This function will return a string to print
   *        the auipc instruction.
   *
   * @param insn instruction to process.
   *
   * @return string to print out auipc instruction.
   *
   ********************************************************************************/
  static std::string render_auipc(uint32_t insn);
  /**
   * This function will return a string which will be
   * formatted to include the instruction mnemonic,
   * along with register and jump addr extracted from
   * the instruction.
   *
   * @brief This function will return a string to print
   *        the jal instruction.
   *
   * @param addr address where instruction is stored.
   * @param insn instruction to process.
   *
   * @return string to print out jal instruction.
   *
   ********************************************************************************/
  static std::string render_jal(uint32_t addr, uint32_t insn);
  /**
   * This function will return a string which will be
   * formatted to include the instruction mnemonic,
   * along with register, imm_i and rs1 extracted from
   * the instruction.
   *
   * @brief This function will return a string to print
   *        the jalr instruction.
   *
   * @param insn instruction to process.
   *
   * @return string to print out jalr instruction.
   *
   ********************************************************************************/
  static std::string render_jalr(uint32_t insn);
  /**
   * This function will return a string which will be
   * formatted to include the instruction mnemonic,
   * along with rs1, rs2 and the address to branch to,
   * all extracted from the instruction.
   *
   * @brief This function will return a string to print
   *        all b-type instructions.
   *
   * @param addr address of where instruction is stored.
   * @param insn instruction to process.
   * @param mnemonic instruction mnemonic.
   *
   * @return string to print out b-type instruction.
   *
   ********************************************************************************/
  static std::string render_btype(uint32_t addr, uint32_t insn,
                                  const char *mnemonic);
  /**
   * This function will return a string which will be
   * formatted to include the instruction mnemonic,
   * along with register number and imm_u.
   *
   * @brief This function will return a string to print
   *        all u-type instructions.
   *
   * @param insn instruction to process.
   * @param mnemonic instruction mnemonic.
   *
   * @return string to print out u-type instruction.
   *
   ********************************************************************************/
  static std::string render_utype(uint32_t insn, const char *mnemonic);
  /**
   * This function will return a string which will be
   * formatted to include the instruction mnemonic,
   * along with register number, imm_i and rs1.
   *
   * @brief This function will return a string to print
   *        all i-type load instructions.
   *
   * @param insn instruction to process.
   * @param mnemonic instruction mnemonic.
   *
   * @return string to print out i-type load instruction.
   *
   ********************************************************************************/
  static std::string render_itype_load(uint32_t insn, const char *mnemonic);
  /**
   * This function will return a string which will be
   * formatted to include the instruction mnemonic,
   * along with rs2, imm_s and rs1.
   *
   * @brief This function will return a string to print
   *        all s-type instructions.
   *
   * @param insn instruction to process.
   * @param mnemonic instruction mnemonic.
   *
   * @return string to print out s-type instruction.
   *
   ********************************************************************************/
  static std::string render_stype(uint32_t insn, const char *mnemonic);
  /**
   * This function will return a string which will be
   * formatted to include the instruction mnemonic,
   * along with register number, rs1 and imm_i.
   *
   * @brief This function will return a string to print
   *        all i-type alu instructions.
   *
   * @param insn instruction to process.
   * @param mnemonic instruction mnemonic.
   * @param imm_i instruction extracted immediate memory.
   *
   * @return string to print out i-type alu instruction.
   *
   ********************************************************************************/
  static std::string render_itype_alu(uint32_t insn, const char *mnemonic,
                                      int32_t imm_i);
  /**
   * This function will return a string which will be
   * formatted to include the instruction mnemonic,
   * along with register number, rs1 and rs2 extracted from
   * the instruction.
   *
   * @brief This function will return a string to print
   *        all r-type instructions.
   *
   * @param insn instruction to process.
   * @param mnemonic instruction mnemonic.
   *
   * @return string to print out r-type instruction.
   *
   ********************************************************************************/
  static std::string render_rtype(uint32_t insn, const char *mnemonic);
  /**
   * This function will return a string which will be
   * formatted to include the instruction mnemonic.
   *
   * @brief This function will return a string to print ecall instruction.
   *
   * @param insn instruction to process.
   *
   * @return string to print out ecall instruction.
   *
   ********************************************************************************/
  static std::string render_ecall(uint32_t insn);
  /**
   * This function will return a string which will be
   * formatted to include the instruction mnemonic.
   *
   * @brief This function will return a string to print ebreak instruction.
   *
   * @param insn instruction to process.
   *
   * @return string to print out ebreak instruction.
   *
   ********************************************************************************/
  static std::string render_ebreak(uint32_t insn);
  /**
   * This function will return a string which will be
   * formatted to include the instruction mnemonic,
   * along with register number, csr and rs1 extracted from
   * the instruction.
   *
   * @brief This function will return a string to print
   *        all csrrx-type instructions.
   *
   * @param insn instruction to process.
   * @param mnemonic instruction mnemonic.
   *
   * @note this is a subtype of i-type instructions.
   *
   * @return string to print out csrrx-type instruction.
   *
   ********************************************************************************/
  static std::string render_csrrx(uint32_t insn, const char *mnemonic);
  /**
   * This function will return a string which will be
   * formatted to include the instruction mnemonic,
   * along with register number, csr and zimm extracted from
   * the instruction.
   *
   * @brief This function will return a string to print
   *        all csrrxi-type instructions.
   *
   * @param insn instruction to process.
   * @param mnemonic instruction mnemonic.
   *
   * @note this is a subtype of i-type instructions.
   *
   * @return string to print out csrrxi-type instruction.
   *
   ********************************************************************************/
  static std::string render_csrrxi(uint32_t insn, const char *mnemonic);
  /**
   * This function will return a string which will be
   * formatted to include the register number with an
   * 'x' as a prefix.
   *
   * @brief This function will return a string to print
   *        a register number.
   *
   * @param r register number to print.
   *
   * @return string to print out register number.
   *
   ********************************************************************************/
  static std::string render_reg(int r);
  /**
   * This function will return a string which will be
   * formatted to include the calculated base displacement.
   *
   * @brief This function will return base displacement as a string.
   *
   * @param base starting point.
   * @param disp amount to displace.
   *
   * @return string to print out displacement.
   *
   ********************************************************************************/
  static std::string render_base_disp(uint32_t base, int32_t disp);
  /**
   * This function will return a string which will be
   * formatted to include the mnemonic while maintaining
   * the correct amount of white space, left justified.
   *
   * @brief This function will return the mnemonic as a string.
   *
   * @param m mnemonic to process.
   *
   * @return string to print out formatted mnemonic.
   *
   ********************************************************************************/
  static std::string render_mnemonic(const std::string &m);
};
#endif
