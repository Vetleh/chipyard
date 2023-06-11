#include "mmio.h"
#include <stdio.h>
#include <stdlib.h>
#include "bismodriver.h"
#include "rocc.h"

// BISMO BASE
#define BISMO_BASE 0x90020000

unsigned int bismo_ref(unsigned int x, unsigned int y)
{
  return x;
}

static inline void accum_write(int idx, unsigned long data)
{
	ROCC_INSTRUCTION_SS(0, data, idx, 0);
}

static inline unsigned long accum_read(int idx)
{
	unsigned long value;
	ROCC_INSTRUCTION_DSS(0, value, 0, idx, 1);
	return value;
}

// store mmio: 27
// load mmio: 27

// store rocc: b
// load rocc: e



// DOC include start: BISMO test
int main(void)
{
  // RoCC speed
  accum_write(56, 1);
  long cc_before = accum_read(55);
  //accum_write(0, 1);
  long val = accum_read(43);
  long cc_after = accum_read(55);
  accum_write(56, 0);

  // MMIO speed
  // reg_write8(0x7000214c, 1);
  // uint64_t cc_before = reg_read64(0x70002144);
  // //reg_write8(0x70002000, 1);
  // uint64_t val = reg_read32(0x700020d8);
  // uint64_t cc_after = reg_read64(0x70002144);
  // reg_write8(0x7000214c, 0);

  printf("Values is: %lx\n", val);
  printf("Cycles taken: %lx \n", cc_after - cc_before);
  return 0;
}
// DOC include end: BISMO test