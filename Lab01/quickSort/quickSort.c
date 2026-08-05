#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000000

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition_1(int arr[], int low, int high) { // Lomuto partition
    int pivot = arr[high];
    int i = (low - 1);

    for (int j = low ; j < high ; j++){
        if (arr[j] < pivot){
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

int partition_2(int arr[], int low, int high) // Hoare Partition
{
    int pivot = arr[(low + high) / 2];

    int i = low;
    int j = high;

    while (i <= j)
    {
        while (arr[i] < pivot)
            i++;

        while (arr[j] > pivot)
            j--;

        if (i <= j)
        {
            swap(&arr[i], &arr[j]);
            i++;
            j--;
        }
    }

    return j;
}

void quickSort_1 (int arr[], int low, int high) {
    if (low < high) {
        int pi = partition_1(arr, low, high);
        quickSort_1(arr, low, pi - 1);
        quickSort_1(arr, pi + 1, high);
    }
}

void quickSort_2(int arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition_2(arr, low, high);

        quickSort_2(arr, low, pi);
        quickSort_2(arr, pi + 1, high);
    }
}

int main()
{
    int *arr = (int*)malloc(SIZE * sizeof(int));
    if(!arr){
        printf("Memory allocation failed\n");
        return 1;
    }

    srand(time(NULL));

    for (int i = 0 ; i < SIZE ; i++) {
        arr[i] = rand();
    }

    clock_t start, end;
    start = clock();

    quickSort_1(arr, 0, SIZE - 1);

    end = clock();
    double time_taken = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("QuickSort of %d elements took %.2f seconds.\n", SIZE, time_taken);

    free(arr);
    return 0;
}