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

#include "cpu_single_hart.h"
#include "memory.h"
#include "rv32i_decode.h"
#include <cstddef>
#include <iostream>
#include <unistd.h>
using std::cerr;
using std::endl;

/**
 * Loop through the vector and for every 4 bytes, call
 * the decode() function to extract the respective instruction
 * and print a formatted string that details the mnemonic and
 * any data associated with that instruction.
 *
 * @brief Loop through the vector and decode 4 bytes at a time.
 *
 * @param mem vector holding bytes to process.
 *
 ********************************************************************************/
static void disassemble(const memory &mem) {
  //
  for (std::size_t i = 0; i < mem.get_size(); i++) {
    if (i % 4 == 0) {
      //
      std::cout << hex::to_hex32(i) << ": " << hex::to_hex32(mem.get32(i))
                << "  " << rv32i_decode::decode(i, mem.get32(i)) << std::endl;
    }
  }
}
/**
 * Print a message to stderr telling the user how to
 * properly run the program and then exiting with a
 * return code of 1.
 *
 * @brief Print a message detailing how to run program.
 *
 ********************************************************************************/
static void usage() {
  cerr << "Usage: rv32i [-m hex-mem-size] infile" << endl;
  cerr << "    -m specify memory size (default = 0x100)" << endl;
  exit(1);
}
/**
 * Check whether user set specific memory size as argument, if not then
 * a default of 0x100 is used. If incorrect arguments are passed in then
 * usage() is called. Then load_file() is used to load the file into
 * the vector. If the file is not properly loaded into the vector then an
 * error message is displayed as well usage() is called. Once the file
 * is loaded into the vector, disassemble() is called and this function
 * wil process the entirety of the vector, printing decoded instructions.
 * then dump() is called in order to dump the contents of the vector onto
 * the screen. Function then returns with success.
 *
 * @brief Load file into memory and disassemble.
 *
 * @param argc argument count.
 * @param argv array containing all arguments.
 *
 * @return Error or success of disassembling file.
 *
 ********************************************************************************/
int main(int argc, char **argv) {
  uint32_t memory_limit = 0x100; // default memory size is 0x100
  bool show_disassembly = false;

  memory mem(memory_limit);
  cpu_single_hart cpu(mem);

  int opt;
  while ((opt = getopt(argc, argv, "dilrm:")) != -1) {
    switch (opt) {
    case 'd': {
      show_disassembly = true;
    } break;
    case 'i': {
      cpu.set_show_instructions(true);
    } break;
    case 'l': { // TODO this requires parameters
      std::cout << "exec limit" << std::endl;
    } break;
    case 'r': {
      cpu.set_show_registers(true);
    } break;
    case 'm': {
      std::istringstream iss(optarg);
      iss >> std::hex >> memory_limit;
    } break;

    default:
      usage();
    }
  }

  if (optind >= argc)
    usage(); // missing filename

  if (!mem.load_file(argv[optind]))
    usage();

  if (show_disassembly)
    disassemble(mem);

  cpu.run(0); // TODO make this actually use exec_limit
  cpu.reset();

  //  mem.dump();

  return 0;
}
