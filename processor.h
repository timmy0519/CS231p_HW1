#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct processor
{
  int access_time;
  int mem_mod_requested;
  double time_cumulative_avg;
};
typedef struct processor Processor;

void incrAccessTime(Processor * p);

int getAccessTime(Processor * p);

/**
 * args: -proc  : processor that generates the request
 *       -mSize : number of memory modules
 *       -d     : the type of distribution to sample from
 *                'u' for uniform distribution sampling
 *                'n' for normal distribution sampling
 */
int generate_request(Processor * proc, int mSize, char d);

double calc_time_cumulative_avg(Processor * proc, int curr_cycle);

#endif
