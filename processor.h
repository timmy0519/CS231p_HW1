#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct processor
{
  // int id;
  int access_time;
  // MemoryMod * prevRequest;
}
typedef struct processor Processor;

void incrAccessTime(Processor * p);

int getAccessTime(Processor * p);

/**
 * args: -mSize : number of memory modules
 *       -d     : the type of distribution to sample from
 *                'u' for uniform distribution sampling
 *                'n' for normal distribution sampling
 */
int generate_request(int mSize, char d);

#endif
