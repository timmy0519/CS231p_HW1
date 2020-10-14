#include "processor.h"

void incrAccessTime(Processor * p)
{
  (p->access_time)++;
}

int getAccessTime(Processor * p)
{
  return p->access_time;
}

/**
 * args: -mSize : number of memory modules
 *       -d     : the type of distribution to sample from
 *                'u' for uniform distribution sampling
 *                'n' for normal distribution sampling
 */
int generate_request(int mSize, char d)
{
  printf("TODO: generate_request not implemented yet\n");
  return 0;
}
