#include<stdio.h>
#include<stdlib.h>
#include<omp.h>

int main() 
{
    int a = 10;

    #pragma omp parallel for num_threads(4)
    for(int i = 0; i < a; i++) {
        printf("Thread %d: %d\n", omp_get_thread_num(), i);
    }       
    return 0;
}