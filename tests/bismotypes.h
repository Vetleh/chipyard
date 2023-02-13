#include <stdio.h>
#include "mmio.h"

typedef struct
{
  uint64_t bram_addr_base;
  uint64_t bram_id_start;
  uint64_t bram_id_range;
  void *dram_base;
  uint32_t dram_block_offset_bytes;
  uint32_t dram_block_size_bytes;
  uint16_t dram_block_count;
  uint64_t tiles_per_row;
} FetchRunCfg;

typedef struct
{
  uint32_t lhs_offset;
  uint32_t rhs_offset;
  uint32_t num_tiles;
  uint32_t shift_amount;
  bool negate;
  bool clear_before_first_accumulation;
  bool write_en;
  uint32_t write_addr;
} ExecRunCfg;

typedef struct
{
  uint64_t dram_base;
  uint64_t dram_skip;
  bool wait_complete;
  uint32_t wait_complete_bytes;
  uint32_t resmem_addr;
} ResultRunCfg;

typedef struct
{
  uint32_t read_chan_width;
  uint32_t write_chan_width;
  uint32_t dpa_dim_lhs;
  uint32_t dpa_dim_rhs;
  uint32_t dpa_dim_common;
  uint32_t lhs_entries_per_mem;
  uint32_t rhs_entries_per_mem;
  uint32_t acc_width;
  uint32_t max_shift_steps;
  uint32_t cmd_queue_entries;
} HardwareCfg;

typedef enum {
  opRun = 0, opSendToken, opReceiveToken
} OpCode;

typedef struct {
  OpCode opcode;
  uint32_t syncChannel;
} Op;
