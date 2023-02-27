#include "mmio.h"
#include <stdio.h>
#include <stdlib.h>
#include "bismodriver.h"

// BISMO BASE
#define BISMO_BASE 0x90020000

unsigned int bismo_ref(unsigned int x, unsigned int y)
{
  return x;
}

// DOC include start: BISMO test
int main(void)
{
  printf("Running \n");
  
  
  // print_hardware_config();
  
  volatile uint8_t *fetch_enable = (volatile uint8_t *) 0x90020000;
	*fetch_enable = 1;

  volatile uint32_t *bram_id_range = (volatile uint32_t *) 0x9002004cL;
	*bram_id_range = 1;
  // size_t nwords = 8;
  // size_t nbytes = nwords * sizeof(uint64_t);

  // const size_t nrhs = DPA_RHS * 1, nlhs = DPA_LHS * 2;

  // uint32_t result = 1;
  // uint64_t *hostbuf[nwords];

  // for(int i = 0; i < nwords; i++) {
  //   hostbuf[i] = i+1;
  // }

  // // Fetch runcfg variables
  // FetchRunCfg fetch_run_cfg_variables = {
  //     .bram_addr_base = 0,
  //     .bram_id_start = 1,
  //     .bram_id_range = 1,
  //     .dram_base = hostbuf,
  //     .dram_block_offset_bytes = 0,
  //     .dram_block_size_bytes = nbytes,
  //     .dram_block_count = 1,
  //     .tiles_per_row = nwords};

  // // Exec runcfg variables
  // ExecRunCfg exec_run_cfg_variables = {
  //     .lhs_offset = 8,
  //     .rhs_offset = 8,
  //     .num_tiles = 2,
  //     .shift_amount = 3,
  //     .negate = 1,
  //     .clear_before_first_accumulation = 1,
  //     .write_en = 1,
  //     .write_addr = 64};

  // // Result runcfg variables
  // ResultRunCfg result_run_cfg_variables = {
  //     .dram_base = 500,
  //     .dram_skip = nlhs * sizeof(uint32_t),
  //     .wait_complete = 1,
  //     .wait_complete_bytes = 1,
  //     .resmem_addr = 64};

  // // Prints hardware config
  // // print_hardware_config();

  // HardwareCfg hwcfg = get_bismo_hardware_cfg();

  // uint32_t dpa_y = hwcfg.dpa_dim_lhs;
  // uint32_t dpa_x = hwcfg.dpa_dim_rhs;
  // uint32_t dpa_z = hwcfg.dpa_dim_common;
  // uint32_t dpa_z_bytes = dpa_z / 8;
  // reg_write8()
  // int current_bram_region = 0;

  // bismo_test(hwcfg.dpa_dim_lhs, hwcfg.dpa_dim_rhs, (hwcfg.dpa_dim_common * hwcfg.lhs_entries_per_mem / 2));
  // // Verify for sanity check
  // // getBISMOHardwareCfg();
  // // verifyFetchRunCfg(fetch_run_cfg_variables);
  // // verifyResultRunCfg(result_run_cfg_variables);

  // // DRAM fetch controller variables
  // push_fetch_runcfg(fetch_run_cfg_variables);
  // Op fetch_op = {
  //   .opcode = opSendToken,
  //   .syncChannel = 0
  // };
  // push_fetch_op(fetch_op);
  // push_fetch_op(fetch_op);
  // push_fetch_op(fetch_op);

  // Exec stage controller variables
  // push_exec_runcfg(exec_run_cfg_variables);

  // Result stage controller variables
  // push_result_runcfg(result_run_cfg_variables);

  //uint32_t op_count = reg_read32(FETCH_OP_COUNT);

  //printf("Fetch op count: %d \n", op_count);
  //printf("Hardware result %d is correct \n", result);
  return 0;
}
// DOC include end: BISMO test