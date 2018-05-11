#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Prototype declarations */
void startTimers(time_t *wallClock, clock_t *cpuClock);
void endTimers(time_t *wallClock, clock_t *cpuClock);

int main(int argc, char const *argv[])
{
	/* code */
	return 0;
}

void startTimers(time_t *wallClock, clock_t *cpuClock)
{
    *wallClock = time(NULL);
    *cpuClock = clock();
}

void endTimers(time_t *wallClock, clock_t *cpuClock)
{
    *wallClock = time(NULL) - *wallClock;
    *cpuClock = clock() - *cpuClock;
}