#include "mmio.h"

#define FETCH_ENABLE 0x1000
#define FETCH_ENABLE_R 0x1004

unsigned int bismo_ref(unsigned int x, unsigned int y) {
  while (y != 0) {
    if (x > y)
      x = x - y;
    else
      y = y - x;
  }
  return x;
}

// DOC include start: GCD test
int main(void)
{
  uint32_t result, fetch = 1;

  // wait for peripheral to be ready
  while ((reg_read8(FETCH_ENABLE) & 0x1) == 0) ;

  reg_write32(FETCH_ENABLE, fetch);


  // wait for peripheral to complete
  while ((reg_read8(FETCH_ENABLE_R) & 0x1) == 0) ;

  result = reg_read32(FETCH_ENABLE_R);

  if (result != 1) {
    printf("Hardware result %d does not match reference value %d\n", result);
    return 1;
  }
  printf("Hardware result %d is correct for GCD\n", result);
  return 0;
}
// DOC include end: GCD test