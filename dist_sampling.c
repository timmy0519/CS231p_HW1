/**
 * Credit: 
 *   https://www.tutorialspoint.com/generate-random-numbers-following-a-normal-distribution-in-c-cplusplus
 *   https://www.youtube.com/watch?v=EXsdT91XFAY
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/**
 * returns a randomly sampled number from a uniform distribution
 * ranged from 0 to 1
 */
double rand_gen() {
  // return a uniformly distributed random value
  return ( (double)(rand()) + 1. )/( (double)(RAND_MAX) + 1. );
}

/**
 * returns a randomly sampled number from a normal distribution
 */
double normalRandom() {
  // return a normally distributed random value
  double v1=rand_gen();
  double v2=rand_gen();
  // Box-Muller inverse transform
  return cos(2*M_PI*v2)*sqrt(-2.*log(v1));
}

/**
 * CL args: 1) number of times to sample from normal distribution
 */
int main(int argc, char * argv[]) {
  // set mean and sd of gaussian distrobution
  // (default mu = 0, sigma = 1 for normal distribution)
  double sigma = 1;
  double mu = 0;

  // initialize seed for random number generator
  time_t t;
  srand((unsigned) time(&t));

  // input processing
  int N = atoi(argv[1]);
  if(!N)
	return -1;

  // print out N randomly sampled numbers to stdout
  for(int i = 0; i < N-1; i++) {
    // multiplying sigma and adding mu shift range/centering 
    // of sampled numbers to desired parameters
	double x = normalRandom() * sigma + mu;
	printf("%0.4f,\n", x);
  }
  printf("%0.4f\n", normalRandom() * sigma + mu);

  // terminate
  return 0;
}
