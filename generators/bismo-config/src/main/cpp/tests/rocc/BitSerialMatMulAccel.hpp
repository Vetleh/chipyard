#ifndef BitSerialMatMulAccel_H
#define BitSerialMatMulAccel_H
#include <map>
#include <string>
#include <vector>
#include "rocc.h"
#include <stdio.h>

// template parameters used for instantiating TemplatedHLSBlackBoxes, if any:
typedef unsigned int AccelReg;
typedef uint64_t AccelDblReg;
// TODO create all the addresses in a seperate header file for easier
// debugging and making it easier to change later
using namespace std;
class BitSerialMatMulAccel
{
public:
  BitSerialMatMulAccel(){}
  // Result perf
  void set_perf_prf_res_sel(AccelReg value) { bismo_rocc_write(62, value); }
  AccelReg get_perf_prf_res_count() { return bismo_rocc_read(61); }

  // Exec perf
  void set_perf_prf_exec_sel(AccelReg value) { bismo_rocc_write(60, value); }
  AccelReg get_perf_prf_exec_count() { return bismo_rocc_read(59); }

  // Fetch perf
  void set_perf_prf_fetch_sel(AccelReg value) { bismo_rocc_write(58, value); }
  AccelReg get_perf_prf_fetch_count() { return bismo_rocc_read(57); }

  // perf cc
  void set_perf_cc_enable(AccelReg value) { bismo_rocc_write(56, value); }
  AccelReg get_perf_cc() { return bismo_rocc_read(55); }

  // HW
  AccelReg get_hw_cmdQueueEntries() { return bismo_rocc_read(51); }
  AccelReg get_hw_maxShiftSteps() { return bismo_rocc_read(50); }
  AccelReg get_hw_accWidth() { return bismo_rocc_read(49); }
  AccelReg get_hw_rhsEntriesPerMem() { return bismo_rocc_read(48); }
  AccelReg get_hw_lhsEntriesPerMem() { return bismo_rocc_read(47); }
  AccelReg get_hw_dpaDimCommon() { return bismo_rocc_read(46); }
  AccelReg get_hw_dpaDimRHS() { return bismo_rocc_read(45); }
  AccelReg get_hw_dpaDimLHS() { return bismo_rocc_read(44); }
  AccelReg get_hw_writeChanWidth() { return bismo_rocc_read(43); }
  AccelReg get_hw_readChanWidth() { return bismo_rocc_read(42); }

  // op counts
  AccelReg get_result_op_count() { return bismo_rocc_read(41); }
  AccelReg get_exec_op_count() { return bismo_rocc_read(29); }
  AccelReg get_fetch_op_count() { return bismo_rocc_read(14); }

  // Result runcfg
  void set_result_runcfg_bits_resmem_addr(AccelReg value) { bismo_rocc_write(38, value); }
  void set_result_runcfg_bits_waitCompleteBytes(AccelReg value) { bismo_rocc_write(40, value); }
  void set_result_runcfg_bits_waitComplete(AccelReg value) { bismo_rocc_write(39, value); }
  void set_result_runcfg_bits_dram_skip(AccelDblReg value) { bismo_rocc_write(37, value); }
  void set_result_runcfg_bits_dram_base(AccelDblReg value) { bismo_rocc_write(36, value); }
  void set_result_runcfg_valid(AccelReg value) { bismo_rocc_write(35, value); }
  AccelReg get_result_runcfg_ready() { return bismo_rocc_read(54); }

  // Exec runcfg
  void set_exec_runcfg_bits_writeAddr(AccelReg value) { bismo_rocc_write(27, value); }
  void set_exec_runcfg_bits_writeEn(AccelReg value) { bismo_rocc_write(26, value); }
  void set_exec_runcfg_bits_clear_before_first_accumulation(AccelReg value) { bismo_rocc_write(21, value); }
  void set_exec_runcfg_bits_negate(AccelReg value) { bismo_rocc_write(24, value); }
  void set_exec_runcfg_bits_shiftAmount(AccelReg value) { bismo_rocc_write(28, value); }
  void set_exec_runcfg_bits_numTiles(AccelReg value) { bismo_rocc_write(25, value); }
  void set_exec_runcfg_bits_rhsOffset(AccelReg value) { bismo_rocc_write(22, value); }
  void set_exec_runcfg_bits_lhsOffset(AccelReg value) { bismo_rocc_write(23, value); }
  void set_exec_runcfg_valid(AccelReg value) { bismo_rocc_write(20, value); }
  AccelReg get_exec_runcfg_ready() { return bismo_rocc_read(53); }

  // Fetch runcfg
  void set_fetch_runcfg_bits_bram_id_range(AccelReg value) { bismo_rocc_write(13, value); }
  void set_fetch_runcfg_bits_bram_id_start(AccelReg value) { bismo_rocc_write(12, value); }
  void set_fetch_runcfg_bits_bram_addr_base(AccelReg value) { bismo_rocc_write(11, value); }
  void set_fetch_runcfg_bits_tiles_per_row(AccelReg value) { bismo_rocc_write(10, value); }
  void set_fetch_runcfg_bits_dram_block_count(AccelReg value) { bismo_rocc_write(9, value); }
  void set_fetch_runcfg_bits_dram_block_offset_bytes(AccelReg value) { bismo_rocc_write(8, value); }
  void set_fetch_runcfg_bits_dram_block_size_bytes(AccelReg value) { bismo_rocc_write(7, value); }
  void set_fetch_runcfg_bits_dram_base(AccelDblReg value) { bismo_rocc_write(6, value); }
  void set_fetch_runcfg_valid(AccelReg value) { bismo_rocc_write(5, value); }
  AccelReg get_fetch_runcfg_ready() { return bismo_rocc_read(52); }

  // Result op
  void set_result_op_bits_token_channel(AccelReg value) { bismo_rocc_write(32, value); }
  void set_result_op_bits_opcode(AccelReg value) { bismo_rocc_write(31, value); }
  void set_result_op_valid(AccelReg value) { bismo_rocc_write(33, value); }
  AccelReg get_result_op_ready() { return bismo_rocc_read(34); }

  // Exec op
  void set_exec_op_bits_token_channel(AccelReg value) { bismo_rocc_write(17, value); }
  void set_exec_op_bits_opcode(AccelReg value) { bismo_rocc_write(16, value); }
  void set_exec_op_valid(AccelReg value) { bismo_rocc_write(18, value); }
  AccelReg get_exec_op_ready() { return bismo_rocc_read(19); }

  // Fetch op
  void set_fetch_op_bits_token_channel(AccelReg value) { bismo_rocc_write(2, value); }
  void set_fetch_op_bits_opcode(AccelReg value) { bismo_rocc_write(1, value); }
  void set_fetch_op_valid(AccelReg value) { bismo_rocc_write(3, value); }
  AccelReg get_fetch_op_ready() { return bismo_rocc_read(4); }

  // Enables
  void set_result_enable(AccelReg value) { bismo_rocc_write(30, value); }
  void set_exec_enable(AccelReg value) { bismo_rocc_write(15, value); }
  void set_fetch_enable(AccelReg value) { bismo_rocc_write(0, value); }

  void reset_accel(AccelReg value) { bismo_rocc_write(63, value); }

  static inline void bismo_rocc_write(int idx, unsigned long data)
  {
    ROCC_INSTRUCTION_SS(0, data, idx, 0);
  }

  static inline unsigned long bismo_rocc_read(int idx)
  {
    unsigned long value;
    ROCC_INSTRUCTION_DSS(0, value, 0, idx, 1);
    return value;
  }
};
#endif
