#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#define N (1024*4)

static __inline__ unsigned long long rdtsc(void)
{
  unsigned hi, lo;
  __asm__ __volatile__ ("RDTSC\n\t"/*read the clock*/
                        "mov %%edx, %0\n\t"
                        "mov %%eax, %1\n\t": "=r" (hi), "=r"
                        (lo):: "%rax", "%rbx", "%rcx", "%rdx");
  return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}


int main(int argc, char* argv[])
{
  unsigned long long a,b;
  char* p;
  a = rdtsc();
  p = (char*)malloc(N);
  b = rdtsc();
  b = b - a;
  printf("cycles spent in allocating %d bytes of memory: %llu\n",N,b);

  return 0;
}
