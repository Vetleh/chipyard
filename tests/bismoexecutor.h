#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "mmio.h"
#include "bismoaddresses.h"

uint32_t m_bytes_to_fetch, m_bytes_to_write;

static void bismo_test(size_t nrows_lhs, size_t nrows_rhs, size_t ncols)
{
    size_t nbits_lhs = 1;
    size_t nbits_rhs = 1;
    bool sgn_lhs = false;
    bool sgn_rhs = false;
    
}

static void makeinstr_fetch_run(FetchRunCfg r){
    if(r.dram_block_size_bytes == r.dram_block_offset_bytes) {
      // merge consecutive blocks to speed up fetch:
      // one big block instead of several smaller ones
      r.dram_block_size_bytes *= r.dram_block_count;
      r.dram_block_offset_bytes *= r.dram_block_count;
      r.dram_block_count = 1;
    }

    verifyFetchRunCfg(r);

    uint32_t fetchPerGroup = r.dram_block_size_bytes * r.dram_block_count;
    m_bytes_to_fetch += fetchPerGroup;
}



// void build_schedule_trivial() {
//     HardwareCfg cfg = get_bismo_hardware_cfg();
//     const uint32_t dpa_y = cfg.dpa_dim_lhs; // DPA Y dimension
//     const uint32_t dpa_x = cfg.dpa_dim_rhs; // DPA X dimension
//     const uint32_t dpa_z = cfg.dpa_dim_common; // DPA z dimension (64)
//     const uint32_t dpa_z_bytes = dpa_z / 8;
//     gemmbitserial::BitSerialMatrix lhs = m_shape.lhs; // Matrix for lhs and rhs
//     gemmbitserial::BitSerialMatrix rhs = m_shape.rhs;
//     int current_bram_region = 0;
//     const size_t bram_regions = FETCHEXEC_TOKENS;
//     int current_resmem_region = 0;
//     const size_t resmem_regions = EXECRES_TOKENS;

//     assert(dpa_z >= cfg.readChanWidth);
//     assert(dpa_z % cfg.readChanWidth == 0);
//     const size_t exec_to_fetch_width_ratio = dpa_z / cfg.readChanWidth;

//     const uint32_t lhs_l0_per_bram = cfg.lhsEntriesPerMem / bram_regions;
//     const uint32_t rhs_l0_per_bram = cfg.rhsEntriesPerMem / bram_regions;

//     const size_t lhs_bytes_per_l0 = dpa_y * dpa_z / 8;
//     const size_t rhs_bytes_per_l0 = dpa_x * dpa_z / 8;
//     // L1 tile. min 1 L0 tile, maximum L0 tiles that fit into OCM.
//     // only tiled along the common dimension
//     const size_t l0_per_stripe = lhs.ncols_a / dpa_z;
//     const size_t lhs_l0_per_l1 = min(lhs_l0_per_bram, l0_per_stripe);
//     const size_t lhs_bytes_per_l1 = lhs_l0_per_l1 * lhs_bytes_per_l0;
//     const size_t rhs_l0_per_l1 = min(rhs_l0_per_bram, l0_per_stripe);
//     const size_t rhs_bytes_per_l1 = rhs_l0_per_l1 * rhs_bytes_per_l0;
//     // L2 tile. min 1 L1 tile, maximum L1 tiles that fit into OCM.
//     // tiled along either:
//     // only common dimension if rows are wider than BRAM (hw-bound)
//     // only lhs/rhs dimension if rows are smaller than BRAM (sw-bound)
//     const size_t lhs_max_l1_hw = lhs_l0_per_bram / lhs_l0_per_l1;
//     const size_t lhs_max_l1_sw = lhs_eff_rows() / dpa_y;
//     const size_t lhs_l1_per_l2 = min(lhs_max_l1_hw, lhs_max_l1_sw);
//     const size_t lhs_bytes_per_l2 = lhs_l1_per_l2 * lhs_bytes_per_l1;
//     const size_t rhs_max_l1_hw = rhs_l0_per_bram / rhs_l0_per_l1;
//     const size_t rhs_max_l1_sw = rhs_eff_rows() / dpa_x;
//     const size_t rhs_l1_per_l2 = min(rhs_max_l1_hw, rhs_max_l1_sw);
//     const size_t rhs_bytes_per_l2 = rhs_l1_per_l2 * rhs_bytes_per_l1;
//     // total L2 tile counts in the matrices
//     // TODO use the minimum-sized of LHS or RHS BRAM capacity here
//     const size_t z_l2_per_matrix = max(1, (lhs.ncols_a / dpa_z) / lhs_l0_per_bram); //l1 tiles per z direction
//     // the L2 tile count obtained by total_bytes / L2_tiles does not have any
//     // axis information (e.g. may be product of LHS and Z tiling)
//     // so divide by the common dimension tiling factor
//     const size_t lhs_l2_per_matrix = (lhsBytes() / lhs_bytes_per_l2) / z_l2_per_matrix; // l1 tiles in y direction per l2 tile
//     const size_t rhs_l2_per_matrix = (rhsBytes() / rhs_bytes_per_l2) / z_l2_per_matrix; // l1 tiles in x direction per l2 tile

//     // TODO l1 tile size is not guaranteed to evenly divide the matrix
//     // due to partial tiles, and same with l2 tile size. need to handle this
//     // either by smart padding/alignment during allocation, or changing tile
//     // upper bounds.
//     assert(lhsBytes() % lhs_bytes_per_l2 == 0);
//     assert(rhsBytes() % rhs_bytes_per_l2 == 0);

//     // ensure the LHS rows are integer multiples of the DPA dims
//     assert(0 == lhs_eff_rows() % dpa_y);
//     assert(0 == rhs_eff_rows() % dpa_x);
//     assert(0 == lhs.ncols_a % dpa_z);
//     //Binary matrix
//     assert(lhs.nbits == 1 && rhs.nbits == 1);

//     /*lhs.printSummary();
//     rhs.printSummary();
//     lhs.printHex();
//     rhs.printHex();
//     cout << "lhs_bytes_per_l0	" <<	lhs_bytes_per_l0	<< endl;
//     cout << "rhs_bytes_per_l0	" <<	rhs_bytes_per_l0	<< endl;
//     cout << "lhs_l0_per_bram	" <<	lhs_l0_per_bram	<< endl;
//     cout << "rhs_l0_per_bram	" <<	rhs_l0_per_bram	<< endl;
//     cout << "l0_per_stripe	" <<	l0_per_stripe	<< endl;
//     cout << "lhs_l0_per_l1	" <<	lhs_l0_per_l1	<< endl;
//     cout << "lhs_bytes_per_l1	" <<	lhs_bytes_per_l1	<< endl;
//     cout << "rhs_l0_per_l1	" <<	rhs_l0_per_l1	<< endl;
//     cout << "rhs_bytes_per_l1	" <<	rhs_bytes_per_l1	<< endl;
//     cout << "lhs_max_l1_hw	" <<	lhs_max_l1_hw	<< endl;
//     cout << "lhs_max_l1_sw	" <<	lhs_max_l1_sw	<< endl;
//     cout << "lhs_l1_per_l2	" <<	lhs_l1_per_l2	<< endl;
//     cout << "lhs_bytes_per_l2	" <<	lhs_bytes_per_l2	<< endl;
//     cout << "rhs_max_l1_hw	" <<	rhs_max_l1_hw	<< endl;
//     cout << "rhs_max_l1_sw	" <<	rhs_max_l1_sw	<< endl;
//     cout << "rhs_l1_per_l2	" <<	rhs_l1_per_l2	<< endl;
//     cout << "rhs_bytes_per_l2	" <<	rhs_bytes_per_l2	<< endl;
//     cout << "z_l2_per_matrix " << z_l2_per_matrix << endl;
//     cout << "lhs_l2_per_matrix	" <<	lhs_l2_per_matrix	<< endl;
//     cout << "rhs_l2_per_matrix	" <<	rhs_l2_per_matrix	<< endl;*/

//     const uint64_t fetch_base_lhs = (uint64_t) m_accelLHS;
//     const uint64_t fetch_base_rhs = (uint64_t) m_accelRHS;
//     uint64_t res_base = (uint64_t) m_accelRes;

//     for(int lhs_l2 = 0; lhs_l2 < lhs_l2_per_matrix; lhs_l2++) {
//       for(int rhs_l2 = 0; rhs_l2 < rhs_l2_per_matrix; rhs_l2++) {
//         for(int z_l2 = 0; z_l2 < z_l2_per_matrix; z_l2++) {
//           // acquire fetch buffers to fill
//           makeinstr_fetch_sync_getexecbuffer();
//           FetchRunCfg frc;
//           // TODO avoid redundant fetches here
//           // fetch lhs l2 tile
//           frc.bram_addr_base = current_bram_region * lhs_l0_per_bram * exec_to_fetch_width_ratio;
//           frc.bram_id_start = 0;
//           frc.bram_id_range = dpa_y - 1;

//           size_t bytesPerFetchGroup, bytesPerRow;
//           bytesPerRow = (lhs.ncols_a / 8);
//           bytesPerFetchGroup = lhs_bytes_per_l2;
//           // was: lhs_l0_per_l1 * lhs_l1_per_l2
//           frc.tiles_per_row = lhs_l0_per_l1 * exec_to_fetch_width_ratio;
//           // size of each block in bytes (contiguous in memory)
//           frc.dram_block_size_bytes = lhs_l0_per_l1 * dpa_z_bytes;
//           frc.dram_base = (void *)(fetch_base_lhs + lhs_l2*z_l2_per_matrix*lhs_bytes_per_l2 + z_l2 * frc.dram_block_size_bytes);
//           // number of blocks to fetch
//           assert(bytesPerFetchGroup % frc.dram_block_size_bytes == 0);
//           assert(bytesPerFetchGroup / frc.dram_block_size_bytes >= 1);
//           frc.dram_block_count = bytesPerFetchGroup / frc.dram_block_size_bytes;
//           // offset to next block to be fetched
//           frc.dram_block_offset_bytes = bytesPerRow;
//           // only issue fetch if not already in cache
//           if(m_cached_lhs[current_bram_region] != (uint64_t) frc.dram_base) {
//             //m_acc->printFetchRunCfg(frc);
//             makeinstr_fetch_run(frc);
//             m_cached_lhs[current_bram_region] = (uint64_t) frc.dram_base;
//           }

//           // fetch rhs l2 tile
//           frc.bram_addr_base = current_bram_region * rhs_l0_per_bram * exec_to_fetch_width_ratio;
//           frc.bram_id_start = dpa_y;
//           frc.bram_id_range = dpa_x - 1;
//           bytesPerFetchGroup = rhs_bytes_per_l2;
//           // was: rhs_l0_per_l1 * rhs_l1_per_l2
//           frc.tiles_per_row = rhs_l0_per_l1 * exec_to_fetch_width_ratio;
//           // size of each block in bytes (contiguous in memory)
//           frc.dram_block_size_bytes = rhs_l0_per_l1 * dpa_z_bytes;
//           frc.dram_base = (void *)(fetch_base_rhs + rhs_l2*z_l2_per_matrix*rhs_bytes_per_l2 + z_l2 * frc.dram_block_size_bytes);
//           // number of blocks to fetch
//           assert(bytesPerFetchGroup % frc.dram_block_size_bytes == 0);
//           assert(bytesPerFetchGroup / frc.dram_block_size_bytes >= 1);
//           frc.dram_block_count = bytesPerFetchGroup / frc.dram_block_size_bytes;
//           // offset to next block to be fetched
//           frc.dram_block_offset_bytes = bytesPerRow;

//           // only issue fetch if not already in cache
//           if(m_cached_rhs[current_bram_region] != (uint64_t) frc.dram_base) {
//             //m_acc->printFetchRunCfg(frc);
//             makeinstr_fetch_run(frc);
//             m_cached_rhs[current_bram_region] = (uint64_t) frc.dram_base;
//           }

//           // send the prepared buffers to exec
//           makeinstr_fetch_sync_putexecbuffer();

//           // process the fetched L2 tile
//           // exec stage acquires input matrix buffers
//           makeinstr_exec_sync_getfetchbuffer();
//           // process combinations of L1 tiles within the L2 tile
//           for(int lhs_l1 = 0; lhs_l1 < lhs_l1_per_l2; lhs_l1++) {
//             for(int rhs_l1 = 0; rhs_l1 < rhs_l1_per_l2; rhs_l1++) {
//               if(z_l2 == z_l2_per_matrix - 1) {
//                 // about to finish a new stripe
//                 // exec stage acquires new result buffer
//                 makeinstr_exec_sync_getresultbuffer();
//               }
//               ExecRunCfg erc;
//               erc.numTiles = lhs_l0_per_l1;
//               erc.lhsOffset = current_bram_region * lhs_l0_per_bram + lhs_l1 * erc.numTiles;
//               erc.lhsOffset *= exec_to_fetch_width_ratio;
//               erc.rhsOffset = current_bram_region * rhs_l0_per_bram + rhs_l1 * erc.numTiles;
//               erc.rhsOffset *= exec_to_fetch_width_ratio;
//               erc.doNegate = 0;
//               erc.shiftAmount = 0;
//               erc.doClear = (z_l2 == 0 ? 1 : 0); // clear when starting new stripe
//               // write result at the end of z tile
//               erc.writeEn = (z_l2 == z_l2_per_matrix - 1 ? 1 : 0);
//               erc.writeAddr = current_resmem_region;
//               //m_acc->printExecRunCfg(erc);
//               makeinstr_exec_run(erc);

//               if(z_l2 == z_l2_per_matrix - 1) {
//                 // finishing a stripe: release result buffer from exec
//                 makeinstr_exec_sync_putresultbuffer();
//                 // result stage: acquire result buffer
//                 makeinstr_result_sync_getexecbuffer();
//                 // generate result
//                 ResultRunCfg rrc;
//                 rrc.resmem_addr = current_resmem_region;
//                 // find the inds of which L1 tile we are currently working on
//                 size_t lhs_tile = lhs_l1_per_l2 * lhs_l2 + lhs_l1;
//                 size_t rhs_tile = rhs_l1_per_l2 * rhs_l2 + rhs_l1;
//                 rrc.dram_base = get_result_tile_ptr(lhs_tile, rhs_tile);
//                 rrc.dram_skip = lhs_eff_rows() * sizeof(ResultType);
//                 rrc.waitComplete = false;
//                 rrc.waitCompleteBytes = 0;
//                 makeinstr_result_run(rrc);
//                 makeinstr_result_sync_putexecbuffer();
//                 // use next resmem region for next time
//                 current_resmem_region = current_resmem_region < resmem_regions-1 ? current_resmem_region + 1 : 0;
//               }
//             }
//           }
//           // finished processing L2 tile
//           // exec releases input matrix buffers
//           makeinstr_exec_sync_putfetchbuffer();
//           current_bram_region = current_bram_region < bram_regions-1 ? current_bram_region + 1 : 0;
//         }
//       }
//     }
//     // wait until all result writes are complete
//     ResultRunCfg rrc;
//     rrc.waitComplete = true;
//     rrc.waitCompleteBytes = resBytes();
//     // these params are ignored when waitComplete = true
//     rrc.resmem_addr = 0;
//     rrc.dram_base = 0;
//     rrc.dram_skip = 0;
//     makeinstr_result_run(rrc);
//   }
// };