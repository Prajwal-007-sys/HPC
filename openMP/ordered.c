#include<stdio.h>
#include<omp.h>

int main() {
    #pragma omp parallel num_threads(4)
    {
        #pragma omp ordered
        printf("Thread %d", omp_get_thread_num());
    }
    return 0;
}