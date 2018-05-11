#ifndef SORT_H_
#define SORT_H_

/**
 * Prototype declarations
 */
void insertionSort(int *array, int len, int asc);
void selectionSort(int *array, int len, int asc);
void bubbleSort(int *array, int len, int asc);
void shellSort(int *array, int len, int asc);
void swap(int *a, int *b);
int partitionAscending(int *array, int low, int high);
int partitionDescending(int *array, int low, int high);
void quickSort(int *array, int low, int high, int asc);
void mergeAscending(int *array, int *tmp, int begin, int median, int end);
void mergeDescending(int *array, int *tmp, int begin, int median, int end);
void splitAndMerge(int *tmp, int *array, int begin, int end, int asc);
void mergeSort(int *array, int len, int asc);

#endif