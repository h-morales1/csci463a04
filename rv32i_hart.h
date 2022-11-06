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
#ifndef RV32I_HART_H
#define RV32I_HART_H

#include "memory.h"
#include "registerfile.h"
#include "rv32i_decode.h"

class rv32i_hart : public rv32i_decode {
public:
  /**
   * This constructor will initialize the
   * memory object being passed in.
   *
   * @brief initialize mem.
   *
   * @param mem memory object being passed to constructor.
   *
   ********************************************************************************/
  rv32i_hart(memory &m) : mem(m) {}
  /**
   * This function will set the show_instructions
   * bool to b.
   *
   * @brief set show_instructions bool.
   *
   * @param b true or false.
   *
   ********************************************************************************/
  void set_show_instructions(bool b) { show_instructions = b; }
  /**
   * This function will set the show_registers
   * bool to b.
   *
   * @brief set show_registers bool.
   *
   * @param b true or false.
   *
   ********************************************************************************/
  void set_show_registers(bool b) { show_registers = b; }
  /**
   * This function will return the value of
   * bool halt.
   *
   * @brief return value of halt.
   *
   * @return value of halt.
   *
   ********************************************************************************/
  bool is_halted() const { return halt; }
  /**
   * This function will return the value of
   * halt_reason.
   *
   * @brief return the value of halt_reason.
   *
   * @return halt_reason value.
   *
   ********************************************************************************/
  const std ::string &get_halt_reason() const { return halt_reason; }
  /**
   * This function will return the value of
   * insn_counter counter.
   *
   * @brief returns the value of insn_counter.
   *
   * @return value of insn_counter.
   *
   ********************************************************************************/
  uint64_t get_insn_counter() const { return insn_counter; }
  /**
   * This function will set mhartid to i.
   *
   * @brief set mhartid to i.
   *
   * @param i value to set mhartid to.
   *
   ********************************************************************************/
  void set_mhartid(int i) { mhartid = i; }

  /**
   * This function will tell simulator how to
   * execute an instruction. If the hart is halted
   * then return without doing anything, otherwise simulate
   * the execution of a single instruction, incrementing the
   * insn_counter, dumping registers is show_registers is set
   * checking if the pc is a multiple of 4, if not then halt.
   * if show_instructions is set then print the hdr, pc register
   * and call exec(insn, &std::cout) for execution and rendering.
   * otherwise call exec(insn, nullptr) to execute and instruction
   * without rendering anything.
   *
   * @brief execute instruction if not halted.
   *
   * @param hdr prefix to print.
   *
   ********************************************************************************/
  void tick(const std ::string &hdr = "");
  /**
   * This function will prefix each line printed by
   * the hdr, it will dump the GP-regs by calling
   * regs.dump(hdr) and then dump the pc register.
   *
   * @brief dump state of hart.
   *
   * @param hdr prefix to print in dump lines.
   *
   ********************************************************************************/
  void dump(const std ::string &hdr = "") const;
  /**
   * This function will set pc to 0, call
   * regs.reset() to reset registers, set
   * halt to false and set the halt_reason
   * to none.
   *
   * @brief reset the rv32i obj and registerfile.
   *
   ********************************************************************************/
  void reset();

private:
  static constexpr int instruction_width = 35;
  /**
   * This function will use get_xxx() methods to
   * extract needed instruction fields in order
   * to decode the instruction and invoke the associated
   * exec_xxx() function. This is done by extracting the opcode
   * and using a switch case to call helper functions depending on
   * the opcode. If no opcode match is found then exec_illegal_insn()
   * is called and will handle the unmatched instruction.
   *
   * @brief execute given rv32i instruction.
   *
   * @param insn 32bit instruction to execute.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract the rd, imm_u values and then
   * set register rd to the imm_u value. it
   * will print instruction details if
   * show_instructions is set.
   *
   * @brief set rd to imm_u.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_lui(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract the rd and imm_u values. Then
   * the sum of the current pc and the imm_u will
   * be stored in register rd. it will print instruction
   * details if show_instructions is set.
   *
   * @brief store sum of pc and imm in rd.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_auipc(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers to
   * extract the rs1, rs2 and imm_b. If register
   * rs1 is equal to register rs2 then imm_b will
   * be added to the pc register.
   *
   * @brief add imm_b to pc.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_beq(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract the rs1, rs2 and imm_b. If the
   * signed value in rs1 register is greater than
   * or equal to the signed value in register rs2
   * then add imm_b to pc register. If show_instructions
   * is  set then instruction details will be printed.
   *
   * @brief add imm_b to pc register.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_bge(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract the rs1, rs2 and imm_b. If the
   * signed value in rs1 register is greater than
   * or equal to the signed value in register rs2
   * then add imm_b to pc register. If show_instructions
   * is  set then instruction details will be printed.
   *
   * @brief add imm_b to pc register.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_bgeu(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract the rs1, rs2 and imm_b. If the
   * signed value in rs1 register is less than
   * the signed value in register rs2
   * then add imm_b to pc register. If show_instructions
   * is  set then instruction details will be printed.
   *
   * @brief add imm_b to pc register.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_blt(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract the rs1, rs2 and imm_b. If the
   * unsigned value in rs1 register is less than
   * the unsigned value in register rs2
   * then add imm_b to pc register. If show_instructions
   * is  set then instruction details will be printed.
   *
   * @brief add imm_b to pc register.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_bltu(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract the rs1, rs2 and imm_b. If the
   * value in rs1 register is not equal to
   * the value in register rs2
   * then add imm_b to pc register. If show_instructions
   * is  set then instruction details will be printed.
   *
   * @brief add imm_b to pc register.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_bne(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract the rd. Set register rd to the
   * sum of pc and 4. Then set the pc to
   * the address given by the sum of pc and imm_j.
   * if show_instructions is set then instruction details
   * will be printed.
   *
   * @brief set a register rd to pc+4.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_jal(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers to
   * extract rd, imm_i, and rs1. Set register rd
   * to the pc + 4. Then set pc to the sum of
   * rs1 register and imm_i.
   *
   * @brief set register rd to pc+4, jump to rs1+imm_i.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_jalr(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract rd, rs1 and imm_i. It will set
   * register rd to the sum of contents of register
   * rs1 and imm_i. If show_instructions is set then
   * instruction details will be printed.
   *
   * @brief set rd to sum of rs1 and imm_i.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_addi(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract the rd, rs1 and imm_i. It will
   * check if the signed integer value in rs1
   * is less than signed integer value in imm_i
   * then set rd to 1, otherwise set rd to 0.
   * if show_instructions is set then instruction
   * details will be printed.
   *
   * @brief set a register to 1 or 0.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_slti(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract the rd, rs1, and imm_i. If
   * the unsigned integer value in rs1 is less than
   * the unsigned integer value in imm_i then set
   * rd to 1, otherwise to 0. if show_instructions is
   * set then instruction details will be printed.
   *
   * @brief set rd to 1 or 0.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_sltiu(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract the rd, rs1 and imm_i. It will
   * set rd to the bitwise xor of rs1 and imm_i.
   * if show_instructions is set then instruction
   * details will be printed.
   *
   * @brief set rd to xor of rs1 and imm_i.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_xori(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract the rd, rs1 and imm_i. It will
   * set rd to the bitwise or of rs1 and imm_i.
   * if show_instructions is set then instruction
   * details will be printed.
   *
   * @brief set rd to or of rs1 and imm_i.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_ori(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract the rd, rs1 and imm_i. It will
   * set rd to the bitwise and of rs1 and imm_i.
   * if show_instructions is set then instruction
   * details will be printed.
   *
   * @brief set rd to and of rs1 and imm_i.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_andi(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract the rd, rs1 and imm. It will
   * shift the rs1 register contents left by
   * the number of bits specified in the shamt_i
   * and store the result in register rd.
   *
   * @brief shift rs1 left by shamt_i.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_slli(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract the rd, rs1 and imm. It will
   * logical shift the rs1 register contents right by
   * the number of bits specified in the shamt_i
   * and store the result in register rd.
   *
   * @brief logical shift rs1 right by shamt_i.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_srli(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract the rd, rs1 and imm. It will
   * arithmetic shift the rs1 register contents right by
   * the number of bits specified in the shamt_i
   * and store the result in register rd.
   *
   * @brief arithmetic shift rs1 right by shamt_i.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_srai(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract the rd, rs1 and rs2. It will
   * set register rd to the sum of rs1 and rs2.
   * if show_instructions is set then instruction
   * details will be printed.
   *
   * @brief set rd to sum of rs1 and rs2.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_add(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract the rd, rs1 and rs2. It will
   * set register rd to the result of rs1 - rs2.
   * if show_instructions is set then instruction
   * details will be printed.
   *
   * @brief set rd to result of rs1 - rs2.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_sub(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract the rd, rs1 and rs2. It will
   * set register rd to the result of shifting
   * rs1 left by the number of bits specified in
   * the least significant 5 bits of rs2. if show_instructions
   * is set then instruction details will be printed.
   *
   * @brief shift rs1 left by lsb in rs2, store result in rd.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_sll(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract rd, rs1 and rs2. If the signed
   * integer value in rs1 is less than the signed
   * integer value in rs2 then set rd to 1, otherwise
   * set rd to 0. if show_instructions is set then
   * instruction details will be printed.
   *
   * @brief set rd to 0 or 1.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_slt(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract rd, rs1 and rs2. If the unsigned
   * integer value in rs1 is less than the unsigned
   * integer value in rs2 then set rd to 1, otherwise
   * set rd to 0. if show_instructions is set then
   * instruction details will be printed.
   *
   * @brief set rd to 0 or 1.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_sltu(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract rd, rs1 and rs2. It will set
   * rd to the bitwise xor of rs1 and rs2. If
   * show_instructions is set then instruction
   * details will be printed.
   *
   * @brief set rd to bitwise xor of rs1 and rs2.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_xor(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract rd, rs1 and rs2. It will logical
   * shift rs1 right by the number of bits given
   * in the least significant 5 bits of the rs2
   * register and store the result in rd. if
   * show_instructions is set then instruction
   * details will be printed.
   *
   * @brief logical shift rs1 right by lsb in rs2.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_srl(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract rd, rs1 and rs2. It will arithmetic
   * shift rs1 right by the number of bits given
   * in the least significant 5 bits of the rs2
   * register and store the result in rd. if
   * show_instructions is set then instruction
   * details will be printed.
   *
   * @brief arithmetic shift rs1 right by lsb in rs2.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_sra(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract rd, rs1 and rs2. It will set
   * register rd to the bitwise or of rs1 and rs2.
   * if show_instructions is set then instruction
   * details will be printed.
   *
   * @brief set rd to bitwise or of rs1 and rs2.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_or(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract rd, rs1 and rs2. It will set
   * register rd to the bitwise and of rs1 and rs2.
   * if show_instructions is set then instruction
   * details will be printed.
   *
   * @brief set rd to bitwise and of rs1 and rs2.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_and(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract rd, imm_i and rs1. It will
   * set register rd to the value of the
   * signed extended byte fetched from the
   * memory address given by the sum of rs1
   * and imm_i. if show_instructions is set
   * then instruction details will be printed.
   *
   * @brief set rd to byte at (rs1 + imm_i)
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_lb(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract rd, imm_i and rs1. It will
   * set register rd to the value of the
   * zero extended byte fetched from the
   * memory address given by the sum of rs1
   * and imm_i. if show_instructions is set
   * then instruction details will be printed.
   *
   * @brief set rd to byte at (rs1 + imm_i)
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_lbu(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract rd, imm_i and rs1. It will
   * set register rd to the value of the
   * sign extended half-word fetched from the
   * memory address given by the sum of rs1
   * and imm_i. if show_instructions is set
   * then instruction details will be printed.
   *
   * @brief set rd to half-word at (rs1 + imm_i)
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_lh(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract rd, imm_i and rs1. It will
   * set register rd to the value of the
   * zero extended half-word fetched from the
   * memory address given by the sum of rs1
   * and imm_i. if show_instructions is set
   * then instruction details will be printed.
   *
   * @brief set rd to half-word at (rs1 + imm_i)
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_lhu(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract rd, imm_i and rs1. It will
   * set register rd to the value of the
   * sign extended word fetched from the
   * memory address given by the sum of rs1
   * and imm_i. if show_instructions is set
   * then instruction details will be printed.
   *
   * @brief set rd to word at (rs1 + imm_i)
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_lw(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract rs2, imm_s and rs1. It will
   * set the byte of memory at the address given
   * by the sum of rs1 and imm_s to the 8
   * LSB of rs2.
   *
   * @brief set byte of memory to 8 LSB of (rs2)
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_sb(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract rs2, imm_s and rs1. It will
   * set the half-word of memory at the address given
   * by the sum of rs1 and imm_s to the 16
   * LSB of rs2.
   *
   * @brief set half-word of memory to 16 LSB of (rs2)
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_sh(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract rs2, imm_s and rs1. It will
   * set the word of memory at the address given
   * by the sum of rs1 and imm_s to the 32
   * LSB of rs2.
   *
   * @brief set word of memory to 32 LSB of (rs2)
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_sw(uint32_t insn, std::ostream *pos);
  /**
   * This function will set halt to true
   * and set the halt reason to Ebreak. It
   * will cause the simulator to halt.
   *
   * @brief set halt to true.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_ebreak(uint32_t insn, std::ostream *pos);
  /**
   * This function will set halt to true
   * and set the halt reason to Ecall. It
   * will cause the simulator to halt.
   *
   * @brief set halt to true.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_ecall(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract ther rd and rs1. It will set
   * rd to csr and it will set csr to rs1.
   * if show_instructions is set then instruction
   * details will be printed.
   *
   * @brief set rd to csr and csr to rs1.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_csrrw(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract ther rd and rs1. It will set
   * rd to csr and it will set csr to the
   * result of rs1 bitwise or with csr.
   * if show_instructions is set then instruction
   * details will be printed.
   *
   * @brief set rd to csr and csr to rs1.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_csrrs(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract ther rd and rs1. It will set
   * rd to csr and it will set csr to the
   * result of rs1 bitwise and with ~(csr).
   * if show_instructions is set then instruction
   * details will be printed.
   *
   * @brief set rd to csr and csr to rs1.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_csrrc(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract ther rd and rs1. It will set
   * rd to csr and it will set csr to the
   * zimm.
   * if show_instructions is set then instruction
   * details will be printed.
   *
   * @brief set rd to csr and csr to rs1.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_csrrwi(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract ther rd and rs1. It will set
   * rd to csr and it will set csr to the
   * result of zimm bitwise or with csr.
   * if show_instructions is set then instruction
   * details will be printed.
   *
   * @brief set rd to csr and csr to rs1.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_csrrsi(uint32_t insn, std::ostream *pos);
  /**
   * This function will use get_xxx() helpers
   * to extract ther rd and rs1. It will set
   * rd to csr and it will set csr to the
   * result of ~(zimm) bitwise and with csr.
   * if show_instructions is set then instruction
   * details will be printed.
   *
   * @brief set rd to csr and csr to rs1.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_csrrci(uint32_t insn, std::ostream *pos);
  /**
   * This function will handles processing
   * an instruction which is not known to the
   * simulator. it will print a warning message
   * detailing that an illegal instruction was
   * detected.
   *
   * @brief handle illegal instructions.
   *
   * @param insn instruction to process.
   * @param pos previous cout.
   *
   ********************************************************************************/
  void exec_illegal_insn(uint32_t insn, std::ostream *pos);

  bool halt = {false};
  std::string halt_reason = {"none"};
  bool show_instructions = {false};
  bool show_registers = {false};

  uint64_t insn_counter = {0};
  uint32_t pc = {0};
  uint32_t mhartid = {0};

protected:
  registerfile regs;
  memory &mem;
};

#endif
