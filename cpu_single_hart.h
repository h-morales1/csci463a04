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

#ifndef CPU_SINGLE_HART_H
#define CPU_SINGLE_HART_H

#include "rv32i_hart.h"

class cpu_single_hart : public rv32i_hart {
public:
  /**
   * Constructor for cpu_single_hart that takes in
   * a memory object.
   *
   * @brief constructor accepting memory obj.
   *
   * @param mem vector to pass to constructor.
   *
   ********************************************************************************/
  cpu_single_hart(memory &mem) : rv32i_hart(mem) {
    //
  }

  /**
   * This function will check to see if the exec_limit
   * is 0, if so then it will call the tick() function
   * until it is halted. If an exec limit other than 0
   * is present then it will call tick() until it is
   * halted or until it meets the exec limit. After
   * being halted, it will print how many
   * instructions were executed.
   *
   * @brief execute instructions until halted.
   *
   * @param exec_limit the amount of instructions to execute.
   *
   ********************************************************************************/
  void run(uint64_t exec_limit);
};

#endif
