#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include<time.h>

#define SIZE 100000

void swap(int* arr, int i, int j)
{
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

void test_pass2(int *a, int *b){
    int temp  = *a;
    *a = *b;
    *b = temp;
}

// void test_pass(int *arr, int len)
// {
//     for (int i = 0; i < len; i++)
//     {
//         printf("%d", arr[i]);
//     }
// }

int main()
{
    // int arr[] = {4, 3, 5, 2, 1, 6};
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
    
    int len = SIZE;
    bool swapped;
    // test_pass(arr, len);

    start = clock();

    do
    {
        swapped = false;

        for(int i=0 ; i<len-1 ; i++)
        {
            int j = i + 1;

            if(arr[i] > arr[j]){
                swapped = true;
                //swap(arr, i, j);
                test_pass2(&arr[i], &arr[j]);
            }
        }
    }while(swapped);

    end = clock();

    double time_taken = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("BubbleSort of %d elements took %.9f seconds.\n", SIZE, time_taken);
    // printf("Bubble Sorted array\n");
    // for (int i = 0; i < len; i++)
    // {
    //     printf("%d ", arr[i]);
    // }

    return 0;
}