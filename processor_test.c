// compile with: gcc processor.c dist_sampling.c processor_test.c -o b.out -lm

#include "processor.h"

int main(int argc, char * argv[])
{
  // init
  Processor proc = {0,0,0,0.0};

  // test getters and setters
  printf("proc.access time: %5d\tExpected: 0\n", getAccessTime(&proc));
  incrAccessTime(&proc);
  printf("proc.access time: %5d\tExpected: 1\n", getAccessTime(&proc));
  printf("proc.access counter: %5d\tExpected: 0\n", getAccessCounter(&proc));
  incrAccessCounter(&proc);
  printf("proc.access counter: %5d\tExpected: 1\n", getAccessCounter(&proc));

  proc.access_time = 0;
  proc.access_counter = 0;
  proc.mem_mod_requested = 0;
  proc.time_cumulative_avg = 0.0;

  // test calc_time_cumulative_avg
  int curr_cycle = 1523;
  // edge case: access_counter == 0
  printf("calc_time_cumulative_avg: %5.4f\tExpected: 0\n", calc_time_cumulative_avg(&proc, curr_cycle));
  // curr_cycle == 1523, access_counter == 381
  proc.access_counter = 381;
  printf("calc_time_cumulative_avg: %5.4f\tExpected: ~3.9973\n", calc_time_cumulative_avg(&proc, curr_cycle));

  // test generate_request
  printf("\nMake sure sample indices are within range [0, mSize], where mSize = 2048\n");
  for(int i = 0; i < 20; i++)
    printf("unif sample: %5.4d\n", generate_request(&proc, 2048, 'u'));

  printf("\nMake sure sample indices are within range [0, mSize], where mSize = 2048\n");

  proc.mem_mod_requested = 2; // set mem_mod_requested (i.e. mu_pi in pdf)
  for(int i = 0; i < 20; i++)
    printf("norm sample: %5.4d\n", generate_request(&proc, 2048, 'n'));

  return 0;
}
