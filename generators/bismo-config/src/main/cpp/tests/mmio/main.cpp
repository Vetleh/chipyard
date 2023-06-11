// Copyright (c) 2018 Norwegian University of Science and Technology (NTNU)
//
// BSD v3 License
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of [project] nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#include "rocc.h"
#include "BISMOTests.hpp"
#include "BitSerialMatMulAccel.hpp"

#define SYS_paddr_to_vaddr 337

int main(int argc, char const *argv[])
{
  uintptr_t mem_offset = syscall(SYS_paddr_to_vaddr, 0x70002000, 0x200);
  BitSerialMatMulAccelDriver *acc = new BitSerialMatMulAccelDriver(mem_offset);
  acc->print_hwcfg_summary();

  bool all_OK = true;
  // all_OK &= test_binary_offchip_widerows_multitile(acc);
  all_OK &= test_binary_onchip_onetile(acc);
  all_OK &= test_binary_onchip_multitile(acc);
  all_OK &= test_binary_offchip_multitile(acc);
  all_OK &= test_binary_offchip_widerows_multitile(acc);

  if (all_OK)
  {
    cout << "All tests passed succesfully" << endl;
  }
  else
  {
    cout << "Some tests failed!" << endl;
  }

  delete acc;
  return 0;
}
