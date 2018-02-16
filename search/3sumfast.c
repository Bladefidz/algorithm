#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/**
 * Return an array of arrays of size *returnSize.
 * Note: The returned array must be malloced, assume caller calls free().
 */
int numcmp(const void *a, const void *b) {
    return *((int *)a) - *((int *)b);
}

int** threeSum(int* nums, int numsSize, int* returnSize) {
    int **rtn = (int **)malloc(sizeof(int *)*numsSize*numsSize);
    int count = 0;
    qsort(nums, numsSize, sizeof(int), numcmp);

    int64_t pre = INT64_MAX;
    for (int i = 0; i < numsSize-2; ++i) {
        if (nums[i] > 0) break;
        if (pre == nums[i]) continue;
        pre = nums[i];

        int remain = -nums[i];
        int l = i+1, r = numsSize-1;
        while (l < r) {
            int sum = nums[l] + nums[r];
            if (sum == remain) {
                rtn[count] = (int *)malloc(sizeof(int)*3);
                rtn[count][0] = nums[i], rtn[count][1] = nums[l];
                rtn[count][2] = nums[r], ++count;

                while (nums[r] == nums[r-1]) --r;
                while (nums[l] == nums[l+1]) ++l;
                ++l, --r;
            }
            else if (sum > remain) --r;
            else ++l;
        }
    }

    *returnSize = count;
    realloc(rtn, sizeof(int *)*count);
    if (!count) return NULL;
    return rtn;
}