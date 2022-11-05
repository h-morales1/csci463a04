/*
 */

#include "cpu_single_hart.h"
#include "registerfile.h"

void cpu_single_hart::run(uint64_t exec_limit) {
  regs.set(2, mem.get_size());
  if (exec_limit == 0) {
    while (!is_halted()) {
      tick("");
    }
  } else {
    while ((!is_halted()) && (exec_limit > get_insn_counter())) {
      tick("");
    }
  }

  if (is_halted()) {
    std::cout << "Execution terminated. Reason: " << get_halt_reason()
              << std::endl;
  }

  std::cout << get_insn_counter() << " instructions executed" << std::endl;
}
