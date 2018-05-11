#include <stdlib.h>
#include "sort.h"

/**
 * Insertion Sort
 * Let A is array.
 * Let n is length of array.
 * Let x is series {2, 3, ..., n}.
 * For every i in x:
 *     Sort A_i to A_0
 *
 * @param arr Array
 * @param len Array length
 * @param asc Ascending
 */
void insertionSort(int *array, int len, int asc)
{
    int i, j, current;

    if (asc >= 1) {
        // Ascending sort
        for (i = 1; i < len; ++i) {
            current = array[i];
            j = i-1;

            while (j >= 0 && current < array[j]) {
                array[j+1] = array[j];
                j--;
            }

            array[j+1] = current;
        }
    } else {
        // Descending sort
        for (i = 1; i < len; ++i) {
            current = array[i];

            for (j = i-1; j >= 0; j--) {
                if (current > array[j]) {
                    array[j+1] = array[j];
                }
            }

            array[j+1] = current;
        }
    }
}

/**
 * Selection sort
 * Let A is array.
 * Let n is length of array.
 * Let x is series {1, 2, ..., n}.
 * For every i in x:
 *     Find smallest value from A_i to A_n to be stored into A_i.
 *
 * @param arr Array
 * @param len Array length
 * @param asc Ascending
 */
void selectionSort(int *array, int len, int asc)
{
    int i, j, smallest;

    if (asc >= 1) {
        // Ascending sort
        for (i = 0; i < len-1; ++i) {
            smallest = i;

            for (j = i+1; j < len; ++j) {
                if (array[j] < array[smallest]) {
                    smallest = j;
                }
            }

            if (array[i] != array[smallest]) {
                array[smallest] = array[i] + array[smallest];
                array[i] = array[smallest] - array[i];
                array[smallest] = array[smallest] - array[i];
            }
        }
    } else {
        // Descending sort
        for (i = 0; i < len-1; ++i)
        {
            smallest = i;

            for (j = i+1; j < len; ++j) {
                if (array[j] > array[smallest]) {
                    smallest = j;
                }
            }

            if (array[i] != array[smallest]) {
                array[smallest] = array[i] + array[smallest];
                array[i] = array[smallest] - array[i];
                array[smallest] = array[smallest] - array[i];
            }
        }
    }
}

/**
 * Bubble Sort
 * Let A is array.
 * Let n is length of array.
 * Let p is series of pairs {A-i, A-i+1}:
 *     [{1, 2}, {2, 3}, ...., {n-1, n}].
 * For i in p:
 *     Swap each pair (i_0 and i_1) if needed.
 * Repeat until swap not occurred.
 *
 * @param arr Array
 * @param len Array length
 * @param asc Ascending
 */
void bubbleSort(int *array, int len, int asc)
{
    int pair1, pair2, swap;

    swap = 1;

    if (asc) {
        while(swap) {
            swap = 0;

            for (pair1 = 0; pair1 < len-1; ++pair1) {
                pair2 = pair1 + 1;

                if (array[pair2] < array[pair1]) {
                    if (swap == 0) {
                        swap = 1;
                    }

                    array[pair2] = array[pair1] + array[pair2];
                    array[pair1] = array[pair2] - array[pair1];
                    array[pair2] = array[pair2] - array[pair1];
                }
            }
        }
    } else {
        while(swap)
        {
            swap = 0;

            for (pair1 = 0; pair1 < len-1; ++pair1) {
                pair2 = pair1 + 1;

                if (array[pair2] > array[pair1]) {
                    if (swap == 0) {
                        swap = 1;
                    }

                    array[pair2] = array[pair1] + array[pair2];
                    array[pair1] = array[pair2] - array[pair1];
                    array[pair2] = array[pair2] - array[pair1];
                }
            }
        }
    }
}

/**
 * Shell Sort
 * Let A is array.
 * Let n is length of array.
 * Let g is series of gaps:
 *     1959, by Donald Shell:
 *         Gap interval is n/2^k: {n/2, n/4, ..., 1}.
 * For i in g:
 *     Sort {A_0(i)+1, A_1(i)+1, A_2(i)+1], ..., A_m+1}
 *     where m less than n.
 *
 * @param array Array data
 * @param len   Array length
 * @param asc   Ascending
 */
void shellSort(int *array, int len, int asc)
{
    int i, j, d, current;

    d = len/2;

    if (asc) {
        // Ascending sort
        while (d >= 1) {
            for (i = d; i < len; i += d) {
                current = array[i];
                j = i - d;

                while (j >= 0 && array[j] > current) {
                    array[j+d] = array[j];
                    j = j - d;
                }

                array[j+d] = current;
            }

            d = d/2;
        }
    } else {
        // Descending sort
        while (d >= 1) {
            for (i = d; i < len; i += d) {
                current = array[i];
                j = i - d;

                while (j >= 0 && array[j] < current) {
                    array[j+d] = array[j];
                    j = j - d;
                }

                array[j+d] = current;
            }

            d = d/2;
        }
    }
}

/**
 * Swap two array
 *
 * @param a Array a
 * @param b Array b
 */
void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

/**
 * Divide array into two partition based on Lumoto's scheme.
 * Ascending.
 *
 * @param  arr  Array
 * @param  low  Lowest index
 * @param  high Highest index
 * @return      integer
 */
int partitionAscending(int *array, int low, int high)
{
    int j;
    int pivot = array[high];
    int i = low - 1;

    for (j = low; j < high; j++)
    {
        if (array[j] < pivot) {
            i++;
            swap(&array[i], &array[j]);
        }
    }

    swap(&array[i + 1], &array[high]);

    return (i+1);
}

/**
 * Divide array into two partition based on Lumoto's scheme.
 * Descending
 *
 * @param  arr  Array
 * @param  low  Lowest index
 * @param  high Highest index
 * @return      integer
 */
int partitionDescending(int *array, int low, int high)
{
    int j;
    int pivot = array[high];
    int i = low - 1;

    for (j = low; j < high; j++)
    {
        if (array[j] > pivot) {
            i++;
            swap(&array[i], &array[j]);
        }
    }

    swap(&array[i + 1], &array[high]);

    return (i+1);
}

/**
 * Quick Sort
 * Let A is array.
 * Let n is length of array.
 * Let pv is pivot.
 * Divide A into two partitions: greater than and lesser than pv.
 * For each partition:
 *     Choose a pivot and divide.
 * Do that recursively until we found a pivot that can not devide A into two partition.
 *
 * @param arr  Array
 * @param low  Lowest index
 * @param high Highest index
 */
void quickSort(int *array, int low, int high, int asc)
{
    int pivot;

    if (low < high) {
        if (asc >= 1) {
            pivot = partitionAscending(array, low, high);
        } else {
            pivot = partitionDescending(array, low, high);
        }

        quickSort(array, low, pivot - 1, asc);
        quickSort(array, pivot + 1, high, asc);
    }
}

/**
 * Merge and sort two sub array.
 * Compare each i in sub-array_A and sub-array_B:
 *     Place each smaller i into right position in union.
 *
 * @param array  Last merged array
 * @param tmp    Next merged array
 * @param begin  Begin index
 * @param median Middle index
 * @param end    Last index
 */
void mergeAscending(int *array, int *tmp, int begin, int median, int end)
{
    int begin1 = median + 1;

    // Compare A{0, ..., median} with B{median+1, ..., n}
    for (int i = begin; i <= end; i++)
    {
        if (begin < (median + 1)
            && (array[begin] <= array[begin1] || begin1 > end)
        ) {
            // As long not A not exhaust
            // and value_i of A lesser than value_i of B
            tmp[i] = array[begin];
            begin++;
        } else {
            // If A have been exhausted
            // or value_i of B lesser than value_i of A
            tmp[i] = array[begin1];
            begin1 = begin1 + 1;
        }
    }
}

/**
 * Merge and sort two sub array.
 * Compare each i in sub-array_A and sub-array_B:
 *     Place each bigger i into right position in union.
 *
 * @param array  Last merged array
 * @param tmp    Next merged array
 * @param begin  Begin index
 * @param median Middle index
 * @param end    Last index
 */
void mergeDescending(int *array, int *tmp, int begin, int median, int end)
{
    int begin1 = median + 1;

    // Compare A{0, ..., median} with B{median+1, ..., n}
    for (int i = begin; i <= end; i++)
    {
        if (begin < (median + 1)
            && (array[begin] >= array[begin1] || begin1 > end)
        ) {
            // As long not A not exhaust
            // and value_i of A bigger than value_i of B
            tmp[i] = array[begin];
            begin++;
        } else {
            // If A have been exhausted
            // or value_i of B bigger than value_i of A
            tmp[i] = array[begin1];
            begin1 = begin1 + 1;
        }
    }
}

/**
 * Split an array and merge into sorted array
 * Let A is array.
 * Let n is length of array.
 * Divide array into n_sub-array until consist only 1 element each sub-array.
 * For each element in adjacent sub-array:
 *     Compare and merge into a sub-array (union).
 * Do union recursively until all sub-array merged into one.
 *
 * @param arr   Array
 * @param left  Start index
 * @param right last index
 */
void splitAndMerge(int *tmp, int *array, int begin, int end, int asc)
{
    int median;

    if (end - begin >= 1) {
        median = (begin + end) / 2;
        splitAndMerge(array, tmp, begin, median, asc);
        splitAndMerge(array, tmp, median + 1, end, asc);

        if (asc >= 1) {
            mergeAscending(tmp, array, begin, median, end);
        } else {
            mergeDescending(tmp, array, begin, median, end);
        }
    }
}

/**
 * Merge sort
 * This sort is stable and simple.
 * Implementing devide and conquer strategy that described in splitAndMerge()
 *
 * @param arr   Array
 * @param left  Start index
 * @param right last index
 */
void mergeSort(int *array, int len, int asc)
{
    int *tmp;

    tmp = (int *) malloc((len + 1) * sizeof(int));

    for (int i = 0; i <= len; ++i)
    {
        tmp[i] = array[i];
    }

    splitAndMerge(tmp, array, 0, len, asc);
}
