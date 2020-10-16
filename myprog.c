#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <string.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define N_ARGS 2
#define MAX_M 2048
#define MAX_CYCLES 1000000

/**
 * returns a randomly sampled number from a uniform distribution
 * ranged from 0 to 1
 */
double sample_unif() {
  // return a uniformly distributed random value
  return ( (double)(rand()) + 1. )/( (double)(RAND_MAX) + 1. );
}

/**
 * returns a randomly sampled number from a normal distribution
 */
double sample_norm() {
  // return a normally distributed random value
  double v1=sample_unif();
  double v2=sample_unif();
  // Box-Muller inverse transform
  return cos(2 * M_PI * v2) * sqrt(-2. * log(v1));
}

/**
 * args: -proc  : processor that generates the request
 *       -mSize : number of memory modules
 *       -d     : the type of distribution to sample from
 *                'u' for uniform distribution sampling
 *                'n' for normal distribution sampling
 */
int generate_request(int mu_pi, int mSize, char d)
{
  // return variable
  int new_mem_module_req_index = 0;

  // decide type of distribution to sample from
  if(d == 'n') // normal sampling
  {
    // generate a normally distributed number within norm(mu=proc->mmr, sd=mSize)
    new_mem_module_req_index = sample_norm()
                               * (mSize / 6)  // set std-dev here
                               + mu_pi;       // set mean here
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

/**
 * calculates the time cumulative average for a processor at cycle curr_cycle
 */
double calc_time_cumulative_avg(int curr_cycle, int proc_AC)
{
  if(proc_AC == 0)
    return 0;
  return ((double) curr_cycle) / ((double) proc_AC);
}

int main(int argc, char * argv[]) {
  // expect correct number of command line arguments
  if(argc != N_ARGS + 1)
    return -1;

  // parse command line arguments
  int pSize = atoi(argv[1]);
  char dist_type = argv[2][0];

  // init program state variables
  int requests[pSize];
  int access_counter[pSize];
  int priority[pSize];
  int mu_pi[pSize];
  double time_cumul_avg[pSize];
  int memory_mod_taken[MAX_M];

  // define early termination threshold
  double threshold = 0.00001;

//-----------------------------FROM-M=1-TO-M=2048------------------------------
  // simulate for all m in range [1, 2048]
  for(int mSize = 1; mSize <= MAX_M; mSize++)
  {
    //printf("FROM M=1 TO M=2048 mSize = %d\n", mSize);
    // zero out state variables
    for(int k = 0; k < pSize; k++)
    {
      requests[k] = 0;
      access_counter[k] = 0;
      priority[k] = 0;
      mu_pi[k] = 0;
      time_cumul_avg[k] = 0.0;
    }
    // zero out state variables
    for(int m = 0; m < MAX_M; m++)
    {
      memory_mod_taken[m] = 0;
    }

    // set values for mu_pi
    for(int k = 0; k < pSize; k++)
    {
      if(dist_type == 'n')
      {
        mu_pi[k] = generate_request(mu_pi[k], mSize, 'u');
      }
      else
      {
        mu_pi[k] = 0;
      }
    }

    // initialize w_bar per simulation
    double w_bar_old = 0.0, w_bar = 0.0;

//-----------------------------ONE-SIMULATION-RUN------------------------------
    for(int c = 1; c <= MAX_CYCLES; c++)
    {
      //printf("ONE SIMULATION RUN c = %d\n", c);
//--------------------PROCESS-REQUESTS-FOR-MEMORY-MODULES----------------------
      // process requests for processors that waited last cycle first
      for(int k = 0; k < pSize; k++)
      {
        //printf("PROCESS REQUESTS k = %d\n", k);
        // identified a processor that waited last cycle
        if(priority[k])
        {
          // check if memory module is taken
          if(!memory_mod_taken[requests[k]])
          {
            // grant access to memeory module m
            access_counter[k]++;
            memory_mod_taken[requests[k]] = 1;
            priority[k] = 0;
          }
          else
          {
            // queue processor k for priority next cycle
            priority[k] = 1;
          }
        }
      }
      // process requests for the rest of the processors
      for(int k = 0; k < pSize; k++)
      {
        //printf("PROCESS PRIORITY REQUESTS k = %d\n", k);
        // generate new request iff processor k did not wait last cycle
        if(!priority[k])
        {
          requests[k] = generate_request(mu_pi[k], mSize, dist_type);
        }

        // check if memory module is taken
        if(!memory_mod_taken[requests[k]])
        {
          // grant access to memeory module m
          access_counter[k]++;
          //printf("request[%d] granted access_counter[%d] = %d, we have %d modules\n", k, k, access_counter[k], mSize);
          memory_mod_taken[requests[k]] = 1;
          priority[k] = 0;
        }
        else
        {
          // queue processor k for priority next cycle
          //printf("too bad processor[%d] needs to wait, we have %d modules\n", k, mSize);
          priority[k] = 1;
        }
      }
//-------------------END-PROCESS-REQUESTS-FOR-MEMORY-MODULES-------------------

      //printf("CALC TCA c = %d\n", c);
      // calculate time cumulative averages for all processors
      for(int k = 0; k < pSize; k++)
      {
        time_cumul_avg[k] = calc_time_cumulative_avg(c, access_counter[k]);
      }

      // calculate w_bar at this point of the simulation
      w_bar = 0.0;
      for(int k = 0; k < pSize; k++)
      {
        // sum up each processor's time cumulative average
        w_bar += time_cumul_avg[k];
        //printf("w_bar = %f,TCA[%d] = %f, we have %d modules\n", w_bar, k, time_cumul_avg[k], mSize);
      }
      w_bar /= pSize;
      //printf("c = %d, w = %.4f\n",c , w_bar);
      //printf("w_bar_old = %f, w_bar = %f\n",w_bar_old , w_bar);

      //printf("BEFORE EARLY STOP c = %d\n", c);
      // determine if early termination of simulation run is needed
      if(fabs(1 - (w_bar_old / w_bar)) < threshold)
      {
        // early stop
        //printf("early stop on cycle %d for %d modules. wbo = %f, wb = %f\n", c, mSize, w_bar_old, w_bar);
        break;
      }
      // save w_bar for next cycle's early termination check
      w_bar_old = w_bar;

      // 'free' all memory modules
      for(int m = 0; m < mSize; m++)
      {
        memory_mod_taken[m] = 0;
      }
    }
//------------------------------END-SIMULATION-RUN-----------------------------
    // print the time cumulative average for the simulation (W_bar) to stdout
    printf("%.4f\n", w_bar);
  }
//----------------------------END-FROM-M=1-TO-M=2048---------------------------

  return 0;
}
