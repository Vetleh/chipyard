#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "mmio.h"
#include "bismotypes.h"
#include "bismoaddresses.h"

#define CMDFIFO_CAP 16
#define FETCHEXEC_TOKENS 2
#define EXECRES_TOKENS 2
#define N_CTRL_STATES 4
#define FETCH_ADDRALIGN 64
#define FETCH_SIZEALIGN 8

#define max(x, y) (x > y ? x : y)
#define FETCH_ALIGN max(FETCH_ADDRALIGN, FETCH_SIZEALIGN)

//    TODO LIST:
//  - May want to change methods to void
//  - May want to move print methods into its own folder to make it easier to maintain
//  - Same as above with sanity checks and verification methods

HardwareCfg m_cfg;

const size_t get_fetch_nodes_per_group()
{
  return (m_cfg.dpa_dim_lhs + m_cfg.dpa_dim_rhs);
}

const size_t get_fetch_first_rhs_id()
{
  return m_cfg.dpa_dim_lhs;
}

// get command counts in FIFOs
const uint32_t fetch_opcount()
{
  return reg_read32(FETCH_OP_COUNT);
}

const uint32_t exec_opcount()
{
  // TODO fix this
  return 0;
  // return reg_read32(EXEC_OP_COUNT);
}
const uint32_t result_opcount()
{
  return reg_read32(RESULT_OP_COUNT);
}

// Checks if op queues are full
const bool fetch_op_full()
{
  return reg_read8(FETCH_OP_READY) == 1 ? false : true;
}
const bool exec_op_full()
{
  return reg_read8(EXEC_OP_READY) == 1 ? false : true;
}
const bool result_op_full()
{
  return reg_read8(RESULT_OP_READY) == 1 ? false : true;
}

// Checks if runcfgs are full
const bool fetch_runcfg_full()
{
  return reg_read8(FETCH_RUNCFG_READY) == 1 ? false : true;
}

const bool exec_runcfg_full()
{
  return reg_read8(EXEC_RUNCFG_READY) == 1 ? false : true;
}
const bool result_runcfg_full()
{
  return reg_read8(RESULT_RUNCFG_READY) == 1 ? false : true;
}

// do a sanity check on a FetchRunCfg in terms of alignment and
// out-of-bounds values
void verifyFetchRunCfg(FetchRunCfg f)
{
  const size_t exec_to_fetch_width_ratio = m_cfg.dpa_dim_common / m_cfg.read_chan_width;
  // ensure all DRAM accesses are aligned
  assert(((uint64_t)f.dram_base) % FETCH_ADDRALIGN == 0);
  assert(f.dram_block_offset_bytes % FETCH_ADDRALIGN == 0);
  assert(f.dram_block_size_bytes % FETCH_SIZEALIGN == 0);
  // ensure that BRAM accesses are within existing range
  assert(f.bram_id_start < get_fetch_nodes_per_group());
  assert(f.bram_id_start + f.bram_id_range < get_fetch_nodes_per_group());
  if (f.bram_id_start < get_fetch_first_rhs_id())
  {
    assert(f.bram_addr_base < m_cfg.lhs_entries_per_mem * exec_to_fetch_width_ratio);
  }
  else
  {
    assert(f.bram_addr_base < m_cfg.rhs_entries_per_mem * exec_to_fetch_width_ratio);
  }
}

// do a sanity check on a ResultRunCfg in terms of alignment and
// out-of-bounds values
void verifyResultRunCfg(ResultRunCfg r)
{
  // ensure all DRAM accesses are aligned to 8 bytes
  assert(((uint64_t)r.dram_base) % 8 == 0);
  assert(r.dram_skip % 8 == 0);
}

static inline HardwareCfg get_bismo_hardware_cfg()
{
  m_cfg.read_chan_width = reg_read32(READ_CHAN_WIDTH);
  m_cfg.write_chan_width = reg_read32(WRITE_CHAN_WIDTH);
  m_cfg.dpa_dim_lhs = reg_read32(DPA_DIM_LHS);
  m_cfg.dpa_dim_rhs = reg_read32(DPA_DIM_COMMON);
  m_cfg.dpa_dim_common = reg_read32(DPA_DIM_COMMON);
  m_cfg.lhs_entries_per_mem = reg_read32(LHS_ENTRIES_PER_MEM);
  m_cfg.rhs_entries_per_mem = reg_read32(RHS_ENTRIES_PER_MEM);
  m_cfg.acc_width = reg_read32(ACC_WIDTH);
  m_cfg.max_shift_steps = reg_read32(MAX_SHIFT_STEPS);
  m_cfg.cmd_queue_entries = reg_read32(CMD_QUEUE_ENTRIES);
  return m_cfg;
};

static inline void print_hardware_config()
{
  printf("Hardware configuration: \n");
  printf("Read channel width: %d \n", reg_read32(READ_CHAN_WIDTH));
  printf("Write channel width: %d \n", reg_read32(WRITE_CHAN_WIDTH));
  printf("DPA dimension LHS: %d \n", reg_read32(DPA_DIM_LHS));
  printf("DPA dimension RHS: %d \n", reg_read32(DPA_DIM_RHS));
  printf("DPA dimension common: %d \n", reg_read32(DPA_DIM_COMMON));
  printf("LHS entries per mem: %d \n", reg_read32(LHS_ENTRIES_PER_MEM));
  printf("RHS entries per mem: %d \n", reg_read32(RHS_ENTRIES_PER_MEM));
  printf("Acc width: %d \n", reg_read32(ACC_WIDTH));
  printf("Max shift steps: %d \n", reg_read32(MAX_SHIFT_STEPS));
  printf("Command queue entries: %d \n", reg_read32(CMD_QUEUE_ENTRIES));
  printf("\n");
}

static inline unsigned int push_fetch_runcfg(
    FetchRunCfg cfg)
{
  // DRAM fetch variables
  reg_write64(FETCH_RUNCFG_DRAM_BASE, (uint64_t)cfg.dram_base);
  reg_write32(FETCH_RUNCFG_DRAM_BLOCK_SIZE, cfg.dram_block_size_bytes);
  reg_write32(FETCH_RUNCFG_DRAM_BLOCK_OFFSET, cfg.dram_block_offset_bytes);
  reg_write16(FETCH_RUNCFG_DRAM_BLOCK_COUNT, cfg.dram_block_count);

  // BRAM fetch variables
  reg_write64(FETCH_RUNCFG_BRAM_ADDR_BASE, cfg.bram_addr_base);
  reg_write64(FETCH_RUNCFG_BRAM_ID_START, cfg.bram_id_start);
  reg_write64(FETCH_RUNCFG_BRAM_ID_RANGE, cfg.bram_id_range);

  assert(cfg.tiles_per_row < (1 << 16));
  reg_write64(FETCH_RUNCFG_TILES_PER_ROW, cfg.tiles_per_row);

  assert(!fetch_runcfg_full());
  reg_write8(FETCH_RUNCFG_VALID, 1);
  reg_write8(FETCH_RUNCFG_VALID, 0);

  return 1;
}

static inline void print_exec_run_cfg(
    ExecRunCfg cfg)
{
  printf("Exec run configuration: \n");
  printf("LHS offset: %u \n", cfg.lhs_offset);
  printf("RHS offset: %u \n", cfg.rhs_offset);
  printf("Number of tiles: %u \n", cfg.num_tiles);
  printf("Negate: %u \n", cfg.negate);
  printf("Clear before first accumulation: %u \n", cfg.clear_before_first_accumulation);
  printf("Write enable: %u \n", cfg.write_en);
  printf("Write address: %u \n", cfg.write_addr);
}

static void push_fetch_op(Op op)
{
  reg_write8(FETCH_OP_OPCODE, op.opcode);
  reg_write8(FETCH_OP_TOKEN_CHANNEL, op.syncChannel);

  assert(!fetch_op_full);

  reg_write8(FETCH_OP_VALID, 1);
  reg_write8(FETCH_OP_VALID, 0);
}

static void push_exec_op(Op op)
{
  reg_write8(EXEC_OP_OPCODE, op.opcode);
  reg_write8(EXEC_OP_TOKEN_CHANNEL, op.syncChannel);

  assert(!exec_op_full);

  reg_write8(EXEC_OP_VALID, 1);
  reg_write8(EXEC_OP_VALID, 0);
}

static void push_result_op(Op op)
{
  reg_write8(RESULT_OP_OPCODE, op.opcode);
  reg_write8(RESULT_OP_TOKEN_CHANNEL, op.syncChannel);

  assert(!result_op_full);

  reg_write8(RESULT_OP_VALID, 1);
  reg_write8(RESULT_OP_VALID, 0);
}

static inline unsigned int push_exec_runcfg(
    ExecRunCfg cfg)
{
  reg_write32(EXEC_RUNCFG_LHS_OFFSET, cfg.lhs_offset);
  reg_write32(EXEC_RUNCFG_RHS_OFFSET, cfg.rhs_offset);
  reg_write32(EXEC_RUNCFG_NUM_TILES, cfg.num_tiles);
  reg_write8(EXEC_RUNCFG_NEGATE, cfg.negate);
  reg_write8(EXEC_RUNCFG_CLEAR_BEFORE_FIRST_ACCUMULATION, cfg.clear_before_first_accumulation);
  reg_write8(EXEC_RUNCFG_WRITE_EN, cfg.write_en);
  reg_write32(EXEC_RUNCFG_WRITE_ADDR, cfg.write_addr);

  assert(!exec_runcfg_full());

  reg_write8(EXEC_RUNCFG_VALID, 1);
  reg_write8(EXEC_RUNCFG_VALID, 0);
  return 1;
}

static inline unsigned int push_result_runcfg(
    ResultRunCfg result_config)
{
  reg_write64(RESULT_RUNCFG_DRAM_BASE, result_config.dram_base);
  reg_write64(RESULT_RUNCFG_DRAM_SKIP, result_config.dram_skip);
  reg_write8(RESULT_RUNCFG_WAIT_COMPLETE, result_config.wait_complete);
  reg_write32(RESULT_RUNCFG_WAIT_COMPLETE_BYTES, result_config.wait_complete_bytes);
  reg_write32(RESULT_RUNCFG_RESMEM_ADDR, result_config.resmem_addr);

  assert(!result_runcfg_full());

  reg_write8(RESULT_RUNCFG_VALID, 1);
  reg_write8(RESULT_RUNCFG_VALID, 0);
  return 1;
}

static void set_stage_enables(bool fetch, bool exec, bool result)
{
  reg_write8(FETCH_ENABLE, fetch);
  reg_write8(EXEC_ENABLE, exec);
  reg_write8(RESULT_ENABLE, result);
}