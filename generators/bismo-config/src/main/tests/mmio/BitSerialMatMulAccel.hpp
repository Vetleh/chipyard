#ifndef BitSerialMatMulAccel_H
#define BitSerialMatMulAccel_H
#include <map>
#include <string>
#include <vector>
#include "BISMOAddresses.hpp"
#include "mmio.h"

// template parameters used for instantiating TemplatedHLSBlackBoxes, if any:
typedef unsigned int AccelReg;
typedef uint64_t AccelDblReg;

using namespace std;
class BitSerialMatMulAccel
{
public:
  uintptr_t mem_offset;
  BitSerialMatMulAccel(uintptr_t mem_offset_in)
  {
    mem_offset = mem_offset_in;
  }
  // Result perf
  void set_perf_prf_res_sel(AccelReg value) { reg_write8(offset(PERF_RES_SEL), value); }
  AccelReg get_perf_prf_res_count() { return reg_read32(offset(PERF_RES_COUNT)); }

  // Exec perf
  void set_perf_prf_exec_sel(AccelReg value) { reg_write8(offset(PERF_EXEC_SEL), value); }
  AccelReg get_perf_prf_exec_count() { return reg_read32(offset(PERF_EXEC_COUNT)); }

  // Fetch perf
  void set_perf_prf_fetch_sel(AccelReg value) { reg_write8(offset(PERF_FETCH_SEL), value); }
  AccelReg get_perf_prf_fetch_count() { return reg_read32(offset(PERF_FETCH_COUNT)); }

  // perf cc
  void set_perf_cc_enable(AccelReg value) { reg_write8(offset(CC_ENABLE), value); }
  AccelReg get_perf_cc() { return reg_read32(offset(PERF_CC)); }

  // HW
  AccelReg get_hw_cmdQueueEntries() { return reg_read32(offset(CMD_QUEUE_ENTRIES)); }
  AccelReg get_hw_maxShiftSteps() { return reg_read32(offset(MAX_SHIFT_STEPS)); }
  AccelReg get_hw_accWidth() { return reg_read32(offset(ACC_WIDTH)); }
  AccelReg get_hw_rhsEntriesPerMem() { return reg_read32(offset(RHS_ENTRIES_PER_MEM)); }
  AccelReg get_hw_lhsEntriesPerMem() { return reg_read32(offset(LHS_ENTRIES_PER_MEM)); }
  AccelReg get_hw_dpaDimCommon() { return reg_read32(offset(DPA_DIM_COMMON)); }
  AccelReg get_hw_dpaDimRHS() { return reg_read32(offset(DPA_DIM_RHS)); }
  AccelReg get_hw_dpaDimLHS() { return reg_read32(offset(DPA_DIM_LHS)); }
  AccelReg get_hw_writeChanWidth() { return reg_read32(offset(WRITE_CHAN_WIDTH)); }
  AccelReg get_hw_readChanWidth() { return reg_read32(offset(READ_CHAN_WIDTH)); }

  // op counts
  AccelReg get_result_op_count() { return reg_read32(offset(RESULT_OP_COUNT)); }
  AccelReg get_exec_op_count() { return reg_read32(offset(EXEC_OP_COUNT)); }
  AccelReg get_fetch_op_count() { return reg_read32(offset(FETCH_OP_COUNT)); }

  // Result runcfg
  void set_result_runcfg_bits_resmem_addr(AccelReg value) { reg_write16(offset(RESULT_RUNCFG_RESMEM_ADDR), value); }
  void set_result_runcfg_bits_waitCompleteBytes(AccelReg value) { reg_write8(offset(RESULT_RUNCFG_WAIT_COMPLETE_BYTES), value); }
  void set_result_runcfg_bits_waitComplete(AccelReg value) { reg_write64(offset(RESULT_RUNCFG_WAIT_COMPLETE), value); }
  void set_result_runcfg_bits_dram_skip(AccelDblReg value) { reg_write64(offset(RESULT_RUNCFG_DRAM_SKIP), value); }
  void set_result_runcfg_bits_dram_base(AccelDblReg value) { reg_write64(offset(RESULT_RUNCFG_DRAM_BASE), value); }
  void set_result_runcfg_valid(AccelReg value) { reg_write8(offset(RESULT_RUNCFG_VALID), value); }
  AccelReg get_result_runcfg_ready() { return reg_read8(offset(RESULT_RUNCFG_READY)); }

  // Exec runcfg
  void set_exec_runcfg_bits_writeAddr(AccelReg value) { reg_write32(offset(EXEC_RUNCFG_WRITE_ADDR), value); }
  void set_exec_runcfg_bits_writeEn(AccelReg value) { reg_write8(offset(EXEC_RUNCFG_WRITE_EN), value); }
  void set_exec_runcfg_bits_clear_before_first_accumulation(AccelReg value) { reg_write8(offset(EXEC_RUNCFG_CLEAR_BEFORE_FIRST_ACCUMULATION), value); }
  void set_exec_runcfg_bits_negate(AccelReg value) { reg_write8(offset(EXEC_RUNCFG_NEGATE), value); }
  void set_exec_runcfg_bits_shiftAmount(AccelReg value) { reg_write32(offset(EXEC_RUNCFG_SHIFT_AMOUNT), value); }
  void set_exec_runcfg_bits_numTiles(AccelReg value) { reg_write32(offset(EXEC_RUNCFG_NUM_TILES), value); }
  void set_exec_runcfg_bits_rhsOffset(AccelReg value) { reg_write32(offset(EXEC_RUNCFG_RHS_OFFSET), value); }
  void set_exec_runcfg_bits_lhsOffset(AccelReg value) { reg_write32(offset(EXEC_RUNCFG_LHS_OFFSET), value); }
  void set_exec_runcfg_valid(AccelReg value) { reg_write8(offset(EXEC_RUNCFG_VALID), value); }
  AccelReg get_exec_runcfg_ready() { return reg_read8(offset(EXEC_RUNCFG_READY)); }

  // Fetch runcfg
  void set_fetch_runcfg_bits_bram_id_range(AccelReg value) { reg_write64(offset(FETCH_RUNCFG_BRAM_ID_RANGE), value); }
  void set_fetch_runcfg_bits_bram_id_start(AccelReg value) { reg_write64(offset(FETCH_RUNCFG_BRAM_ID_START), value); }
  void set_fetch_runcfg_bits_bram_addr_base(AccelReg value) { reg_write64(offset(FETCH_RUNCFG_BRAM_ADDR_BASE), value); }
  void set_fetch_runcfg_bits_tiles_per_row(AccelReg value) { reg_write64(offset(FETCH_RUNCFG_TILES_PER_ROW), value); }
  void set_fetch_runcfg_bits_dram_block_count(AccelReg value) { reg_write16(offset(FETCH_RUNCFG_DRAM_BLOCK_COUNT), value); }
  void set_fetch_runcfg_bits_dram_block_offset_bytes(AccelReg value) { reg_write32(offset(FETCH_RUNCFG_DRAM_BLOCK_OFFSET), value); }
  void set_fetch_runcfg_bits_dram_block_size_bytes(AccelReg value) { reg_write32(offset(FETCH_RUNCFG_DRAM_BLOCK_SIZE), value); }
  void set_fetch_runcfg_bits_dram_base(AccelDblReg value) { reg_write64(offset(FETCH_RUNCFG_DRAM_BASE), value); }
  void set_fetch_runcfg_valid(AccelReg value) { reg_write8(offset(FETCH_RUNCFG_VALID), value); }
  AccelReg get_fetch_runcfg_ready() { return reg_read8(offset(FETCH_RUNCFG_READY)); }

  // Result op
  void set_result_op_bits_token_channel(AccelReg value) { reg_write8(offset(RESULT_OP_TOKEN_CHANNEL), value); }
  void set_result_op_bits_opcode(AccelReg value) { reg_write8(offset(RESULT_OP_OPCODE), value); }
  void set_result_op_valid(AccelReg value) { reg_write8(offset(RESULT_OP_VALID), value); }
  AccelReg get_result_op_ready() { return reg_read8(offset(RESULT_OP_READY)); }

  // Exec op
  void set_exec_op_bits_token_channel(AccelReg value) { reg_write8(offset(EXEC_OP_TOKEN_CHANNEL), value); }
  void set_exec_op_bits_opcode(AccelReg value) { reg_write8(offset(EXEC_OP_OPCODE), value); }
  void set_exec_op_valid(AccelReg value) { reg_write8(offset(EXEC_OP_VALID), value); }
  AccelReg get_exec_op_ready() { return reg_read8(offset(EXEC_OP_READY)); }

  // Fetch op
  void set_fetch_op_bits_token_channel(AccelReg value) { reg_write8(offset(FETCH_OP_TOKEN_CHANNEL), value); }
  void set_fetch_op_bits_opcode(AccelReg value) { reg_write8(offset(FETCH_OP_OPCODE), value); }
  void set_fetch_op_valid(AccelReg value) { reg_write8(offset(FETCH_OP_VALID), value); }
  AccelReg get_fetch_op_ready() { return reg_read8(offset(FETCH_OP_READY)); }

  // Enables
  void set_result_enable(AccelReg value) { reg_write8(offset(RESULT_ENABLE), value); }
  void set_exec_enable(AccelReg value) { reg_write8(offset(EXEC_ENABLE), value); }
  void set_fetch_enable(AccelReg value) { reg_write8(offset(FETCH_ENABLE), value); }

  void reset_accel(AccelReg value) { reg_write8(offset(RESET), value); }
  AccelReg get_signature() { return reg_read32(offset(SIGNATURE)); }
  
  uintptr_t offset(uintptr_t addr)
  {
    return addr + mem_offset;
  }
};
#endif
