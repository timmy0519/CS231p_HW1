#include "processor.h"
#include "dist_sampling.h"

void incrAccessTime(Processor * p)
{
  (p->access_time)++;
}

void incrAccessCounter(Processor * p)
{
  (p->access_counter)++;
}

int getAccessTime(Processor * p)
{
  return p->access_time;
}

int getAccessCounter(Processor * p)
{
  return p->access_counter;
}

/**
 * args: -proc  : processor that generates the request
 *       -mSize : number of memory modules
 *       -d     : the type of distribution to sample from
 *                'u' for uniform distribution sampling
 *                'n' for normal distribution sampling
 */
int generate_request(Processor * proc, int mSize, char d)
{
  // return variable
  int new_mem_module_req_index = 0;

  // decide type of distribution to sample from
  if(d == 'n') // normal sampling
  {
    // generate a normally distributed number within norm(mu=proc->mmr, sd=mSize)
    new_mem_module_req_index = sample_norm()
                               * (mSize / 6)                // set std-dev here
                               + (proc->mem_mod_requested); // set mean here
  }
  else // uniform sampling
  {
    // generate a uniformly distributed number within [0, mSize]
    new_mem_module_req_index = sample_unif() * mSize;
  }

  // return new index within number of memory modules
  if(new_mem_module_req_index < 0)
  {
    new_mem_module_req_index = mSize + new_mem_module_req_index;
  }
  return new_mem_module_req_index % mSize;
}

double calc_time_cumulative_avg(Processor * proc, int curr_cycle)
{
  double proc_AC = (double) getAccessCounter(proc);
  if(proc_AC == 0)
    return 0;
  return curr_cycle / proc_AC;
}
