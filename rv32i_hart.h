/*
 */
#include "memory.h"
#include "registerfile.h"
#include "rv32i_decode.h"

class rv32i_hart : public rv32i_decode {
public:
  rv32i_hart(memory &m) : mem(m) {}
  void set_show_instructions(bool b) { show_instructions = b; }
  void set_show_registers(bool b) { show_registers = b; }
  bool is_halted() const { return halt; }
  const std ::string &get_halt_reason() const { return halt_reason; }
  uint64_t get_insn_counter() const { return insn_counter; }
  void set_mhartid(int i) { mhartid = i; }

  void tick(const std ::string &hdr = "");
  void dump(const std ::string &hdr = "") const;
  void reset();

private:
  static constexpr int instruction_width = 35;
  void exec(uint32_t insn, std::ostream *pos);
  void exec_lui(uint32_t insn, std::ostream *pos);
  void exec_auipc(uint32_t insn, std::ostream *pos);
  void exec_beq(uint32_t insn, std::ostream *pos);
  void exec_bge(uint32_t insn, std::ostream *pos);
  void exec_bgeu(uint32_t insn, std::ostream *pos);
  void exec_blt(uint32_t insn, std::ostream *pos);
  void exec_bltu(uint32_t insn, std::ostream *pos);
  void exec_bne(uint32_t insn, std::ostream *pos);
  void exec_jal(uint32_t insn, std::ostream *pos);
  void exec_jalr(uint32_t insn, std::ostream *pos);
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
