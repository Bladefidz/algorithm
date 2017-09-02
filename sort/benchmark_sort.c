#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sort.h"
#include "converter.h"
#define INT_BIT_LEN  10  // Max length of unsigned integer is 4,294,967,295


/* Prototype declarations */
void startTimers(time_t *wallClock, clock_t *cpuClock);
void endTimers(time_t *wallClock, clock_t *cpuClock);


/**
 * Main program
 * Benchmark sort algorithm.
 *
 * # Running time
 * Used CPU Clock Time to measure actual thread execution.
 * reference:
 *     http://www.gnu.org/software/libc/manual/html_node/CPU-Time.html
 *     http://beige.ucs.indiana.edu/B673/node104.html
 *
 * @param  argc Argument count
 * @param  argv Argument variable
 * @return      integer
 */
int main(int argc, char *argv[])
{
    int *intArray, len, asc, i, failed;
    char *buffer;
    FILE *file;

    failed = 0;

    // Timing based on wall clock and not concern about threading
    time_t elapsedWallClock = 0;

    // CPU clock time measurement is the sum of three terms:
    // CPU time, I/O time, and the communication channel delay
    clock_t elapsedCPUCycle = 0;

    if (argc > 5) {
        // Get ascending argument
        if (strcmp(argv[3], "asc") == 0) {
            asc = 1;
        } else if (strcmp(argv[3], "desc") == 0) {
            asc = 0;
        } else {
            printf("Invalid argument order!\n"
                "Automatically used ascending order\n");
            asc = 1;
        }

        // Get data set length
        len = stringToInteger(argv[5]);

        // Allocate array of integer
        intArray = (int *) malloc(len * sizeof(int));

        if (strcmp(argv[1], "time") == 0) {
            printf("Opening file...\n");
            file = fopen(argv[4], "r");

            if (file) {
                printf("Reading file...\n");

                i = 0;
                buffer = (char *) malloc(INT_BIT_LEN * sizeof(char));

                while (i < len && fgets(buffer, 12, (FILE*) file)) {
                    intArray[i] = bufferLineToInteger(buffer);
                    i++;
                }

                fclose(file);

                printf("Success: Done read file!\n");
                printf("Calculate running time of %s sort...\n", argv[2]);

                if (strcmp(argv[2], "insertion") == 0) {
                    startTimers(&elapsedWallClock, &elapsedCPUCycle);
                    insertionSort(intArray, len, asc);
                    endTimers(&elapsedWallClock, &elapsedCPUCycle);
                } else if (strcmp(argv[2], "selection") == 0) {
                    startTimers(&elapsedWallClock, &elapsedCPUCycle);
                    selectionSort(intArray, len, asc);
                    endTimers(&elapsedWallClock, &elapsedCPUCycle);
                } else if (strcmp(argv[2], "bubble") == 0) {
                    startTimers(&elapsedWallClock, &elapsedCPUCycle);
                    bubbleSort(intArray, len, asc);
                    endTimers(&elapsedWallClock, &elapsedCPUCycle);
                } else if (strcmp(argv[2], "shell") == 0) {
                    startTimers(&elapsedWallClock, &elapsedCPUCycle);
                    shellSort(intArray, len, asc);
                    endTimers(&elapsedWallClock, &elapsedCPUCycle);
                } else if (strcmp(argv[2], "quick") == 0) {
                    startTimers(&elapsedWallClock, &elapsedCPUCycle);
                    quickSort(intArray, 0, len - 1, asc);
                    endTimers(&elapsedWallClock, &elapsedCPUCycle);
                } else if (strcmp(argv[2], "merge") == 0) {
                    startTimers(&elapsedWallClock, &elapsedCPUCycle);
                    mergeSort(intArray, len - 1, asc);
                    endTimers(&elapsedWallClock, &elapsedCPUCycle);
                } else {
                    failed = 1;
                }

                if (failed <= 0) {
                    printf("Success: running time benchmark of %s sort was ended\n", argv[2]);
                    printf("Elapsed Wall Time: %f s\n",
                        (float) elapsedWallClock);
                    printf("Elapsed CPU Time : %f s\n",
                        (float) elapsedCPUCycle / CLOCKS_PER_SEC);
                } else {
                    printf("Error: %s sort not available\n", argv[2]);
                }
            } else {
                printf("Error: Can not open file!\n");
            }
        } else {
            printf("Error: benchmark_type '%s' is not available!\n", argv[1]);
        }
    } else if (argc == 2) {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            printf("Benchmark sort algorithm by given data set\n\n"
                "Usage: ./benchmark_sort <benchmark_type> <algorithm> "
                "<order> <data_set> <data_set_length>\n"
                "\nBenchmark type (required):\n"
                "   time: Running time\n\n"
                "\nAlgorithm (required): \n"
                "   insertion: Insertion Sort\n"
                "   selection: Selection Sort\n"
                "   bubble: Bubble Sort\n"
                "   shell: Shell Sort\n"
                "   quick: Quick Sort\n"
                "   merge: Merge Sort\n"
                "\nOrder (required):\n"
                "   desc: Descending\n"
                "   asc : Ascending\n"
                "\nData set (required):\n"
                "   Should be stored in file and separated by new_line\n"
                "\nData set length (required):\n"
                "   Unsigned integer: 0 - 4,294,967,295\n"
                "\nExample:\n"
                "   ./benchmark_sort time insertion /path/to/dataset 10\n"
            );
        } else {
            printf("Error: Invalid argument!\n");
        }
    } else {
        printf("Type ./benchmark_sort -h for Help!\n");
    }

    free(intArray);
    free(buffer);
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