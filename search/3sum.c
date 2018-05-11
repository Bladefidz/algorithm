#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

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
void merge(int *array, int *tmp, int begin, int median, int end)
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
void splitAndMerge(int *tmp, int *array, int begin, int end)
{
    int median;

    if (end - begin >= 1) {
        median = (begin + end) / 2;
        splitAndMerge(array, tmp, begin, median);
        splitAndMerge(array, tmp, median + 1, end);
        merge(tmp, array, begin, median, end);
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
void mergeSort(int *array, int len)
{
    int *tmp;

    tmp = (int *) malloc((len + 1) * sizeof(int));

    for (int i = 0; i <= len; ++i)
    {
        tmp[i] = array[i];
    }

    splitAndMerge(tmp, array, 0, len-1);
}

/**
 * Count combination nCr
 * @param  n Maximum options to choose
 * @param  r Number of choose
 * @return   Combination size
 */
int countComb(int n, int r)
{
    int c = r;
    int d = n-r;
    if (d > c) {
        c = d;
        d = r;
    }
    int y = 1;
    //printf("%d %d %d ", n, c, d);
    for (; n > c; n--)
    {
        y *= n;
    }
    // printf("%d ", y);
    n = 1;
    for (; d > 0; d--)
    {
        n *= d;
    }
    // printf("%d ", n);
    return y/n;
}

/**
 * Return an array of arrays of size *returnSize.
 * Note: The returned array must be malloced, assume caller calls free().
 */
int** threeSum(int* nums, int numsSize, int* returnSize)
{
    int ir = 0;
    int **result = (int **) malloc(countComb(numsSize, 3) * sizeof(int));

    // Defensive technique to check zero
    if (numsSize > 2) {
        char isZeros = '1';
        for (int i = 0; i < 3; i++) {
            if (nums[i] != 0) {
                isZeros = '0';
                break;
            }
        }
        if (isZeros == '1') {
            result[0] = (int *) malloc(3 * sizeof(int));
            result[0][0] = 0;
            result[0][1] = 0;
            result[0][2] = 0;
            (*returnSize)++;
            return result;
        }
    }

    // Sort array
    mergeSort(nums, numsSize);

    // Debug sorted array
    for (int i = 0; i < numsSize; i++)
    {
        printf("%d ", nums[i]);
    }
    printf("\n");

    int lasti = INT_MIN;
    int lastj = lasti;
    for (int i = 0; i < numsSize-2; i++)
    {
        if (nums[i] == lasti) {
            // printf("%d\n", i);
            continue;
        }
        for (int j = i+1; j < numsSize; j++)
        {
            if (nums[j] == lastj) {
                continue;
            }
            int complements = -(nums[i] + nums[j]);
            int lo = 0;
            int hi = numsSize - 1;
            int k = -1;
            while (lo <= hi) {
                int mid = lo + (hi - lo) / 2;
                if      (complements < nums[mid]) hi = mid - 1;
                else if (complements > nums[mid]) lo = mid + 1;
                else {
                    k = mid;
                    break;
                }
            }
            // printf("%d %d %d %d %d\n", i, j, complements, k, nums[k]);
            if (k == i && k+1 < numsSize)
                if (nums[k+1] == nums[i])
                    k++;
            if (k == j && k+1 < numsSize)
                if (nums[k+1] == nums[j])
                    k++;
            if (k > j) {
                result[ir] = (int *) malloc(3 * sizeof(int));
                result[ir][0] = nums[i];
                result[ir][1] = nums[j];
                result[ir][2] = nums[k];
                (*returnSize)++;
                ir++;
            }
            lastj = nums[j];
        }
        lasti = nums[i];
    }

    return result;
}

/**
 * Main test
 * @param  argc Argument count
 * @param  argv Argument variables
 * @return      None
 */
int main(int argc, char const *argv[])
{
    // int nums[6] = {-1, 0, 1, 2, -1, -4};
    // int returnSize = 0;
    // int **result = threeSum(nums, 6, &returnSize);
    // printf("%d\n", returnSize);
    // for (int i = 0; i < returnSize; i++)
    // {
    //     for (int j = 0; j < 3; j++)
    //     {
    //         printf("%d ", result[i][j]);
    //     }
    //     printf("\n");
    // }
    // free(result);
    int nums[3] = {1,1,-2};
    int returnSize = 0;
    int **result = threeSum(nums, 3, &returnSize);
    printf("%d\n", returnSize);
    for (int i = 0; i < returnSize; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%d ", result[i][j]);
        }
        printf("\n");
    }
    free(result);

    // printf("%d\n", countComb(5, 3));
    // printf("%d\n", countComb(6, 3));
    // printf("%d\n", countComb(7, 3));
    // printf("%d\n", countComb(8, 3));

    // int x = 3;
    // int *y = &x;
    // (*y)++;
    // printf("%d\n", *y);
}